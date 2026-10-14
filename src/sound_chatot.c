#include "sound_chatot.h"

#include "constants/species.h"

#include "math_util.h"
#include "sound.h"
#include "sound_system.h"
#include "sound_playback.h"

BOOL ChatotSoundMain(void) {
    u8 *waveOutAllocated = GF_SdatGetAttrPtr(16);
    u8 *chatotPlaying = GF_SdatGetAttrPtr(30);

    if (*chatotPlaying == 1) {
        if (*waveOutAllocated == 1) {
            if (Sound_IsWaveOutPlaying(14) == 0) {
                Chatot_StopCry();
                return TRUE;
            }
        } else {
            Chatot_StopCry();
            return TRUE;
        }
    }

    return FALSE;
}

BOOL Chatot_CheckCry(SOUND_CHATOT *chatot) {
    u8 *forceDefaultCry = GF_SdatGetAttrPtr(31);
    u8 *recordingDisabled = GF_SdatGetAttrPtr(54);

    if (!Chatot_Exists(chatot)) {
        return FALSE;
    }

    if (*recordingDisabled == 1) {
        return FALSE;
    }

    if (*forceDefaultCry != 1) {
        return TRUE;
    }

    return FALSE;
}

BOOL Chatot_PlayCry(SOUND_CHATOT *chatot, u32 a1, s32 volume, s32 pan) {
    s8 *buffer = Sound_GetChatotBuffer();
    s8 *chatotPlaying = GF_SdatGetAttrPtr(30);

    if (!Chatot_CheckCry(chatot)) {
        return FALSE;
    }

    StopCry(0);
    Chatot_StopCry();
    Sound_AllocWaveOutChannel(14);
    u16 speedOffset = (LCRandom() % 8192);

    Chatot_Decode(buffer, Chatot_GetData(chatot));

    SoundWaveOutParam waveOutParam;

    waveOutParam.handle = Sound_GetWaveOutHandle(14);
    waveOutParam.format = NNS_SND_WAVE_FORMAT_PCM8;
    waveOutParam.data = Sound_GetChatotBuffer();
    waveOutParam.loopFlag = 0;
    waveOutParam.loopStartSample = 0;
    waveOutParam.samples = 2000;
    waveOutParam.sampleRate = 2000;
    waveOutParam.speed = (u32)(speedOffset + 0x8000);
    waveOutParam.pan = pan / 2 + 64;
    waveOutParam.volume = volume;

    BOOL ret = Sound_StartWaveOut(&waveOutParam, 14);
    Sound_SetWaveOutVolume(14, volume);
    *chatotPlaying = 1;
    Chatot_SetForceDefaultCry(0);

    return ret;
}

void Chatot_StopCry() {
    u8 *waveOutAllocated = GF_SdatGetAttrPtr(16);
    u8 *chatotPlaying = GF_SdatGetAttrPtr(30);

    if (*waveOutAllocated == 1) {
        Sound_StopWaveOutAndFreeBuffer(14);
        Sound_FreeWaveOutChannel(14);
    }

    *chatotPlaying = 0;
}

BOOL Chatot_StartRecording(void) {
    MICAutoParam st0;

    st0.type = MIC_SAMPLING_TYPE_SIGNED_8BIT;
    st0.buffer = Sound_GetChatotBuffer();
    st0.size = 2000;
    if ((st0.size & 0x1f) != 0) {
        st0.size &= ~0x1f;
    }
    st0.rate = HW_CPU_CLOCK_ARM7 / 2000;
    st0.loop_enable = 0;
    st0.full_callback = 0;
    st0.full_arg = 0;

    return GF_MIC_StartAutoSampling(&st0);
}

void Chatot_StopRecording() {
    GF_MIC_StopAutoSampling();
}

void Chatot_SaveRecording(SOUND_CHATOT *chatot) {
    Chatot_Encode(chatot, Sound_GetChatotBuffer());
}

void Chatot_SetForceDefaultCry(u8 forceDefault) {
    u8 *forceDefaultCry = GF_SdatGetAttrPtr(31);
    *forceDefaultCry = forceDefault;
}

void Chatot_PlayCryWithFallback(SOUND_CHATOT *chatot, u32 a1, u32 volume, s32 pan) {
    SOUND_CHATOT **myChatot = GF_SdatGetAttrPtr(36);
    BOOL ret;
    if (chatot == 0) {
        ret = Chatot_PlayCry(*myChatot, a1, volume, pan);
    } else {
        ret = Chatot_PlayCry(chatot, a1, volume, pan);
    }

    if (ret == 0) {
        Chatot_SetForceDefaultCry(1);
        PlayCryEx(0, SPECIES_CHATOT, pan, volume, 11, 0);
    }
}

BOOL Chatot_PlayCryWithDelayedFallback(SOUND_CHATOT *chatot, u32 a1, u32 volume, s32 pan, u8 delay) {
    SOUND_CHATOT **myChatot = GF_SdatGetAttrPtr(36);
    BOOL ret;
    if (chatot == 0) {
        ret = Chatot_PlayCry(*myChatot, a1, volume, pan);
    } else {
        ret = Chatot_PlayCry(chatot, a1, volume, pan);
    }

    if (ret == 0) {
        Chatot_SetForceDefaultCry(1);
        PlayCryDelayedEx(0, SPECIES_CHATOT, pan, volume, 11, delay, 0);
        ret = 1;
    }

    return ret;
}

u32 Chatot_GetVoiceParam(SOUND_CHATOT *chatot) {
    if (!Chatot_Exists(chatot)) {
        return 0;
    }

    s8 r0 = Chatot_GetData(chatot)[15];
    if (r0 < -30) {
        return 1;
    } else if (r0 >= 30 && r0 < 128) {
        return 2;
    }

    return 0;
}

BOOL Chatot_CryPatternUsesRecording(int pattern) {
    switch (pattern) {
    case 0:
    case 1:
    case 5:
    case 11:
    case 12:
        return TRUE;
    default:
        return FALSE;
    }
}
