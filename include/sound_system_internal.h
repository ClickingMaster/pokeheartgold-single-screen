#ifndef POKEHEARTGOLD_SOUND_SYSTEM_INTERNAL_H
#define POKEHEARTGOLD_SOUND_SYSTEM_INTERNAL_H

#include "global.h"

#include "sound.h"

struct NNSSndArcBankInfo;

// Fields of the sound work reached through GF_SdatGetAttrPtr
enum SoundWorkAttr {
    SOUND_ATTR_WAVE_OUT_14_HANDLE,
    SOUND_ATTR_WAVE_OUT_15_HANDLE,
    SOUND_ATTR_QUEUED_BANK_INFO,
    SOUND_ATTR_BGM_DISABLED = 5,
    SOUND_ATTR_CRY_QUEUE,
    SOUND_ATTR_FADE_TIMER,
    SOUND_ATTR_AFTER_FADE_DELAY_TIMER,
    SOUND_ATTR_QUEUED_SEQ_FADE_IN,
    SOUND_ATTR_CURRENT_SEQ,
    SOUND_ATTR_QUEUED_SEQ,
    SOUND_ATTR_FIELD_PLAYER_PAUSED,
    SOUND_ATTR_BGM_PLAYER_PAUSED,
    SOUND_ATTR_AFTER_FANFARE_WAIT_TIMER,
    SOUND_ATTR_REVERSED_CRY_STARTED,
    SOUND_ATTR_WAVE_OUT_14_ALLOCATED,
    SOUND_ATTR_WAVE_OUT_15_ALLOCATED,
    SOUND_ATTR_USE_CHORUS,
    SOUND_ATTR_RELOAD_FIELD_SEQ,
    SOUND_ATTR_PRIMARY_SCENE = 21,
    SOUND_ATTR_SECONDARY_SCENE,
    SOUND_ATTR_SAVED_HEAP_LEVEL_0,
    SOUND_ATTR_SAVED_HEAP_LEVEL_1,
    SOUND_ATTR_SAVED_HEAP_LEVEL_2,
    SOUND_ATTR_SAVED_HEAP_LEVEL_3,
    SOUND_ATTR_SAVED_HEAP_LEVEL_4,
    SOUND_ATTR_SAVED_HEAP_LEVEL_5,
    SOUND_ATTR_SAVED_HEAP_LEVEL_6,
    SOUND_ATTR_CHATOT_PLAYING,
    SOUND_ATTR_CHATOT_FORCE_DEFAULT_CRY,
    SOUND_ATTR_FIELD_SEQ,
    SOUND_ATTR_REVERSED_WAVE_BUFFER = 34,
    SOUND_ATTR_CRY_STOP_TASK,
    SOUND_ATTR_MY_CHATOT,
    SOUND_ATTR_QUEUED_CRY_PATTERN_0 = 41,
    SOUND_ATTR_QUEUED_CRY_PAN_0,
    SOUND_ATTR_QUEUED_CRY_VOLUME_0,
    SOUND_ATTR_QUEUED_CRY_HEAP_ID_0,
    SOUND_ATTR_QUEUED_CRY_SPECIES_0,
    SOUND_ATTR_QUEUED_CRY_DELAY_0,
    SOUND_ATTR_QUEUED_CRY_PATTERN_1,
    SOUND_ATTR_QUEUED_CRY_PAN_1,
    SOUND_ATTR_QUEUED_CRY_VOLUME_1,
    SOUND_ATTR_QUEUED_CRY_HEAP_ID_1,
    SOUND_ATTR_QUEUED_CRY_SPECIES_1,
    SOUND_ATTR_QUEUED_CRY_DELAY_1,
    SOUND_ATTR_KEEP_PLAYING_CRIES,
    SOUND_ATTR_CHATOT_RECORDING_DISABLED,
    SOUND_ATTR_FOOTSTEP_VARIATION,
    SOUND_ATTR_SELECT_SE_MUTED,
    SOUND_ATTR_GB_SOUNDS_ENABLED,
    SOUND_ATTR_PLAYING_GB_SEQ,
};

typedef struct SoundSystemFlags {
    u8 micSampling;       // auto sampling started by GF_MIC_StartAutoSampling (restarted when the lid opens)
    BOOL fieldBGMStopped; // the field BGM was stopped once for a reload
    BOOL mono;
    BOOL fieldBGMStopOnce; // stop the field BGM only on the first reload
} SoundSystemFlags;

extern SoundSystemFlags gSoundSystemFlags;

void Sound_ClearBGMPauseFlags(void);
int GF_NNS_SndPlayerGetSeqNo(NNSSndHandle *);
int GF_GetBankBySeqNo(int);
void Sound_SetReloadFieldSeqFlag(u16);
int Sound_GetSavedHeapLevel(int);
void Sound_ReloadFieldSceneData(u16, int);
BOOL Sound_PlayFieldBGM(u16);
void Sound_SetBGMPlayerChannels(int);
void Sound_ApplyVoiceChatVolume(int, enum SoundHandleNo);
u8 GF_GetPlayerNoBySeq(u16);
const struct NNSSndArcBankInfo *GF_GetBankInfoBySeqNo(int);
BOOL Sound_PlayReversedWaveArc(int, int, int, int, int);
void Sound_StopWaveOut(int);
void Sound_SetWaveOutPan(int, u8);
void Sound_SetWaveOutSpeed(int, u32);
void GF_SndHandleSetTrackPitch(int, u32, int);
void GF_SndSetTrackPitchBySeqNo(u16, u32, int);
void GF_SndSetTrackPitchByPlayerNo(int, u32, int);
void GF_SndHandleSetTrackPan(int, u32, int);
void GF_SndHandleSetTempoRatio(int, int);
void GF_SndSetFadeTimer(u32);
BOOL GF_NowStartMusicId(u16, u16, int, u8, int);
BOOL GF_FadeStartMusicId(u16, u16, int, int, u8, int);

#endif // POKEHEARTGOLD_SOUND_SYSTEM_INTERNAL_H
