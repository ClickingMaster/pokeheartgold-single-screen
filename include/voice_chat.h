#ifndef POKEHEARTGOLD_VOICE_CHAT_H
#define POKEHEARTGOLD_VOICE_CHAT_H

#include "global.h"

#include "nnsys/snd_internal.h"
#include "vct/vct_session.h"

#define VOICE_CHAT_MODE_PHONE      1
#define VOICE_CHAT_MODE_CONFERENCE 3

#define VOICE_CHAT_CALL_IDLE       0
#define VOICE_CHAT_CALL_REQUESTING 1
#define VOICE_CHAT_CALL_TALKING    2

// 32-byte aligned inside a Heap_Alloc'd block (rawAlloc).
typedef struct VoiceChatWork {
    u8 micBuffer[0x880]; // auto-sampling ring buffer
    void *rawAlloc;
    void *vctBuffer; // aligned
    void *vctBufferRaw;
    u8 streamBuffer[0x880];
    u8 silenceBuffer[0x880]; // sent instead of the mic data while the lid is closed
    void (*endCallback)(void);
    VctSession sessions[3];
    int conferenceClients[4];
    u32 mode;
    u32 callState;
    BOOL muted;
    u32 heapId;
    SndStrm stream;
    VctSession *session;
    u8 samplingState;
    u8 restartSampling;
    u16 requestTimer;
    MICAutoParam micParam;
    u32 unk_1A78;
    u32 ampGain;
    u32 ampGainChanged;
    u32 unk_1A84;
    u32 unk_1A88;
    u16 unk_1A8C[16];
    u32 unk_1AAC;
} VoiceChatWork;

typedef struct VoiceChat {
    s32 frameBudget; // in 1/64000 s units of VCT_Main calls still owed
    VoiceChatWork *work;
    u64 lastTick;
} VoiceChat;

extern VoiceChat gVoiceChat;

void VoiceChat_Update(void);
BOOL VoiceChat_ReceiveData(u32 aid, void *data, u32 size);
void VoiceChat_Init(int heapId, int codec, int count);
void VoiceChat_HangUp(int unused);
void VoiceChat_SetEndCallback(void (*callback)(void));
void VoiceChat_Shutdown(void);
int VoiceChat_GetVoiceActivity(void);
void VoiceChat_Mute(void);
void VoiceChat_Unmute(void);
int VoiceChat_AddConferenceClients(u32 aidMask, int skipAid);

#endif // POKEHEARTGOLD_VOICE_CHAT_H
