#ifndef POKEHEARTGOLD_OVERLAY92_ANIMATION_TAIL_TO50_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY92_ANIMATION_TAIL_TO50_PRIVATE_H
#include "global.h"

#include "sprite.h"

typedef struct Ov92AnimPointTo50 {
    s16 x, y;
} Ov92AnimPointTo50;
typedef struct Ov92AnimTableTo50 {
    Ov92AnimPointTo50 v[48];
} Ov92AnimTableTo50;
typedef struct Ov92AnimTo50 {
    u8 pad00[8];
    s32 prevFrame;
    s32 state;
    u8 pad10[0xF0 - 0x10];
    ManagedSprite *sprite;
    u8 padF4[0x114 - 0xF4];
    u8 obj114[4];
    u8 pad118[0x2A4 - 0x118];
    u8 obj2A4[0x10];
    u8 obj2B4[0x10];
} Ov92AnimTo50;
extern const Ov92AnimTableTo50 ov92_02263A94;
BOOL ov92_0225CC6C(Ov92AnimTo50 *);
void ov92_0225CB2C(void *, s32, s32);
int ManagedSprite_GetAnimationFrame(ManagedSprite *);
int ManagedSprite_GetActiveAnim(ManagedSprite *);
void ManagedSprite_SetPositionXY(ManagedSprite *, s16, s16);
void ManagedSprite_SetAnimNoRestart(ManagedSprite *, int);
void ManagedSprite_SetDrawFlag(ManagedSprite *, BOOL);
void ManagedSprite_TickTwoFrames(ManagedSprite *);
void ov92_02260428(void *, s32, s32, s32, s32, float, s32);
void ov92_02260A38(Ov92AnimTo50 *, s16, s16);
void ov92_022630E8(void *);
#endif
