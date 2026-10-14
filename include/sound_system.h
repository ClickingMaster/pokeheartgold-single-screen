#ifndef POKEHEARTGOLD_SOUND_SYSTEM_H
#define POKEHEARTGOLD_SOUND_SYSTEM_H

#include <nitro.h>
#include <nnsys.h>

typedef struct SoundWaveOutParam {
    NNSSndWaveOutHandle *handle;
    NNSSndWaveFormat format;
    void *data;
    u32 loopFlag;
    u32 loopStartSample;
    u32 samples;
    u32 sampleRate;
    s32 volume;
    u32 speed;
    s32 pan;
} SoundWaveOutParam;

void Sound_ClearSecondaryScene(void);
void GF_SndHandleMoveVolume(int, int, int);

void Sound_SetMasterVolume(int volume);
void sub_02005D00(void);
void GF_MicPauseOnLidClose(void);
void GF_MicResumeOnLidOpen(void);
BOOL SoundSys_GetGBSoundsState(void);
void SoundSys_ToggleGBSounds(void);
BOOL Sound_IsWaveOutPlaying(int channel);
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
void Sound_SetPlayerPause(u8 playerNo, u8 pause);
void GF_SND_BGM_DisableSet(u8 disabled);
BOOL GF_SND_BGM_DisableCheck(void);
void Sound_SetCurrentBGM(u16 seqNo);
void GF_SetCurrentPlayingBGM(u16 seqNo);
u16 GF_GetQueuedBGM(void);
void Sound_SetPlayingGBSeqNo(u16 seqNo);
void Sound_SetFieldBGM(u16 seqNo);
void Sound_SetSecondaryScene(u8 scene);
BOOL Sound_IsMicCounterFull(void);
BOOL Sound_SetSceneAndPlayBGM(u8 scene, u16 seqNo, int mode);
void GF_SetVolumeBySeqNo(u16 seqNo, u16 volume);
void GF_SndHandleSetPlayerVolume(u32 playerNo, u32 volume);
void GF_SndHandleSetInitialVolume(s32 handleNo, s32 volume);
u16 GF_GetVolumeBySeqNo();

void sub_02005448(); // One original caller passes the sequence through the existing r0 value.
void Sound_SetScene(int scene);
void Sound_SetSelectSEMuted(BOOL);

#endif // POKEHEARTGOLD_SOUND_SYSTEM_H
