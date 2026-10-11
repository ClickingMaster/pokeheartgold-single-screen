#ifndef POKEHEARTGOLD_VOICE_CHAT_H
#define POKEHEARTGOLD_VOICE_CHAT_H

#include "global.h"

#include "vct/vct_session.h"

#define VOICE_CHAT_MODE_PHONE      1
#define VOICE_CHAT_MODE_CONFERENCE 3

#define VOICE_CHAT_CALL_IDLE       0
#define VOICE_CHAT_CALL_REQUESTING 1
#define VOICE_CHAT_CALL_TALKING    2

// 0x1AB0 bytes, 32-byte aligned inside a Heap_Alloc'd block (rawAlloc).
typedef struct VoiceChatWork {
    u8 micBuffer[0x880];       // 0x0000, auto-sampling ring buffer
    void *rawAlloc;            // 0x0880
    void *vctBuffer;           // 0x0884, aligned
    void *vctBufferRaw;        // 0x0888
    u8 streamBuffer[0x880];    // 0x088C
    u8 silenceBuffer[0x880];   // 0x110C, sent instead of the mic data while the lid is closed
    void (*endCallback)(void); // 0x198C
    Session sessions[3];       // 0x1990
    int conferenceClients[4];  // 0x19D8
    u32 mode;                  // 0x19E8
    u32 callState;             // 0x19EC
    BOOL muted;                // 0x19F0
    u32 heapId;                // 0x19F4
    u8 stream[0x5C];           // 0x19F8, NNS sound stream
    Session *session;          // 0x1A54
    u8 samplingState;          // 0x1A58
    u8 restartSampling;        // 0x1A59
    u16 requestTimer;          // 0x1A5A
    MICAutoParam micParam;     // 0x1A5C
    u32 unk_1A78;              // 0x1A78
    u32 ampGain;               // 0x1A7C
    u32 ampGainChanged;        // 0x1A80
    u32 unk_1A84;              // 0x1A84
    u32 unk_1A88;              // 0x1A88
    u16 unk_1A8C[16];          // 0x1A8C
    u32 unk_1AAC;              // 0x1AAC
} VoiceChatWork;

typedef struct VoiceChat {
    s32 frameBudget; // in 1/64000 s units of VCT_Main calls still owed
    VoiceChatWork *work;
    u64 lastTick;
} VoiceChat;

extern VoiceChat gVoiceChat;
extern VoiceChatWork *gVoiceChatWork; // same storage as gVoiceChat.work

void VoiceChat_InitAudio(void);
void VoiceChat_MicCallback(MICResult result, void *arg);
void VoiceChat_StreamCallback(int enabled, u32 count, void **buffers, u32 size, int unused, u8 *address);
BOOL VoiceChat_RequestSession(u32 aid);
BOOL VoiceChat_AcceptSession(int unused);
void VoiceChat_ConferenceEventCallback(u32 unusedId, u32 event, Session *session, void *unusedContext);
void VoiceChat_PhoneEventCallback(u32 unusedId, u32 event, Session *session, void *unusedContext);
void VoiceChat_EndSession(Session *session);
void VoiceChat_Update(void);
BOOL VoiceChat_ReceiveData(u32 aid, void *data, void *size);
void VoiceChat_AllocAligned(void **allocation, void **aligned, u32 size, u32 heapId);
void VoiceChat_Init(int heapId, int codec, int count);
void VoiceChat_StartStream(void);
void VoiceChat_HangUp(int unused);
void VoiceChat_SetEndCallback(void (*callback)(void));
void VoiceChat_Shutdown(void);
int VoiceChat_GetVoiceActivity(void);
void VoiceChat_Mute(void);
void VoiceChat_Unmute(void);
int VoiceChat_AddConferenceClients(u32 aidMask, int skipAid);

#endif // POKEHEARTGOLD_VOICE_CHAT_H
