#ifndef POKEHEARTGOLD_OVERLAY92_MOTION_CALLBACKS_R14B_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY92_MOTION_CALLBACKS_R14B_PRIVATE_H
#include "global.h"

typedef struct ManagedSprite ManagedSprite;
typedef struct SysTask SysTask;

typedef struct Ov92Motion {
    ManagedSprite *sprite;
    s32 state;
    s16 angle;
    u8 pad0A[6];
    s32 cycles;
    s16 baseX;
    s16 baseY;
    fx32 radiusX;
    fx32 radiusY;
    u8 tweenX[0x18];
    u8 tweenY[0x18];
} Ov92Motion;

typedef struct Ov92Sub {
    u8 pad00[0x34];
    u8 unk34;
} Ov92Sub;

typedef struct Ov92Ctx {
    Ov92Motion motions[4];
    s32 count;
    s32 dur1;
    s32 dur2;
    s32 loopCount;
    u8 pad150[0x18];
    s32 state;
    u8 pad16C[0x10];
    Ov92Sub *unk17C;
} Ov92Ctx;

typedef struct Ov92Done4 {
    BOOL done[4];
} Ov92Done4;

extern const Ov92Done4 ov92_02263E0C;
extern const Ov92Done4 ov92_02263E1C;
extern const Ov92Done4 ov92_02263E2C;
extern const Ov92Done4 ov92_02263E4C;

BOOL IsPaletteFadeFinished(void);
void SysTask_Destroy(SysTask *task);
void ManagedSprite_SetDrawFlag(ManagedSprite *, BOOL);
void ManagedSprite_GetPositionFxXYWithSubscreenOffset(ManagedSprite *, fx32 *, fx32 *, fx32);
void ManagedSprite_SetPositionFxXYWithSubscreenOffset(ManagedSprite *, fx32, fx32, fx32);
fx32 GF_SinDeg(u16);
fx32 GF_CosDeg(u16);
void ov92_02261E80(Ov92Motion *);
BOOL ov92_02261E88(Ov92Motion *, s32, s32, s32);
BOOL ov92_02261F60(Ov92Motion *, s32, s32, s32);
BOOL ov92_02262018(Ov92Motion *, s32, s32, s32);
BOOL ov92_022620D0(Ov92Motion *, s32, s32, s32);

void ov92_022621A4(SysTask *, Ov92Ctx *);
void ov92_02262548(SysTask *, Ov92Ctx *);
void ov92_022628F0(SysTask *, Ov92Ctx *);
void ov92_02262CEC(SysTask *, Ov92Ctx *);

#endif
