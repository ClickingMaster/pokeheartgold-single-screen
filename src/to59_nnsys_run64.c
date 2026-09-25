typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

void SND_SetTrackPan(u8 player, u32 trackMask, int pan);
void SND_SetTrackAllocatableChannel(u8 player, u32 trackMask, u32 channels);
void SND_SetPlayerTempoRatio(u8 player, u32 ratio);
int SND_GetPlayerLocalVariable(u8 player, int index);
int NNSi_SndReadDriverTrackInfo(u8 player, int track, void *out);
void NNS_SndPlayerSetTrackPan(u8 *volatile *handle, u32 trackMask, int pan);
void NNS_SndPlayerSetTrackAllocatableChannel(u8 *volatile *handle, u32 trackMask, u32 channels);
void NNS_SndPlayerSetTempoRatio(u8 *volatile *handle, u32 ratio);
void NNS_SndPlayerSetSeqNo(u8 *volatile *handle, u16 seqNo);
void NNS_SndPlayerSetSeqArcNo(u8 *volatile *handle, u16 arcNo, u16 seqNo);
int NNS_SndPlayerGetSeqNo(u8 *volatile *handle);
int NNS_SndPlayerReadVariable(u8 *volatile *handle, int index, u16 *value);
int NNS_SndPlayerReadDriverTrackInfo(u8 *volatile *handle, int track, void *out);
void ShutdownPlayer(void *player);
void NNSi_SndPlayerFreeSeqPlayer(void *player);
void ForceStopStrm(u8 *stream);
void NNS_SndStrmStop(u8 *stream);
void StrmCallback(void *stream, int alarm);
void AlarmCallback(void *stream);
void SND_PauseSeq(u32 player, u32 paused);
void NNSi_SndPlayerPause(u8 *player, int paused);
void ForceStopSeq(u8 *player);
void NNSi_SndFaderSet(void *fader, int value);
void SetPlayerPriority(u8 *player, int priority);
void NNSi_SndPlayerStopSeq(u8 *player, int fade);

void NNS_SndUnlockChannel(u32 mask);
void NNS_SndStrmFreeChannel(u8 *stream);
extern u8 _021E0314[];
int NNS_SndCaptureIsActive(void);
int NNS_SndCaptureGetCaptureType(void);
void NNSi_SndCaptureInit(void);

typedef struct StreamFlags {
    u8 pad[0x24];
    signed active : 1;
} StreamFlags;

void NNSi_SndPlayerFreeSeqPlayer(void *player) {
    ShutdownPlayer(player);
}
