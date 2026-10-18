#ifndef POKEHEARTGOLD_SOUND_GB_H
#define POKEHEARTGOLD_SOUND_GB_H

#include "global.h"

void Sound_SetKeepPlayingCries(u8);
void Sound_SetChatotRecordingDisabled(u8);
BOOL GF_SndReadSeqVariable(u16, u32, s16 *);
void Sound_ApplyFootstepVariation(u16);
int Sound_IsSelectSEMuted(void);
u16 GBSounds_GetGBSeqNoByDSSeqNo(u16);
u16 GBSounds_GetDSSeqNoByGBSeqNo(u16);
void GBSounds_SetAllocatableChannels(void);
void Sound_SetFieldBGMStopOnce(BOOL);

#endif // POKEHEARTGOLD_SOUND_GB_H
