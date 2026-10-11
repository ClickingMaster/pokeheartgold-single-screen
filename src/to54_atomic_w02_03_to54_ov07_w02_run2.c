#include "to54_atomic_w02_private.h"

BOOL ov07_02221CEC(int unused, W02Ctx *ctx);
BOOL ov07_02221D10(int unused, W02Ctx *ctx);
BOOL ov07_02221D88(int unused, W02Ctx *ctx);
BOOL ov07_02221DC4(int unused, W02Ctx *ctx);
BOOL ov07_02221E24(int unused, W02Ctx *ctx);
BOOL ov07_02221E60(int unused, W02Ctx *ctx);
BOOL ov07_02221E9C(int unused, W02Ctx *ctx);
BOOL ov07_02221EC4(int unused, W02Ctx *ctx);
void ov07_02221EEC(int i, void *a, void *b);
void ov07_02221F04(int set, int idx, s16 *a, s16 *b);
void ov07_02221F38(void *p, int idx, s16 *a, s16 *b);
s16 ov07_02221F80(void *p, int unused, u32 which);
u8 ov07_02221FF0(void **p);
int ov07_02222004(void *p, int index);
int ov07_0222202C(void *p);
int ov07_02222040(int start, int end, int duration);
int ov07_0222204C(int start, int end, int duration);
void ov07_0222207C(W02Point *off, void *sprite, s16 x, s16 y);
void ov07_0222209C(void *anim, void *sprite);
void ov07_022220B8(W02Point *off, void *pic, int x, int y);
void ov07_022220DC(W02Point *off, void *pic);
void ov07_022220FC(W02SinMotion *m, int ax, int axEnd, int ay, u16 ayEnd, int ampX, int ampY, int duration);
void BattleAnimUtil_InitSinMotionFromStep(W02SinMotion *m, u16 ax, u16 axEnd, int ay, int ayEnd, int ampX, int ampY, s16 stepX);
BOOL BattleAnimUtil_UpdateSinMotion(W02SinMotion *m);
BOOL BattleAnimUtil_UpdateSinMotionSprite(W02SinMotion *m, s16 x, s16 y, void *sprite);
BOOL BattleAnimUtil_UpdateSinMotionPokepic(W02SinMotion *m, s16 x, s16 y, void *pic);
void BattleAnimUtil_InitLinearMotion(W02LinearMotion *m, s16 x, s16 xEnd, s16 y, s16 yEnd, u16 duration);
BOOL BattleAnimUtil_UpdateLinearMotion(W02LinearMotion *m);
BOOL BattleAnimUtil_UpdateLinearMotionSprite(W02LinearMotion *m, void *sprite);
BOOL BattleAnimUtil_UpdateLinearMotionPokepic(W02LinearMotion *m, void *pic);
BOOL BattleAnimUtil_UpdateLinearSinMotion(W02LinearMotion *a, W02SinMotion *b);
BOOL BattleAnimUtil_UpdateLinearSinMotionSprite(W02LinearMotion *a, W02SinMotion *b, void *sprite);
void BattleAnimUtil_InitValueMotion(W02ScalarMotion *m, int start, int end, int duration);
void BattleAnimUtil_InitValueMotionFx(W02ScalarMotion *m, int start, int end, int duration);
BOOL BattleAnimUtil_UpdateValueMotion(W02ScalarMotion *m);
BOOL BattleAnimUtil_UpdateValueMotionFx(W02ScalarMotion *m);

BOOL ov07_02221D4C(int unused, W02Ctx *ctx) {
    W02Angle angle = { 0, 0, 0, 0 };
    int pos[4];
    ov07_02221CE0(ctx, pos);
    angle.x = pos[1];
    angle.y = pos[2];
    angle.z = pos[3];
    Camera_SetAnglePos(&angle, ParticleSystem_GetCamera(ctx->emitter));
    return TRUE;
}
