#ifndef POKEHEARTGOLD_SOUND_SYSTEM_H
#define POKEHEARTGOLD_SOUND_SYSTEM_H

#include <nitro.h>
#include <nnsys.h>

typedef struct SoundWaveOutParam {
    NNSSndWaveOutHandle *unk00;
    NNSSndWaveFormat unk04;
    void *unk08;
    u32 unk0c;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    s32 unk1c;
    u32 unk20;
    s32 unk24;
} SoundWaveOutParam;

void Sound_ClearSecondaryScene(void);
void GF_SndHandleMoveVolume(int, int, int);

void Sound_SetMasterVolume(int a0);
void sub_02005D00(void);
void GF_MicPauseOnLidClose(void);
void GF_MicResumeOnLidOpen(void);
BOOL SoundSys_GetGBSoundsState(void);
void SoundSys_ToggleGBSounds(void);
BOOL Sound_IsWaveOutPlaying(int a0);
s8 *Sound_GetChatotBuffer(void);
BOOL Sound_AllocWaveOutChannel(int channel);
void Sound_StopWaveOutAndFreeBuffer(int);
void Sound_FreeWaveOutChannel(int);
NNSSndWaveOutHandle *Sound_GetWaveOutHandle(int);
BOOL Sound_StartWaveOut(SoundWaveOutParam *wavParam, int);
void Sound_SetWaveOutVolume(int, int);
BOOL GF_MIC_StartAutoSampling(MICAutoParam *param);
void GF_MIC_StopAutoSampling(void);
BOOL PlayCryEx(u32 pattern, u16 species, int pan, int volume, int heapId, u8 form);
BOOL Sound_LoadSceneSoundEffects(int scene);
void BGM_SaveStateAndPlayNew(u16 seqNo);
u16 GF_GetCurrentPlayingBGM(); // Cannot put `void` in it since it's sometimes called with an argument (required for matching), sometimes not.
void Sound_SetPlayerPause(u8 a0, u8 a1);
void GF_SND_BGM_DisableSet(u8 a0);
BOOL GF_SND_BGM_DisableCheck(void);
void Sound_SetCurrentBGM(u16 seqNo);
void GF_SetCurrentPlayingBGM(u16 seqNo);
u16 GF_GetQueuedBGM(void);
void Sound_SetPlayingGBSeqNo(u16 seqNo);
void Sound_SetFieldBGM(u16 seqNo);
void Sound_SetSecondaryScene(u8 scene);
BOOL Sound_IsMicCounterFull(void);
BOOL Sound_SetSceneAndPlayBGM(u8 scene, u16 seqNo, int mode);
void GF_SetVolumeBySeqNo(u16 a0, u16 a1);
void GF_SndHandleSetPlayerVolume(u32 a0, u32 a1);
void GF_SndHandleSetInitialVolume(s32 a0, s32 a1);
u16 GF_GetVolumeBySeqNo();

void sub_02005448(); // One original caller passes the sequence through the existing r0 value.
void Sound_SetScene(int a0);
void sub_02005BEC(BOOL);

#endif // POKEHEARTGOLD_SOUND_SYSTEM_H
