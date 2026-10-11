#ifndef TO54_W02_PRIVATE_H
#define TO54_W02_PRIVATE_H
#include "global.h"

typedef struct W02Ctx {
    void *main;
    void *emitter;
    u8 pad08[0x1c];
    void *obj24;
    void *obj28;
} W02Ctx;
typedef struct W02Point {
    s16 x, y;
} W02Point;
typedef struct W02Angle {
    s16 x, y, z, w;
} W02Angle;
typedef struct W02SinMotion {
    s16 x, y;
    int duration;
    int angleX;
    int ampX;
    int angleY;
    int ampY;
    int stepX;
    int stepY;
} W02SinMotion;
typedef struct W02LinearMotion {
    s16 x, y;
    int duration;
    int dx, dy;
    int curX, curY;
} W02LinearMotion;
typedef struct W02ScalarMotion {
    int value;
    int duration;
    int step;
    int current;
} W02ScalarMotion;

void GF_AssertFail(void);
void *ParticleSystem_GetCamera(void *);
void ParticleSystem_SetPerspectiveType(void *, int);
void Camera_SetAnglePos(const void *, void *);
void Camera_SetLookAtCamTarget(const void *, void *);
void BattleAnimEmitter_ReadCameraCbParams(void *, void *);
int ov07_0221BFC0(void *);
int BattleAnimSystem_IsDoubleBattle(void *);
void ov07_02231D70(void *, void *, void *);
int BattleAnimUtil_GetBattlerType(void *);
int BattleAnimUtil_GetBattlerSide(void *);
void *Sprite_GetPaletteProxy(void *);
u32 ObjPlttTransfer_GetPaletteVramOffset(void *, int);
void ManagedSprite_SetPositionXY(void *, s16, s16);
void ManagedSprite_SetAffineScale(void *, u32, u32);
void Pokepic_SetAttr(void *, int, int);
void BattleAnimUtil_GetScaleFloat(void *, u32 *, u32 *);
int _s32_div_f(int, int);
long long _ll_mul(int, int, int, int);
int FX_Modf(int, int *);
extern const s16 FX_SinCosTable_[];
extern W02Angle sBattleAnimEmitterCameraAngles[];
extern void (*sBattleAnimEmitterCameraCallbacks[])(void *, void *);
extern s16 ov07_02236468[][12];
extern s16 ov07_0223646A[][12];

BOOL BattleAnimEmitterCameraCb_ResetAngle(int, W02Ctx *);
BOOL BattleAnimEmitterCameraCb_SetPresetAngle4(int, W02Ctx *);
BOOL BattleAnimEmitterCameraCb_SetAngleFromScript(int, W02Ctx *);
BOOL BattleAnimEmitterCameraCb_SetPresetAngle3(int, W02Ctx *);
BOOL ov07_02221DC4(int, W02Ctx *);
BOOL BattleAnimEmitterCameraCb_SetPresetAngle0(int, W02Ctx *);
BOOL BattleAnimEmitterCameraCb_SetPresetAngle1(int, W02Ctx *);
BOOL BattleAnimEmitterCameraCb_LookAtBattlerA(int, W02Ctx *);
BOOL BattleAnimEmitterCameraCb_LookAtBattlerB(int, W02Ctx *);
void BattleAnimEmitter_RunCameraCallback(int, void *, void *);
void BattleAnimUtil_LookupScreenPos(int, int, s16 *, s16 *);
void BattleAnimSystem_GetBattlerScreenPos(void *, int, s16 *, s16 *);
s16 BattleAnimSystem_GetBattlerScreenCoord(void *, int, u32);
u8 BattleAnimUtil_GetSpritePaletteSlot(void **);
int ov07_02222004(void *, int);
int BattleAnimUtil_GetBattlerSideSign(void *);
int BattleAnimUtil_CalcStep(int, int, int);
int BattleAnimUtil_CalcStepCount(int, int, int);
void BattleAnimUtil_SetSpritePosWithOffset(W02Point *, void *, s16, s16);
void BattleAnimUtil_SetSpriteScale(void *, void *);
void BattleAnimUtil_SetPokepicPosWithOffset(W02Point *, void *, int, int);
void BattleAnimUtil_SetPokepicScale(W02Point *, void *);
void BattleAnimUtil_InitSinMotion(W02SinMotion *, int, int, int, u16, int, int, int);
void BattleAnimUtil_InitSinMotionFromStep(W02SinMotion *, u16, u16, int, int, int, int, s16);
BOOL BattleAnimUtil_UpdateSinMotion(W02SinMotion *);
BOOL BattleAnimUtil_UpdateSinMotionSprite(W02SinMotion *, s16, s16, void *);
BOOL BattleAnimUtil_UpdateSinMotionPokepic(W02SinMotion *, s16, s16, void *);
void BattleAnimUtil_InitLinearMotion(W02LinearMotion *, s16, s16, s16, s16, u16);
BOOL BattleAnimUtil_UpdateLinearMotion(W02LinearMotion *);
BOOL BattleAnimUtil_UpdateLinearMotionSprite(W02LinearMotion *, void *);
BOOL BattleAnimUtil_UpdateLinearMotionPokepic(W02LinearMotion *, void *);
BOOL BattleAnimUtil_UpdateLinearSinMotion(W02LinearMotion *, W02SinMotion *);
BOOL BattleAnimUtil_UpdateLinearSinMotionSprite(W02LinearMotion *, W02SinMotion *, void *);
void BattleAnimUtil_InitValueMotion(W02ScalarMotion *, int, int, int);
void BattleAnimUtil_InitValueMotionFx(W02ScalarMotion *, int, int, int);
BOOL BattleAnimUtil_UpdateValueMotion(W02ScalarMotion *);
BOOL BattleAnimUtil_UpdateValueMotionFx(W02ScalarMotion *);
#endif
