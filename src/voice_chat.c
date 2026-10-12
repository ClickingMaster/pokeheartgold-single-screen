#include "voice_chat.h"

#include "global.h"

#include "heap.h"
#include "wifi_connection.h"

extern void NNS_SndInit(void);
extern void NNS_SndStrmInit(void *stream);
extern void NNS_SndStrmAllocChannel(void *stream, int count, void *channels);
extern void NNS_SndStrmSetVolume(void *stream, int volume);
extern void NNS_SndStrmSetup(void *stream, int format, void *buffer, u32 size, int rate, int channels, void (*callback)(void), void *arg);
extern void NNS_SndStrmStart(void *stream);
extern void NNS_SndStrmStop(void *stream);
extern void NNS_SndStrmFreeChannel(void *stream);
extern void MIC_StartAutoSamplingAsync(MICAutoParam *param, void (*callback)(MICResult, void *), void *arg);
extern u8 *MIC_GetLastSamplingAddress(void);
extern void MIC_StopAutoSampling(void);
extern void VCT_EnableVAD(int enable);
extern void VCT_EnableEchoCancel(int enable);
extern void VCT_SendAudio(void *data, u32 size);
extern void VCT_ReceiveAudio(void *data, u32 size, int unused);
extern Session *VCT_CreateSession(u32 aid);
extern int VCT_Request(Session *session, int request);
extern int VCT_Response(Session *session, int response);
extern int VCT_StartStreaming(Session *session);
extern void VCT_StopStreaming(Session *session);
extern void VCT_DeleteSession(Session *session);
extern void VCT_Main(void);
extern int VCT_HandleData(u8 aid, void *data, void *size, void *work);
extern void VCT_SetCodec(int codec);
extern void VCT_Cleanup(void);
extern void VCT_GetVADInfo(void *info);
extern void CommState_GetLinkMode(void);
extern int sub_02034084(void);
extern int ov00_021EE490(void);

#pragma force_active on

void VoiceChat_InitAudio(void) {
    int i;

    MIC_Init();
    PM_Init();
    PM_SetAmp((PMAmpSwitch)1);
    PM_SetAmpGain((PMAmpGain)3);
    gVoiceChat.work->unk_1A78 = 0;
    gVoiceChat.work->unk_1A84 = 0;
    gVoiceChat.work->ampGain = 0;
    gVoiceChat.work->ampGainChanged = 0;
    gVoiceChat.work->unk_1A88 = 0;
    for (i = 0; i < 16; i++) {
        gVoiceChat.work->unk_1A8C[i] = 0;
    }
    gVoiceChat.work->unk_1AAC = 0;
    VCT_EnableVAD(1);
    NNS_SndInit();
    NNS_SndStrmInit(gVoiceChat.work->stream);
    MIi_CpuClearFast(0, (u32 *)gVoiceChat.work->silenceBuffer, 0x880);
    gVoiceChat.frameBudget = 0;
    VCT_EnableEchoCancel(1);
}

#pragma force_active reset

void VoiceChat_MicCallback(MICResult result, void *arg) {
}

#pragma force_active on

void VoiceChat_StreamCallback(int enabled, u32 count, void **buffers, u32 size, int unused, u8 *address) {
    u32 i;

    if (!enabled) {
        for (i = 0; i < count; i++) {
            MI_CpuFill8(buffers[i], 0, size);
        }
        return;
    }
    if (gVoiceChat.work->restartSampling) {
        MIC_StartAutoSamplingAsync(&gVoiceChat.work->micParam, VoiceChat_MicCallback, NULL);
        gVoiceChat.work->restartSampling = 0;
    }
    if ((u32)(MIC_GetLastSamplingAddress() - address) < size) {
        address += size;
    }
    // lid closed: send silence
    if ((*(volatile u16 *)0x027FFFA8 & 0x8000) >> 15) {
        address = gVoiceChat.work->silenceBuffer;
    }
    if (!gVoiceChat.work->muted) {
        VCT_SendAudio(address, size);
    }
    for (i = 0; i < count; i++) {
        VCT_ReceiveAudio(buffers[i], size, 0);
    }
    gVoiceChat.work->samplingState = 2;
}

#pragma force_active reset

BOOL VoiceChat_RequestSession(u32 aid) {
    Session *session = gVoiceChat.work->session;
    if (session == NULL) {
        session = VCT_CreateSession(aid);
        if (session == NULL) {
            return FALSE;
        }
        aid = VCT_Request(session, 0);
    } else {
        aid = VCT_Request(session, 0);
    }
    if (aid != 0) {
        VCT_DeleteSession(session);
        gVoiceChat.work->session = NULL;
        return FALSE;
    }
    gVoiceChat.work->session = session;
    return TRUE;
}

// The only caller passes 0 (kept: removing the argument changes its code).
BOOL VoiceChat_AcceptSession(int unused) {
    Session *session = gVoiceChat.work->session;
    if (session != NULL && session->mode == 4) {
        if (VCT_Response(session, 0) != 0) {
            return FALSE;
        }
        if (VCT_StartStreaming(gVoiceChat.work->session) != 0) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

void VoiceChat_ConferenceEventCallback(u32 unusedId, u32 event, Session *session, void *unusedContext) {
    switch (event) {
    case 9:
        VoiceChat_EndSession(session);
        return;
    case 7:
        VCT_StartStreaming(session);
        return;
    case 12:
        VoiceChat_EndSession(session);
        return;
    }
}

#pragma force_active on

void VoiceChat_PhoneEventCallback(u32 unusedId, u32 event, Session *session, void *unusedContext) {
    switch (event) {
    case 1:
        if (gVoiceChat.work->session != NULL) {
            VCT_Response(session, 3);
            VCT_DeleteSession(session);
        } else {
            gVoiceChat.work->session = session;
        }
        break;
    case 8:
        VCT_Response(session, 0);
        VoiceChat_EndSession(session);
        WifiConnection_StopVoiceChat();
        break;
    case 9:
        VoiceChat_EndSession(session);
        WifiConnection_StopVoiceChat();
        break;
    case 4:
        VCT_Response(session, 4);
        VoiceChat_EndSession(session);
        break;
    case 7:
        if (session->state != gVoiceChat.work->mode) {
            VoiceChat_EndSession(session);
        } else if (VCT_StartStreaming(session) != 0) {
            gVoiceChat.work->callState = VOICE_CHAT_CALL_TALKING;
        }
        break;
    case 2:
        VoiceChat_EndSession(session);
        break;
    case 3:
    case 11:
    case 12:
        VoiceChat_EndSession(session);
        break;
    }
}

#pragma force_active reset

void VoiceChat_EndSession(Session *session) {
    VCT_StopStreaming(session);
    VCT_DeleteSession(session);
    gVoiceChat.work->session = NULL;
}

#pragma force_active on

void VoiceChat_Update(void) {
    u64 now = OS_GetTick();

    // VCT_Main has to run every 16.666 ms; catch up on the time elapsed since the last frame.
    gVoiceChat.frameBudget += ((u32)now - (u32)gVoiceChat.lastTick) * 64000 / 33514 - 16666;
    if (gVoiceChat.frameBudget < -10000) {
        gVoiceChat.frameBudget = 0;
    }
    gVoiceChat.lastTick = now;
    VCT_Main();
    while (gVoiceChat.frameBudget >= 16666) {
        VCT_Main();
        gVoiceChat.frameBudget -= 16666;
    }
    if (gVoiceChat.work->ampGainChanged) {
        switch (gVoiceChat.work->ampGain) {
        case 0:
            PM_SetAmpGain(PM_AMPGAIN_160);
            break;
        case 1:
            PM_SetAmpGain(PM_AMPGAIN_80);
            break;
        case 2:
            PM_SetAmpGain(PM_AMPGAIN_40);
            break;
        case 3:
            PM_SetAmpGain(PM_AMPGAIN_20);
            break;
        }
        gVoiceChat.work->ampGainChanged = 0;
    }
    if (gVoiceChat.work->mode != VOICE_CHAT_MODE_CONFERENCE) {
        switch (gVoiceChat.work->callState) {
        case VOICE_CHAT_CALL_IDLE:
            if (WifiConnection_GetAid() == 0) {
                if (VoiceChat_RequestSession(1)) {
                    gVoiceChat.work->callState = VOICE_CHAT_CALL_REQUESTING;
                    gVoiceChat.work->requestTimer = 60;
                }
            } else if (WifiConnection_GetAid() == 1) {
                if (VoiceChat_AcceptSession(0)) {
                    gVoiceChat.work->callState = VOICE_CHAT_CALL_TALKING;
                }
            }
            break;
        case VOICE_CHAT_CALL_REQUESTING:
            gVoiceChat.work->requestTimer--;
            if (!gVoiceChat.work->requestTimer) {
                gVoiceChat.work->callState = VOICE_CHAT_CALL_IDLE;
            }
            break;
        case VOICE_CHAT_CALL_TALKING:
            break;
        }
    }
}

#pragma force_active reset

BOOL VoiceChat_ReceiveData(u32 aid, void *data, void *size) {
    if (gVoiceChat.work == NULL) {
        return FALSE;
    }
    if (VCT_HandleData((u8)aid, data, size, gVoiceChat.work)) {
        return TRUE;
    }
    return FALSE;
}

void VoiceChat_AllocAligned(void **allocation, void **aligned, u32 size, u32 heapId) {
    u32 allocSize = size + 0x20;
    *allocation = Heap_Alloc((enum HeapID)heapId, allocSize);
    MI_CpuFill8(*allocation, 0, allocSize);
    *aligned = (void *)(((u32)*allocation + 0x1F) & ~0x1F);
}

#pragma force_active on

void VoiceChat_Init(int heapId, int codec, int count) {
    u8 channels[3] = { 0, 0, 0 };
    u32 allocation;
    InitArgs config;

    if (!gVoiceChat.work) {
        allocation = 0;
        VoiceChat_AllocAligned((void **)&allocation, (void **)&gVoiceChatWork, sizeof(VoiceChatWork), heapId);
        gVoiceChat.work->rawAlloc = (void *)allocation;
        VoiceChat_AllocAligned(&gVoiceChat.work->vctBufferRaw, &gVoiceChat.work->vctBuffer, count * 0x23C0 + 32, heapId);
        gVoiceChat.work->heapId = heapId;
        gVoiceChat.work->endCallback = NULL;
        VoiceChat_InitAudio();
    }
    gVoiceChat.work->micParam.type = (MICSamplingType)3;
    gVoiceChat.work->micParam.buffer = gVoiceChat.work;
    gVoiceChat.work->micParam.size = 0x880;
    gVoiceChat.work->micParam.rate = 0x1040;
    gVoiceChat.work->micParam.loop_enable = TRUE;
    gVoiceChat.work->micParam.full_callback = NULL;
    gVoiceChat.work->micParam.full_arg = NULL;
    gVoiceChat.work->restartSampling = 1;
    NNS_SndStrmAllocChannel(gVoiceChat.work->stream, 1, channels);
    NNS_SndStrmSetVolume(gVoiceChat.work->stream, 0);
    NNS_SndStrmSetup(gVoiceChat.work->stream, 1, gVoiceChat.work->streamBuffer, 0x880, 65, 2, (void (*)(void))VoiceChat_StreamCallback, gVoiceChat.work);
    gVoiceChat.work->callState = VOICE_CHAT_CALL_IDLE;
    gVoiceChat.work->session = NULL;
    CommState_GetLinkMode();
    if (!sub_02034084()) {
        config.mode = VOICE_CHAT_MODE_PHONE;
    } else {
        config.mode = VOICE_CHAT_MODE_CONFERENCE;
    }
    gVoiceChat.work->mode = config.mode;
    config.data = gVoiceChat.work->sessions;
    config.count = count;
    config.id = (u8)ov00_021EE490();
    if (config.id == -1) {
        GF_ASSERT(0);
    }
    if (gVoiceChat.work->mode == VOICE_CHAT_MODE_CONFERENCE) {
        config.callback = VoiceChat_ConferenceEventCallback;
    } else {
        config.callback = VoiceChat_PhoneEventCallback;
    }
    config.arg = NULL;
    config.buffer = gVoiceChat.work->vctBuffer;
    config.size = count * 0x23C0 + 32;
    VCT_Init(&config);
    gVoiceChat.work->muted = FALSE;
    VCT_SetCodec(codec);
    VoiceChat_StartStream();
    VCT_EnableEchoCancel(1);
}

#pragma force_active reset

void VoiceChat_StartStream(void) {
    NNS_SndStrmStart(gVoiceChat.work->stream);
}

// The only caller passes the voice chat active flag (kept: removing the argument changes its code).
void VoiceChat_HangUp(int unused) {
    Session *session = gVoiceChat.work->session;
    u32 callState;
    if (session == NULL || gVoiceChat.work->callState == VOICE_CHAT_CALL_IDLE) {
        WifiConnection_StopVoiceChat();
        return;
    }
    callState = gVoiceChat.work->callState;
    if (callState == VOICE_CHAT_CALL_REQUESTING) {
        if (VCT_Request(session, 2) != 0) {
            WifiConnection_StopVoiceChat();
            return;
        }
    }
    session = gVoiceChat.work->session;
    if (VCT_Request(session, 1) != 0) {
        WifiConnection_StopVoiceChat();
    }
}

void VoiceChat_SetEndCallback(void (*callback)(void)) {
    gVoiceChat.work->endCallback = callback;
}

#pragma force_active on

void VoiceChat_Shutdown(void) {
    void (*callback)(void);
    if (gVoiceChat.work) {
        callback = gVoiceChat.work->endCallback;
        MIC_StopAutoSampling();
        NNS_SndStrmStop(gVoiceChat.work->stream);
        NNS_SndStrmFreeChannel(gVoiceChat.work->stream);
        VCT_Cleanup();
        Heap_FreeExplicit((enum HeapID)gVoiceChat.work->heapId, gVoiceChat.work->vctBufferRaw);
        Heap_FreeExplicit((enum HeapID)gVoiceChat.work->heapId, gVoiceChat.work->rawAlloc);
        gVoiceChat.work = NULL;
        if (callback) {
            callback();
        }
    }
}

int VoiceChat_GetVoiceActivity(void) {
    int info[4];
    if (gVoiceChat.work) {
        VCT_GetVADInfo(info);
        if (info[1] > 2) {
            return info[0];
        }
    }
    return 0;
}

#pragma force_active reset

void VoiceChat_Mute(void) {
    gVoiceChat.work->muted = TRUE;
}

void VoiceChat_Unmute(void) {
    gVoiceChat.work->muted = FALSE;
}
