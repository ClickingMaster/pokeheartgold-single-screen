#ifndef POKEHEARTGOLD_SOUND_PLAYBACK_INTERNAL_H
#define POKEHEARTGOLD_SOUND_PLAYBACK_INTERNAL_H

#include "global.h"

BOOL PlayBGMDirect(u16);
void StopBGMSEAndCry(void);
BOOL PlaySEOnPlayer(u16, int);
BOOL PlaySE_SetVolume(u16, u16);
void SetSeqTrackPan(u16, u32, int);
u32 GF_SndCountPlayingSeqOnPlayer(u32);
void SetPlayerPan(int, int);
void SetAllSEPan(int);
BOOL IsFanfarePlayingOrWaiting(void);

#endif // POKEHEARTGOLD_SOUND_PLAYBACK_INTERNAL_H
