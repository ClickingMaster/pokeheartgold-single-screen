typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct {
    u32 capture;
    u32 alarms;
    u32 channels;
} ResourceManager;
extern ResourceManager _021DF820;
extern u8 _021DF82C[];
extern u8 _021DFDE4[];
extern u8 _021DFDDC[];
extern u8 _021DFDE0[];
extern u8 _021DFDCC[];
extern u8 _021DF984[];
extern u8 _021DFDC4[];
extern u8 _021DD420[];
extern u8 _021DD460[];

extern s32 SND_RecvCommandReply(s32);
extern void NNSi_SndPlayerMain(void);
extern void NNSi_SndCaptureMain(void);
extern void NNSi_SndArcStrmMain(void);
extern void SND_FlushCommand(s32);
extern void SND_SetMasterVolume(s32);
extern void SND_SetMasterPan(s32);
extern void SND_ResetMasterPan(void);
extern void NNSi_SndCaptureBeginSleep(void);
extern void NNSi_SndCaptureEndSleep(void);
extern void SND_StopTimer(u32, u32, u32, u32);
extern void SND_StartTimer(u32, u32, u32, u32);
extern u32 SND_GetCurrentCommandTag(void);
extern void SND_WaitForCommandProc(u32);
extern void SND_LockChannel(u32, u32);
extern void SND_UnlockChannel(u32, u32);
extern void SND_SetChannelVolume(u32, u32, u32);
extern void SND_SetChannelPan(u32, u32);
extern void SND_SetChannelTimer(u32, u32);
extern s32 SND_IsFinishedCommandTag(u32);
extern u32 SND_GetChannelStatus(void);
extern void NNSi_SndPlayerStopSeq(void *, s32);
extern void NNSi_SndPlayerPause(void *, s32);
extern void NNSi_SndFaderSet(void *, s32, s32);
extern void SND_SetPlayerChannelPriority(u32, u32);
extern void SND_SetTrackPitch(u32, u32, u32);
extern void *NNS_FndGetNextListObject(void *, void *);

void NNS_SndMain(void);
void NNS_SndSetMasterVolume(s32 vol);
void NNS_SndSetMonoFlag(s32 flag);
void BeginSleep(void);
void EndSleep(void);
s32 NNS_SndLockChannel(u32 bits);
void NNS_SndUnlockChannel(u32 bits);
void NNS_SndUnlockCapture(u32 bits);
s32 NNS_SndAllocAlarm(void);
void NNS_SndFreeAlarm(u32 i);
void NNSi_SndInitResourceMgr(void);
void *NNS_SndWaveOutAllocChannel(u32 channel);
void NNS_SndWaveOutFreeChannel(u32 *wave);
void NNS_SndWaveOutStop(u32 *wave);
void NNS_SndWaveOutSetVolume(u32 *wave, u32 vol);
void NNS_SndWaveOutSetPan(u32 *wave, u32 pan);
s32 NNS_SndWaveOutIsPlaying(u32 *wave);
void NNS_SndPlayerSetPlayerVolume(u32 player, u32 vol);
void NNS_SndPlayerSetPlayableSeqCount(u32 player, u32 count);
void NNS_SndPlayerSetAllocatableChannel(u32 player, u32 channel);
void NNS_SndPlayerStopSeq(u32 *handle, s32 fade);
void NNS_SndPlayerStopSeqByPlayerNo(u32 player, s32 fade);
void NNS_SndPlayerStopSeqBySeqNo(u32 seq, s32 fade);
void NNS_SndPlayerStopSeqAll(s32 fade);
void NNS_SndPlayerPause(u32 *handle, s32 flag);
void NNS_SndPlayerPauseByPlayerNo(u32 player, s32 flag);
void NNS_SndHandleInit(u32 *handle);
void NNS_SndHandleReleaseSeq(u32 volatile *handle);
u16 NNS_SndPlayerCountPlayingSeqByPlayerNo(u32 player);
void NNS_SndPlayerSetVolume(u32 volatile *handle, u32 vol);
void NNS_SndPlayerSetInitialVolume(u32 volatile *handle, u32 vol);
void NNS_SndPlayerMoveVolume(u32 volatile *handle, u32 volume, s32 frames);
void NNS_SndPlayerSetChannelPriority(u32 volatile *handle, u32 priority);
void NNS_SndPlayerSetTrackPitch(u32 volatile *handle, u32 track, u32 pitch);

void NNS_SndMain(void) {
    while (SND_RecvCommandReply(0)) {}
    NNSi_SndPlayerMain();
    NNSi_SndCaptureMain();
    NNSi_SndArcStrmMain();
    SND_FlushCommand(0);
}

void NNS_SndSetMasterVolume(s32 vol) {
    SND_SetMasterVolume(vol);
}

void NNS_SndSetMonoFlag(s32 flag) {
    if (flag) {
        SND_SetMasterPan(0x40);
    } else {
        SND_ResetMasterPan();
    }
}
