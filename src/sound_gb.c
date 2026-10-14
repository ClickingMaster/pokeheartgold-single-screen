#include "sound.h"
#include "sound_system.h"

extern int _021D05E8[4];
extern const u8 sSoundFootstepVariations[8][4];
extern const u16 gGBSoundsSeqPairs[136][2];
BOOL NNS_SndPlayerReadVariable(NNSSndHandle *handle, u32 variable, s16 *value);
void NNS_SndPlayerSetTrackAllocatableChannel(NNSSndHandle *handle, u32 tracks, u32 channels);
u8 GF_GetPlayerNoBySeq(u16 seqNo);
void GF_SndHandleSetTrackPitch(int handleNo, u32 tracks, int pitch);
void Sound_SetKeepPlayingCries(u8 value);
void Sound_SetChatotRecordingDisabled(u8 value);
BOOL GF_SndReadSeqVariable(u16 seqNo, u32 variable, s16 *value);
void Sound_ApplyFootstepVariation(u16 seqNo);
int Sound_IsSelectSEMuted(void);
void SoundSys_SetGBSoundsState(u8 enabled);
u16 GBSounds_GetGBSeqNoByDSSeqNo(u16 seqNo);
u16 GBSounds_GetDSSeqNoByGBSeqNo(u16 seqNo);
void GBSounds_SetAllocatableChannels(void);
void Sound_SetFieldBGMStopOnce(BOOL enabled);

void Sound_SetKeepPlayingCries(u8 value) {
    *(u8 *)GF_SdatGetAttrPtr(53) = value;
}
void Sound_SetChatotRecordingDisabled(u8 value) {
    *(u8 *)GF_SdatGetAttrPtr(54) = value;
}
BOOL GF_SndReadSeqVariable(u16 seqNo, u32 variable, s16 *value) {
    GF_ASSERT(value != NULL);
    GF_ASSERT(variable <= 15);
    return NNS_SndPlayerReadVariable(GF_GetSoundHandle(GF_GetSndHandleByPlayerNo(GF_GetPlayerNoBySeq(seqNo))), variable, value);
}
void Sound_ApplyFootstepVariation(u16 seqNo) {
    u8 *state = GF_SdatGetAttrPtr(55);
    GF_SetVolumeBySeqNo(seqNo, sSoundFootstepVariations[state[0]][1]);
    GF_SndHandleSetTrackPitch(SND_HANDLE_SE_2, 65535, sSoundFootstepVariations[state[0]][0]);
    if (state[1] >= 8) {
        state[1] = 0;
    }
}
void Sound_SetSelectSEMuted(BOOL enabled) {
    *(u8 *)GF_SdatGetAttrPtr(56) = enabled;
}
int Sound_IsSelectSEMuted(void) {
    return *(u8 *)GF_SdatGetAttrPtr(56);
}
void SoundSys_SetGBSoundsState(u8 enabled) {
    *(u8 *)GF_SdatGetAttrPtr(57) = enabled;
}
BOOL SoundSys_GetGBSoundsState(void) {
    return *(u8 *)GF_SdatGetAttrPtr(57);
}
void SoundSys_ToggleGBSounds(void) {
    if (SoundSys_GetGBSoundsState() == 0) {
        SoundSys_SetGBSoundsState(1);
    } else {
        SoundSys_SetGBSoundsState(0);
    }
    if (GF_GetQueuedBGM() == 0) {
        u16 seqNo = GF_GetCurrentPlayingBGM();
        u8 volume = GF_SndWorkGetGbSoundsVolume();
        if (seqNo != GBSounds_GetGBSeqNoByDSSeqNo(seqNo)) {
            PlayBGM(seqNo);
        }
        GF_SndHandleMoveVolume(0, volume, 0);
    }
}
u16 GBSounds_GetGBSeqNoByDSSeqNo(u16 seqNo) {
    u16 i;
    for (i = 0; i < 136; i++) {
        if (seqNo == gGBSoundsSeqPairs[i][0]) {
            return gGBSoundsSeqPairs[i][1];
        }
    }
    return seqNo;
}
u16 GBSounds_GetDSSeqNoByGBSeqNo(u16 seqNo) {
    u16 i;
    for (i = 0; i < 136; i++) {
        if (seqNo == gGBSoundsSeqPairs[i][1]) {
            return gGBSoundsSeqPairs[i][0];
        }
    }
    return seqNo;
}
void GBSounds_SetAllocatableChannels(void) {
    NNS_SndPlayerSetTrackAllocatableChannel(GF_GetSoundHandle(0), 15, 0xA7FE);
    NNS_SndPlayerSetTrackAllocatableChannel(GF_GetSoundHandle(7), 15, 0xA7FE);
    NNS_SndPlayerSetTrackAllocatableChannel(GF_GetSoundHandle(2), 15, 0xA7FE);
}
void Sound_SetFieldBGMStopOnce(BOOL enabled) {
    _021D05E8[3] = enabled;
}
void Sound_ResetFieldBGMStopOnce(void) {
    _021D05E8[3] = 0;
    _021D05E8[1] = 0;
}
