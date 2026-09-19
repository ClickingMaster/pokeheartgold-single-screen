#ifndef TO53_H15_LANDING_PRIVATE_H
#define TO53_H15_LANDING_PRIVATE_H

#include "global.h"

typedef struct Ov45TimedState {
    u8 flag0 : 1;
    u8 flag1 : 1;
    u8 flag23 : 2;
    u8 flag4 : 1;
    u8 flag56 : 2;
    u8 flag7 : 1;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s16 timer4;
    s16 timer6;
    s16 timer8;
    s16 timerA;
    u8 unkC;
    u8 unkD;
    u8 unkE;
} Ov45TimedState;

typedef struct Ov45FlagOwner {
    u8 filler[8];
    u32 flags;
} Ov45FlagOwner;

void GF_SndStartFadeOutBGM(int, int);
void PlayBGM(int);
void GF_SndHandleSetPlayerVolume(int, int);
void ov45_0222CA8C(void *);
void ov45_0222C3A8(void *);
void ov45_0222BB60(Ov45TimedState *, Ov45FlagOwner *, void *, void *);
void ov45_0222BC3C(Ov45TimedState *);
void ov45_0222BC84(Ov45TimedState *);

#endif
