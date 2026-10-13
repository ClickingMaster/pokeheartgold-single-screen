#include "sound_system.h"

#include "global.h"

#include "heap.h"
#include "sound.h"

// Sound state on top of the SND_WORK attributes (GF_SdatGetAttrPtr): the current BGM and the sound scenes,
// per-sequence volume, microphone auto sampling, the two wave-out channels (14 and 15) that play reversed wave
// archives, capture and track controls, and BGM changes with fade out, delay and fade in.

struct NNSSndArcBankInfo;

typedef struct WaveArcInfo {
    u32 fileId : 24;
    u32 flags : 8;
} WaveArcInfo;

// asm/sound_system_data.s
extern int _021D05E8[4];
extern MICAutoParam gSoundMicAutoParam;
extern s8 _021D0620[2000];

// sound_gb.c, sound_playback.c, communication
u16 GBSounds_GetDSSeqNoByGBSeqNo(u16 seqNo);
void StopBGMSEAndCry(void);
void Sound_Stop(void);
BOOL sub_020378CC(void);

// NitroSystem / MIC, with the argument types these call sites were compiled against
void NNS_SndPlayerPause(NNSSndHandle *handle, BOOL pause);
void NNS_SndPlayerMoveVolume(NNSSndHandle *handle, int volume, int frames);
void NNS_SndPlayerSetInitialVolume(NNSSndHandle *handle, int volume);
int NNS_SndPlayerGetSeqNo(NNSSndHandle *handle);
const struct NNSSndArcBankInfo *NNS_SndArcGetBankInfo(int bankNo);
MICResult MIC_StartAutoSampling(const MICAutoParam *param);
MICResult MIC_StopAutoSampling(void);
const WaveArcInfo *NNS_SndArcGetWaveArcInfo(int waveArcNo);
u32 NNS_SndArcGetFileSize(u32 fileId);
s32 NNS_SndArcReadFile(u32 fileId, void *buffer, u32 size, s32 offset);
NNSSndWaveOutHandle NNS_SndWaveOutAllocChannel(int channel);
void NNS_SndWaveOutFreeChannel(NNSSndWaveOutHandle handle);
BOOL NNS_SndWaveOutStart(NNSSndWaveOutHandle handle, NNSSndWaveFormat format, const void *data, u32 loop, u32 loopStart, u32 length, u32 sampleRate, s32 volume, u32 speed, s32 pan);
void NNS_SndWaveOutStop(NNSSndWaveOutHandle handle);
BOOL NNS_SndWaveOutIsPlaying(NNSSndWaveOutHandle handle);
void NNS_SndWaveOutSetPan(NNSSndWaveOutHandle handle, u32 pan);
void NNS_SndWaveOutSetSpeed(NNSSndWaveOutHandle handle, u32 speed);
void NNS_SndWaveOutSetVolume(NNSSndWaveOutHandle handle, int volume);
BOOL NNS_SndCaptureIsActive(void);
int NNS_SndCaptureGetCaptureType(void);
void NNS_SndCaptureStopReverb(int frames);
void NNS_SndPlayerSetTrackPitch(NNSSndHandle *handle, u32 tracks, int pitch);
void NNS_SndPlayerSetTrackPan(NNSSndHandle *handle, u32 tracks, int pan);
void NNS_SndPlayerSetTempoRatio(NNSSndHandle *handle, int ratio);
void NNS_SndSetMonoFlag(int flag);
void NNS_SndSetMasterVolume(int volume);
void NNS_SndPlayerSetAllocatableChannel(int playerNo, u32 channels);

void Sound_UnloadSecondarySceneData(void);
void Sound_PauseOrStopFieldBGM(void);
void Sound_ClearBGMPauseFlags(void);
int GF_NNS_SndPlayerGetSeqNo(NNSSndHandle *handle);
int GF_GetBankBySeqNo(int seqNo);
void Sound_SetReloadFieldSeqFlag(u16 seqNo);
int Sound_GetSavedHeapLevel(int state);
void Sound_ResumeFieldBGM(u16 seqNo, u16 previousSeqNo);
void Sound_ReloadFieldSceneData(u16 seqNo, int bankNo);
BOOL Sound_PlayFieldBGM(u16 seqNo);
void Sound_ResetAndLoadSceneSoundEffects(int scene);
void Sound_SetBGMPlayerChannels(int mode);
void Sound_PlayBGMForFieldScene(u16 seqNo, int mode);
void Sound_PlayBGMForBattleScene(u16 seqNo, int mode);
void Sound_PlayBGMForWifiScene(u16 seqNo, int mode);
void sub_02005280(u16 seqNo, int mode);
void sub_020052A4(u16 seqNo, int mode);
void Sound_LoadSecondarySceneSoundEffects(int scene);
void Sound_PlayBGMForScene(int scene, u16 seqNo, int mode);
void sub_02005464(int seqNo, enum SoundHandleNo handleNo);
u8 GF_GetPlayerNoBySeq(u16 seqNo);
const struct NNSSndArcBankInfo *GF_GetBankInfoBySeqNo(int seqNo);
BOOL Sound_PlayReversedWaveArc(int waveArcNo, int volume, int pan, int channel, int heapId);
void Sound_ReverseWaveBuffer(void *buffer, u32 size);
void Sound_StopWaveOut(int channel);
void Sound_SetWaveOutPan(int channel, u32 pan);
void Sound_SetWaveOutSpeed(int channel, u32 speed);
BOOL Sound_IsCaptureActive(void);
int Sound_GetCaptureType(void);
void Sound_StopCaptureReverb(int frames);
void GF_SndHandleSetTrackPitch(int handleNo, u32 tracks, int pitch);
void GF_SndSetTrackPitchBySeqNo(u16 seqNo, u32 tracks, int pitch);
void GF_SndSetTrackPitchByPlayerNo(int playerNo, u32 tracks, int pitch);
void GF_SndHandleSetTrackPan(int handleNo, u32 tracks, int pan);
void GF_SndHandleSetTempoRatio(int handleNo, int ratio);
void GF_SndSetFadeTimer(u32 frames);
void GF_SndSetAfterFadeDelayTimer(u32 frames);
BOOL GF_NowStartMusicId(u16 seqNo, u16 fadeOutFrames, int delay, u8 reload, int unused);
BOOL GF_NowStartMusicIdEx(int scene, u16 seqNo, u16 fadeOutFrames, int delay, u8 reload, int unused);
BOOL GF_FadeStartMusicId(u16 seqNo, u16 fadeOutFrames, int delay, int fadeInFrames, u8 reload, int unused);
BOOL GF_FadeStartMusicIdEx(int scene, u16 seqNo, u16 fadeOutFrames, int delay, int fadeInFrames, u8 reload, int unused);
void Sound_PrepareBGMChange(int scene, u16 seqNo, u16 fadeOutFrames, int delay, u8 reload, int unused);
void GF_SndSetAllocatableChannelForBGMPlayer(u32 channels);

void GF_SND_BGM_DisableSet(u8 disabled) {
    u8 *value = GF_SdatGetAttrPtr(5);
    *value = disabled;
}

BOOL GF_SND_BGM_DisableCheck(void) {
    u8 *value = GF_SdatGetAttrPtr(5);
    return *value;
}

void Sound_SetCurrentBGM(u16 seqNo) {
    u16 *currentSeqNo = GF_SdatGetAttrPtr(10);
    if (seqNo > SEQ_GS_P_START) {
        Sound_SetPlayingGBSeqNo(seqNo);
        *currentSeqNo = GBSounds_GetDSSeqNoByGBSeqNo(seqNo);
    } else {
        *currentSeqNo = seqNo;
    }
    GF_SetCurrentPlayingBGM(0);
}

u16 GF_GetCurrentPlayingBGM(void) {
    u16 *seqNo = GF_SdatGetAttrPtr(10);
    return *seqNo;
}

void GF_SetCurrentPlayingBGM(u16 seqNo) {
    u16 *queuedSeqNo = GF_SdatGetAttrPtr(11);
    *queuedSeqNo = seqNo;
}

u16 GF_GetQueuedBGM(void) {
    u16 *queuedSeqNo = GF_SdatGetAttrPtr(11);
    return *queuedSeqNo;
}

void Sound_SetPlayingGBSeqNo(u16 seqNo) {
    u16 *value = GF_SdatGetAttrPtr(58);
    *value = seqNo;
}

void Sound_SetFieldBGM(u16 seqNo) {
    u16 *fieldSeqNo = GF_SdatGetAttrPtr(32);
    *fieldSeqNo = seqNo;
}

void Sound_SetScene(int scene) {
    u8 *primaryScene = GF_SdatGetAttrPtr(21);
    u8 *secondaryScene = GF_SdatGetAttrPtr(22);
    if ((u32)scene < 51) {
        *primaryScene = scene;
        *secondaryScene = 0;
    } else {
        *secondaryScene = scene;
    }
}

void Sound_SetSecondaryScene(u8 scene) {
    GF_SdatGetAttrPtr(21);
    u8 *secondaryScene = GF_SdatGetAttrPtr(22);
    *secondaryScene = scene;
}

void Sound_ClearSecondaryScene(void) {
    u8 *secondaryScene = GF_SdatGetAttrPtr(22);
    Sound_UnloadSecondarySceneData();
    *secondaryScene = 0;
}

BOOL Sound_LoadSceneSoundEffects(int scene) {
    BOOL success;
    switch (scene) {
    case 1:
    case 9:
    case 10:
    case 17:
    case 20:
    case 23:
        success = GF_Snd_LoadGroup(GROUP_SE_FIELD);
        break;
    case 19:
        success = GF_Snd_LoadGroup(GROUP_SE_FIELD);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_BALLOON02, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_BALLOON03_2, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_BALLOON05, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_BALLOON01, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_BALLOON07, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_ALERT4, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_DP_FW104, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_NOMI02, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_DP_023, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_POINT1, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_POINT2, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_POINT3, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_BALLOON05_2, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_DP_HAMARU, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_DP_CON_016, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_KIRAKIRA, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_FCALL, 1);
        break;
    case 14:
        success = GF_Snd_LoadGroup(GROUP_SE_NUTMIXER);
        break;
    case 2:
    case 13:
        success = GF_Snd_LoadGroup(GROUP_SE_BATTLE);
        break;
    case 21:
        GF_Snd_LoadBank(BANK_SE_HIROBA);
        success = GF_Snd_LoadWaveArc(WAVE_ARC_SE_HIROBA);
        break;
    case 3:
        success = GF_Snd_LoadGroup(GROUP_SE_TRADE);
        break;
    case 4:
    case 22:
        success = GF_Snd_LoadGroup(GROUP_SE_FIELD);
        break;
    case 5:
        success = GF_Snd_LoadGroup(GROUP_SE_BATTLE);
        break;
    case 11:
        success = GF_Snd_LoadGroup(GROUP_SE_FIELD);
        break;
    case 6:
        success = GF_Snd_LoadGroup(GROUP_SE_CONTEST);
        break;
    case 8:
        success = GF_Snd_LoadGroup(GROUP_SE_FIELD);
        break;
    case 12:
        success = GF_Snd_LoadGroup(GROUP_SE_NUTMIXER);
        break;
    case 16:
        GF_Snd_LoadGroup(GROUP_SE_FIELD);
        success = GF_Snd_LoadGroup(GROUP_SE_DIG);
        break;
    case 15:
        success = GF_Snd_LoadGroup(GROUP_SE_FIELD);
        break;
    case 24:
        GF_Snd_LoadBank(BANK_SE_THLON);
        success = GF_Snd_LoadWaveArc(WAVE_ARC_SE_THLON);
        break;
    case 25:
        GF_Snd_LoadBank(BANK_SE_THLON_OPED);
        success = GF_Snd_LoadWaveArc(WAVE_ARC_SE_THLON_OPED);
        break;
    case 51:
        success = GF_Snd_LoadGroup(GROUP_SE_BAG);
        break;
    case 64:
        success = GF_Snd_LoadGroup(GROUP_SE_SLOT);
        break;
    case 52:
    case 67:
        success = GF_Snd_LoadGroup(GROUP_SE_NAMEIN);
        break;
    case 7:
    case 53:
        success = GF_Snd_LoadGroup(GROUP_SE_IMAGE);
        break;
    case 54:
        success = GF_Snd_LoadGroup(GROUP_SE_ZUKAN);
        break;
    case 55:
    case 65:
        GF_Snd_LoadBank(BANK_SE_TOWNMAP);
        success = GF_Snd_LoadWaveArc(WAVE_ARC_SE_TOWNMAP);
        break;
    case 56:
        success = GF_Snd_LoadGroup(GROUP_SE_TRCARD);
        break;
    case 57:
        success = GF_Snd_LoadGroup(GROUP_SE_POKELIST);
        break;
    case 58:
        success = GF_Snd_LoadGroup(GROUP_SE_DIG);
        break;
    case 59:
        success = GF_Snd_LoadGroup(GROUP_SE_CUSTOM);
        break;
    case 60:
        success = GF_Snd_LoadGroup(GROUP_SE_BAG);
        break;
    case 61:
        success = GF_Snd_LoadGroup(GROUP_SE_NAMEIN);
        break;
    case 62:
        success = GF_Snd_LoadGroup(GROUP_SE_CUSTOM);
        break;
    case 63:
        success = GF_Snd_LoadGroup(GROUP_SE_CLIMAX);
        break;
    case 66:
        GF_Snd_LoadBank(BANK_SE_SCRATCH);
        success = GF_Snd_LoadWaveArc(WAVE_ARC_SE_SCRATCH);
        break;
    case 69:
        GF_Snd_LoadBank(BANK_SE_PLANTER);
        success = GF_Snd_LoadWaveArc(WAVE_ARC_SE_PLANTER);
        break;
    case 68:
        GF_Snd_LoadBank(BANK_SE_LINEAR);
        success = GF_Snd_LoadWaveArc(WAVE_ARC_SE_LINEAR);
        break;
    case 70:
        GF_Snd_LoadBank(BANK_SE_COIN);
        success = GF_Snd_LoadWaveArc(WAVE_ARC_SE_COIN);
        break;
    case 71:
        GF_Snd_LoadBank(BANK_SE_DENDO);
        success = GF_Snd_LoadWaveArc(WAVE_ARC_SE_DENDO);
        break;
    case 72:
        GF_Snd_LoadBank(BANK_SE_JUICE);
        success = GF_Snd_LoadWaveArc(WAVE_ARC_SE_JUICE);
        break;
    case 73:
        GF_Snd_LoadBank(BANK_SE_PHC);
        success = GF_Snd_LoadWaveArc(WAVE_ARC_SE_PHC);
        break;
    case 74:
        GF_Snd_LoadBank(BANK_SE_SEKIBAN);
        success = GF_Snd_LoadWaveArc(WAVE_ARC_SE_SEKIBAN);
        break;
    case 75:
        GF_Snd_LoadBank(BANK_SE_EVENT);
        success = GF_Snd_LoadWaveArc(WAVE_ARC_SE_EVENT);
        break;
    default:
        GF_ASSERT(FALSE);
        success = FALSE;
        break;
    }
    return success;
}

BOOL Sound_PlayFieldBGM(u16 seqNo) {
    return Sound_SetSceneAndPlayBGM(4, seqNo, 1);
}

BOOL Sound_SetSceneAndPlayBGM(u8 scene, u16 seqNo, int mode) {
    u8 *primary = GF_SdatGetAttrPtr(21);
    u8 *secondary = GF_SdatGetAttrPtr(22);
    u16 *fanfareWait = GF_SdatGetAttrPtr(14);
    if (scene < 51) {
        if (*primary == scene) {
            return FALSE;
        }
    } else {
        if (*secondary == scene) {
            return FALSE;
        }
    }
    Sound_SetScene(scene);
    switch (scene) {
    case 4:
        Sound_SetBGMPlayerChannels(0);
        Sound_PlayBGMForFieldScene(seqNo, mode);
        *fanfareWait = 0;
        break;
    case 5:
        Sound_PlayBGMForBattleScene(seqNo, mode);
        break;
    case 11:
        Sound_PlayBGMForWifiScene(seqNo, mode);
        break;
    case 6:
        sub_02005280(seqNo, mode);
        break;
    case 7:
        sub_020052A4(seqNo, mode);
        break;
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
    case 59:
    case 60:
    case 61:
    case 62:
    case 63:
    case 64:
    case 65:
    case 66:
    case 67:
    case 69:
    case 70:
    case 71:
    case 72:
    case 74:
        Sound_LoadSecondarySceneSoundEffects(scene);
        break;
    case 68:
        Sound_LoadSecondarySceneSoundEffects(scene);
        PlayBGM(seqNo);
        break;
    case 1:
        Sound_SetBGMPlayerChannels(1);
        Sound_PlayBGMForScene(scene, seqNo, mode);
        break;
    case 14:
        Sound_SetBGMPlayerChannels(2);
        Sound_PlayBGMForScene(scene, seqNo, mode);
        break;
    case 2:
        Sound_SetBGMPlayerChannels(0);
        Sound_PlayBGMForScene(scene, seqNo, mode);
        break;
    case 3:
    case 8:
    case 9:
    case 10:
    case 12:
    case 13:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 23:
    case 24:
    case 25:
    case 73:
        Sound_PlayBGMForScene(scene, seqNo, mode);
        break;
    case 22:
        Sound_PlayBGMForScene(scene, seqNo, mode);
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

void Sound_ResetAndLoadSceneSoundEffects(int scene) {
    GF_Snd_LoadState(*(int *)GF_SdatGetAttrPtr(24));
    GF_Snd_SaveState(GF_SdatGetAttrPtr(25));
    Sound_LoadSceneSoundEffects(scene);
    GF_Snd_SaveState(GF_SdatGetAttrPtr(26));
}

void Sound_PlayBGMForFieldScene(u16 seqNo, int mode) {
    int playingSeqNo;
    u8 *reload = GF_SdatGetAttrPtr(12);
    GF_SdatGetAttrPtr(24);
    GF_SdatGetAttrPtr(32);
    playingSeqNo = GF_NNS_SndPlayerGetSeqNo(GF_GetSoundHandle(0));
    u16 currentSeqNo = playingSeqNo;
    if (playingSeqNo > SEQ_GS_P_START) {
        currentSeqNo = GBSounds_GetDSSeqNoByGBSeqNo(currentSeqNo);
    }
    if (*reload != 0 || currentSeqNo != seqNo || GF_GetQueuedBGM() == SEQ_GS_BICYCLE) {
        Sound_SetReloadFieldSeqFlag(1);
        if (_021D05E8[3] == 0 || _021D05E8[1] == 0) {
            StopBGMSEAndCry();
            _021D05E8[1] = 1;
        }
        if (currentSeqNo != seqNo) {
            Sound_SetPlayerPause(1, 0);
            Sound_Stop();
        }
        if (*reload == 1) {
            GF_Snd_LoadState(Sound_GetSavedHeapLevel(2));
            Sound_LoadSceneSoundEffects(4);
            GF_Snd_SaveState(GF_SdatGetAttrPtr(26));
            if (currentSeqNo != seqNo) {
                Sound_SetPlayerPause(1, 0);
            }
            Sound_ResumeFieldBGM(seqNo, (u16)playingSeqNo);
        } else {
            PlayBGM(seqNo);
        }
    }
}

void Sound_ResumeFieldBGM(u16 seqNo, u16 previousSeqNo) {
    u16 *fieldSeqNo = GF_SdatGetAttrPtr(32);
    if (GF_GetBankBySeqNo(*fieldSeqNo) == BANK_BASIC) {
        GF_Snd_LoadSeqEx(seqNo, 4);
        GF_ASSERT(FALSE);
    } else {
        GF_Snd_LoadSeqEx(*fieldSeqNo, 6);
    }
    GF_Snd_SaveState(GF_SdatGetAttrPtr(27));
    Sound_SetPlayerPause(1, 0);
    GF_SndStartFadeInBGM(127, 40, 0);
    Sound_SetReloadFieldSeqFlag(0);
}

void Sound_ReloadFieldSceneData(u16 seqNo, int bankNo) {
    u8 *reload = GF_SdatGetAttrPtr(19);
    u16 *fieldSeqNo = GF_SdatGetAttrPtr(32);
    if (*reload == 1 || bankNo == 0) {
        GF_Snd_LoadState(Sound_GetSavedHeapLevel(1));
        Sound_SetSecondaryScene(0);
        GF_Snd_LoadSeqEx(*fieldSeqNo, 2);
        GF_Snd_SaveState(GF_SdatGetAttrPtr(25));
        Sound_LoadSceneSoundEffects(4);
        GF_Snd_SaveState(GF_SdatGetAttrPtr(26));
        if (GF_GetBankBySeqNo(*fieldSeqNo) == BANK_BASIC) {
            GF_Snd_LoadSeqEx(seqNo, 4);
            GF_ASSERT(FALSE);
        } else {
            GF_Snd_LoadSeqEx(*fieldSeqNo, 4);
        }
        GF_Snd_SaveState(GF_SdatGetAttrPtr(27));
    }
}

void Sound_PlayBGMForBattleScene(u16 seqNo, int mode) {
    GF_SdatGetAttrPtr(24);
    Sound_PauseOrStopFieldBGM();
    GF_Snd_LoadState(Sound_GetSavedHeapLevel(2));
    Sound_LoadSceneSoundEffects(5);
    GF_Snd_SaveState(GF_SdatGetAttrPtr(26));
    Sound_SetReloadFieldSeqFlag(1);
    PlayBGM(seqNo);
}

void Sound_PlayBGMForWifiScene(u16 seqNo, int mode) {
    GF_SdatGetAttrPtr(24);
    Sound_Stop();
    Sound_ClearBGMPauseFlags();
    Sound_ResetAndLoadSceneSoundEffects(4);
    PlayBGM(seqNo);
}

void sub_02005280(u16 seqNo, int mode) {
    GF_SdatGetAttrPtr(24);
    Sound_Stop();
    Sound_ResetAndLoadSceneSoundEffects(6);
    Sound_SetReloadFieldSeqFlag(1);
    PlayBGM(seqNo);
}

void sub_020052A4(u16 seqNo, int mode) {
    GF_SdatGetAttrPtr(24);
    Sound_Stop();
    Sound_ResetAndLoadSceneSoundEffects(7);
    Sound_SetReloadFieldSeqFlag(1);
    PlayBGM(seqNo);
}

void Sound_LoadSecondarySceneSoundEffects(int scene) {
    Sound_UnloadSecondarySceneData();
    Sound_LoadSceneSoundEffects(scene);
    GF_Snd_SaveState(GF_SdatGetAttrPtr(28));
}

void Sound_PlayBGMForScene(int scene, u16 seqNo, int mode) {
    GF_SdatGetAttrPtr(24);
    Sound_Stop();
    Sound_ResetAndLoadSceneSoundEffects(scene);
    PlayBGM(seqNo);
}

void BGM_SaveStateAndPlayNew(u16 seqNo) {
    GetSoundDataPointer();
    Sound_PauseOrStopFieldBGM();
    PlayBGM(seqNo);
}

void Sound_UnloadSecondarySceneData(void) {
    GF_Snd_LoadState(Sound_GetSavedHeapLevel(4));
}

int Sound_GetSavedHeapLevel(int state) {
    int *level;
    GetSoundDataPointer();
    if (state >= 7) {
        GF_ASSERT(FALSE);
        return *(int *)GF_SdatGetAttrPtr(27);
    }
    // Valid state IDs are 0 through 6; preserve the original out-of-range behavior.
    switch (state) {
    case 0:
        level = GF_SdatGetAttrPtr(23);
        break;
    case 1:
        level = GF_SdatGetAttrPtr(24);
        break;
    case 2:
        level = GF_SdatGetAttrPtr(25);
        break;
    case 3:
        level = GF_SdatGetAttrPtr(26);
        break;
    case 4:
        level = GF_SdatGetAttrPtr(27);
        break;
    case 5:
        level = GF_SdatGetAttrPtr(28);
        break;
    case 6:
        level = GF_SdatGetAttrPtr(29);
        break;
    }
    return *level;
}

void Sound_SetPlayerPause(u8 playerNo, u8 pause) {
    u8 *flag;
    enum SoundHandleNo handleNo;
    if (playerNo == 1) {
        flag = GF_SdatGetAttrPtr(12);
        handleNo = SND_HANDLE_FIELD;
    } else if (playerNo == 7) {
        flag = GF_SdatGetAttrPtr(13);
        handleNo = SND_HANDLE_BGM;
    } else {
        return;
    }
    if (pause == 0) {
        Sound_SetCurrentBGM((u16)GF_NNS_SndPlayerGetSeqNo(GF_GetSoundHandle(handleNo)));
    }
    NNS_SndPlayerPause(GF_GetSoundHandle(handleNo), pause);
    *flag = pause;
}

void Sound_ClearBGMPauseFlags(void) {
    u8 *primary = GF_SdatGetAttrPtr(12);
    u8 *secondary = GF_SdatGetAttrPtr(13);
    *primary = 0;
    *secondary = 0;
}

void GF_SndHandleMoveVolume(int handleNo, int volume, int frames) {
    NNS_SndPlayerMoveVolume(GF_GetSoundHandle(handleNo), volume, frames);
    if (handleNo == 0) {
        GF_SndWorkSetGbSoundsVolume((u8)volume);
    }
}

void GF_SndHandleSetInitialVolume(s32 handleNo, s32 volume) {
    if (volume < 0) {
        volume = 0;
    }
    if (volume > 127) {
        volume = 127;
    }
    NNS_SndPlayerSetInitialVolume(GF_GetSoundHandle(handleNo), volume);
}

void sub_02005448(int seqNo) {
    sub_02005464(seqNo, GF_GetSndHandleByPlayerNo(GF_GetPlayerNoBySeq((u16)seqNo)));
}

void sub_02005464(int seqNo, enum SoundHandleNo handleNo) {
    const NNSSndSeqParam *param = NNS_SndArcGetSeqParam(seqNo);
    int volume;
    if (handleNo == SND_HANDLE_PV || handleNo == SND_HANDLE_CHORUS) {
        volume = 127;
    } else {
        if (param == NULL) {
            return;
        }
        volume = param->volume;
    }
    if (sub_020378CC() == 1) {
        GF_SndHandleSetInitialVolume(handleNo, volume / 5);
    }
}

u16 GF_GetVolumeBySeqNo(int seqNo) {
    const NNSSndSeqParam *param = NNS_SndArcGetSeqParam(seqNo);
    if (param == NULL) {
        return 0;
    }
    return param->volume;
}

void GF_SetVolumeBySeqNo(u16 seqNo, u16 volume) {
    GF_SndHandleSetInitialVolume(GF_GetSndHandleByPlayerNo(GF_GetPlayerNoBySeq(seqNo)), volume);
}

u32 GF_SndPlayerCountPlayingSeqByPlayerNo(u32 playerNo) {
    GF_ASSERT((s32)playerNo >= 0);
    return NNS_SndPlayerCountPlayingSeqByPlayerNo(playerNo);
}

u8 GF_GetPlayerNoBySeq(u16 seqNo) {
    const NNSSndSeqParam *param;
    if (seqNo == 0) {
        return 255;
    }
    param = NNS_SndArcGetSeqParam(seqNo);
    if (param == NULL) {
        return 255;
    }
    return param->playerNo;
}

int GF_NNS_SndPlayerGetSeqNo(NNSSndHandle *handle) {
    return NNS_SndPlayerGetSeqNo(handle);
}

const struct NNSSndArcBankInfo *GF_GetBankInfoBySeqNo(int seqNo) {
    return NNS_SndArcGetBankInfo(GF_GetBankBySeqNo(seqNo));
}

int GF_GetBankBySeqNo(int seqNo) {
    const NNSSndSeqParam *param = NNS_SndArcGetSeqParam(seqNo);
    if (param == NULL) {
        return 0;
    }
    return param->bankNo;
}

BOOL Sound_IsMicCounterFull(void) {
    return GF_SndWorkMicCounterFull();
}

BOOL GF_MIC_StartAutoSampling(MICAutoParam *param) {
    MICResult result = MIC_StartAutoSampling(param);
    ((u8 *)_021D05E8)[0] = 1;
    gSoundMicAutoParam = *param;
    return result;
}

void GF_MIC_StopAutoSampling(void) {
    GetSoundDataPointer();
    ((u8 *)_021D05E8)[0] = 0;
    MIC_StopAutoSampling();
}

void GF_MicPauseOnLidClose(void) {
    if (((u8 *)_021D05E8)[0] != 0) {
        if (MIC_StopAutoSampling() != MIC_RESULT_SUCCESS) {
            GF_ASSERT(FALSE);
        }
    }
}

void GF_MicResumeOnLidOpen(void) {
    if (((u8 *)_021D05E8)[0] != 0) {
        if (MIC_StartAutoSampling(&gSoundMicAutoParam) != MIC_RESULT_SUCCESS) {
            GF_ASSERT(FALSE);
        }
    }
    GF_SndWorkMicCounterReset();
}

NNSSndWaveOutHandle *Sound_GetWaveOutHandle(int channel) {
    GetSoundDataPointer();
    u8 *first = GF_SdatGetAttrPtr(16);
    u8 *second = GF_SdatGetAttrPtr(17);
    GF_ASSERT(channel == 14 || channel == 15);
    if (channel == 14 && *first == 0) {
        GF_ASSERT(FALSE);
    }
    if (channel == 15 && *second == 0) {
        GF_ASSERT(FALSE);
    }
    if (channel == 14) {
        return GF_SdatGetAttrPtr(0);
    }
    return GF_SdatGetAttrPtr(1);
}

BOOL Sound_AllocWaveOutChannel(int channel) {
    GetSoundDataPointer();
    u8 *first = GF_SdatGetAttrPtr(16);
    u8 *second = GF_SdatGetAttrPtr(17);
    GF_ASSERT(channel == 14 || channel == 15);
    if (channel == 14) {
        if (*first == 0) {
            NNSSndWaveOutHandle *handle = GF_SdatGetAttrPtr(0);
            *handle = NNS_SndWaveOutAllocChannel(channel);
            if (*handle == NULL) {
                return FALSE;
            }
            *first = 1;
        } else {
            GF_ASSERT(FALSE);
        }
    } else {
        if (*second == 0) {
            NNSSndWaveOutHandle *handle = GF_SdatGetAttrPtr(1);
            *handle = NNS_SndWaveOutAllocChannel(channel);
            if (*handle == NULL) {
                return FALSE;
            }
            *second = 1;
        } else {
            GF_ASSERT(FALSE);
        }
    }
    return TRUE;
}

void Sound_FreeWaveOutChannel(int channel) {
    GetSoundDataPointer();
    u8 *first = GF_SdatGetAttrPtr(16);
    u8 *second = GF_SdatGetAttrPtr(17);
    if (channel != 14 && channel != 15) {
        GF_ASSERT(FALSE);
        return;
    }
    if (channel == 14) {
        if (*first == 1) {
            NNS_SndWaveOutFreeChannel(*Sound_GetWaveOutHandle(channel));
            *first = 0;
        } else {
            GF_ASSERT(FALSE);
        }
    } else {
        if (*second == 1) {
            NNS_SndWaveOutFreeChannel(*Sound_GetWaveOutHandle(channel));
            *second = 0;
        } else {
            GF_ASSERT(FALSE);
        }
    }
}

BOOL Sound_StartWaveOut(SoundWaveOutParam *param, int channel) {
    BOOL success = NNS_SndWaveOutStart(*param->handle, param->format, param->data, param->loopFlag, param->loopStartSample, param->samples, param->sampleRate, param->volume, param->speed, param->pan);
    if (!success) {
        Sound_FreeWaveOutChannel(channel);
    }
    return success;
}

void Sound_StopWaveOut(int channel) {
    NNS_SndWaveOutStop(*Sound_GetWaveOutHandle(channel));
}

BOOL Sound_IsWaveOutPlaying(int channel) {
    return NNS_SndWaveOutIsPlaying(*Sound_GetWaveOutHandle(channel));
}

void Sound_SetWaveOutPan(int channel, u32 pan) {
    if (pan > 127) {
        pan = 127;
    }
    NNS_SndWaveOutSetPan(*Sound_GetWaveOutHandle(channel), pan);
}

void Sound_SetWaveOutSpeed(int channel, u32 speed) {
    NNS_SndWaveOutSetSpeed(*Sound_GetWaveOutHandle(channel), speed);
}

void Sound_SetWaveOutVolume(int channel, int volume) {
    if (sub_020378CC() == 1) {
        NNS_SndWaveOutSetVolume(*Sound_GetWaveOutHandle(channel), volume / 5);
    } else {
        NNS_SndWaveOutSetVolume(*Sound_GetWaveOutHandle(channel), volume);
    }
}

BOOL Sound_PlayReversedWaveArc(int waveArcNo, int volume, int pan, int channel, int heapId) {
    GetSoundDataPointer();
    void **buffer = GF_SdatGetAttrPtr(34);
    const WaveArcInfo *info;
    u32 size;
    SoundWaveOutParam param;
    BOOL success;
    GF_ASSERT(channel == 14 || channel == 15);
    info = NNS_SndArcGetWaveArcInfo(waveArcNo);
    if (info == NULL) {
        GF_ASSERT(FALSE);
        return FALSE;
    }
    size = NNS_SndArcGetFileSize(info->fileId);
    if (size == 0) {
        GF_ASSERT(FALSE);
        return FALSE;
    }
    if (channel == 14) {
        *buffer = Heap_Alloc((enum HeapID)heapId, size);
        if (*buffer == NULL) {
            GF_ASSERT(FALSE);
            return FALSE;
        }
        memset(*buffer, 0, size);
        if (NNS_SndArcReadFile(info->fileId, *buffer, size, 0) == -1) {
            GF_ASSERT(FALSE);
            return FALSE;
        }
        Sound_ReverseWaveBuffer(*buffer, size);
    }
    param.handle = Sound_GetWaveOutHandle(channel);
    param.format = NNS_SND_WAVE_FORMAT_PCM8;
    param.data = *buffer;
    param.loopFlag = 0;
    param.loopStartSample = 0;
    param.samples = size;
    param.sampleRate = 13379;
    param.volume = volume;
    param.speed = 24576;
    param.pan = pan;
    success = Sound_StartWaveOut(&param, channel);
    Sound_SetWaveOutVolume(channel, volume);
    *(u8 *)GF_SdatGetAttrPtr(15) = 1;
    return success;
}

void Sound_ReverseWaveBuffer(void *buffer, u32 size) {
    u8 *data = buffer;
    u32 i;
    for (i = 0; i < size / 2; i++) {
        u8 temp = data[i];
        data[i] = data[size - 1 - i];
        data[size - 1 - i] = temp;
    }
}

void Sound_StopWaveOutAndFreeBuffer(int channel) {
    GetSoundDataPointer();
    u8 *allocated = GF_SdatGetAttrPtr(15);
    void **buffer = GF_SdatGetAttrPtr(34);
    GF_ASSERT(channel == 14 || channel == 15);
    Sound_StopWaveOut(channel);
    if (*allocated == 1) {
        *allocated = 0;
        Heap_Free(*buffer);
    }
}

BOOL Sound_IsCaptureActive(void) {
    BOOL active = NNS_SndCaptureIsActive();
    if (active == 1) {
        Sound_GetCaptureType();
    }
    return active;
}

int Sound_GetCaptureType(void) {
    return NNS_SndCaptureGetCaptureType();
}

void Sound_StopCaptureReverb(int frames) {
    NNS_SndCaptureStopReverb(frames);
}

void GF_SndHandleSetTrackPitch(int handleNo, u32 tracks, int pitch) {
    NNS_SndPlayerSetTrackPitch(GF_GetSoundHandle(handleNo), tracks, pitch);
}

void GF_SndSetTrackPitchBySeqNo(u16 seqNo, u32 tracks, int pitch) {
    GF_SndHandleSetTrackPitch(GF_GetSndHandleByPlayerNo(GF_GetPlayerNoBySeq(seqNo)), tracks, pitch);
}

void GF_SndSetTrackPitchByPlayerNo(int playerNo, u32 tracks, int pitch) {
    GF_SndHandleSetTrackPitch(GF_GetSndHandleByPlayerNo(playerNo), tracks, pitch);
}

void GF_SndHandleSetTrackPan(int handleNo, u32 tracks, int pan) {
    NNS_SndPlayerSetTrackPan(GF_GetSoundHandle(handleNo), tracks, pan);
}

void GF_SndHandleSetTempoRatio(int handleNo, int ratio) {
    NNS_SndPlayerSetTempoRatio(GF_GetSoundHandle(handleNo), ratio);
}

void GF_SndSetMonoFlag(int flag) {
    NNS_SndSetMonoFlag(flag);
    _021D05E8[2] = flag;
}

void GF_SndSetFadeTimer(u32 frames) {
    *(u32 *)GF_SdatGetAttrPtr(7) = frames;
}

void GF_SndSetAfterFadeDelayTimer(u32 frames) {
    *(u32 *)GF_SdatGetAttrPtr(8) = frames;
}

BOOL GF_SndGetAfterFadeDelayTimer(void) {
    u16 *timer = GF_SdatGetAttrPtr(8);
    if (*timer == 0) {
        *timer = 0;
        return 0;
    }
    --*timer;
    return *timer;
}

void Sound_SetMasterVolume(int volume) {
    NNS_SndSetMasterVolume(volume);
}

s8 *Sound_GetChatotBuffer(void) {
    return _021D0620;
}

void Sound_SetReloadFieldSeqFlag(u16 state) {
    *(u8 *)GF_SdatGetAttrPtr(19) = state;
}

BOOL GF_NowStartMusicId(u16 seqNo, u16 fadeOutFrames, int delay, u8 reload, int unused) {
    return GF_NowStartMusicIdEx(4, seqNo, fadeOutFrames, delay, reload, unused);
}

BOOL GF_NowStartMusicIdEx(int scene, u16 seqNo, u16 fadeOutFrames, int delay, u8 reload, int unused) {
    u8 *secondary = GF_SdatGetAttrPtr(22);
    Sound_PrepareBGMChange(scene, seqNo, fadeOutFrames, delay, reload, unused);
    *secondary = 0;
    GF_SndSetState(5);
    return TRUE;
}

BOOL GF_FadeStartMusicId(u16 seqNo, u16 fadeOutFrames, int delay, int fadeInFrames, u8 reload, int unused) {
    return GF_FadeStartMusicIdEx(4, seqNo, fadeOutFrames, delay, fadeInFrames, reload, unused);
}

BOOL GF_FadeStartMusicIdEx(int scene, u16 seqNo, u16 fadeOutFrames, int delay, int fadeInFrames, u8 reload, int unused) {
    int *fadeIn = GF_SdatGetAttrPtr(9);
    Sound_PrepareBGMChange(scene, seqNo, fadeOutFrames, delay, reload, unused);
    *fadeIn = fadeInFrames;
    GF_SndSetState(6);
    return TRUE;
}

void Sound_PrepareBGMChange(int scene, u16 seqNo, u16 fadeOutFrames, int delay, u8 reload, int unused) {
    const struct NNSSndArcBankInfo **bank = GF_SdatGetAttrPtr(2);
    GF_SndStartFadeOutBGM(0, fadeOutFrames);
    Sound_SetCurrentBGM(0);
    GF_SetCurrentPlayingBGM(seqNo);
    GF_SndSetAfterFadeDelayTimer(delay);
    *bank = GF_GetBankInfoBySeqNo(seqNo);
    Sound_SetReloadFieldSeqFlag(reload);
}

void GF_SndSetAllocatableChannelForBGMPlayer(u32 channels) {
    NNS_SndPlayerSetAllocatableChannel(7, channels);
}

void Sound_SetBGMPlayerChannels(int mode) {
    if (mode == 0) {
        GF_SndSetAllocatableChannelForBGMPlayer(0xA7FE);
        Sound_StopCaptureReverb(0);
    } else {
        GF_SndSetAllocatableChannelForBGMPlayer(0x3FFF);
    }
    Sound_IsCaptureActive();
}

void Sound_PauseOrStopFieldBGM(void) {
    if (GF_SndGetFadeTimer() == 0 && GF_NNS_SndPlayerGetSeqNo(GF_GetSoundHandle(0)) != -1) {
        StopBGMSEAndCry();
        Sound_SetPlayerPause(1, 1);
    } else {
        Sound_Stop();
    }
}
