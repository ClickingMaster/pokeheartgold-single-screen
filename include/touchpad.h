#ifndef POKEHEARTGOLD_TOUCHPAD_H
#define POKEHEARTGOLD_TOUCHPAD_H

#include "global.h"

void GF_TouchpadInit(void);
void GF_TouchpadPauseOnLidClose(void);
void GF_TouchpadResumeOnLidOpen(void);
void GF_TouchpadResetSampling(void);
BOOL GF_TouchpadStartSampling(int);
BOOL GF_TouchpadStopSampling(void);

#endif // POKEHEARTGOLD_TOUCHPAD_H
