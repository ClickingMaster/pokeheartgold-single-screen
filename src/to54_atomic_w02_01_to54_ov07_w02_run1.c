#include "to54_atomic_w02_private.h"

BOOL ov07_02221D10(int unused, W02Ctx *ctx);
BOOL ov07_02221D4C(int unused, W02Ctx *ctx);
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
void ov07_0222212C(W02SinMotion *m, u16 ax, u16 axEnd, int ay, int ayEnd, int ampX, int ampY, s16 stepX);
BOOL ov07_02222180(W02SinMotion *m);
BOOL ov07_02222218(W02SinMotion *m, s16 x, s16 y, void *sprite);
BOOL ov07_02222240(W02SinMotion *m, s16 x, s16 y, void *pic);
void ov07_02222268(W02LinearMotion *m, s16 x, s16 xEnd, s16 y, s16 yEnd, u16 duration);
BOOL ov07_022222B4(W02LinearMotion *m);
BOOL ov07_022222F0(W02LinearMotion *m, void *sprite);
BOOL ov07_02222314(W02LinearMotion *m, void *pic);
BOOL ov07_02222384(W02LinearMotion *a, W02SinMotion *b);
BOOL ov07_022223CC(W02LinearMotion *a, W02SinMotion *b, void *sprite);
void ov07_022223F0(W02ScalarMotion *m, int start, int end, int duration);
void ov07_02222418(W02ScalarMotion *m, int start, int end, int duration);
BOOL ov07_02222440(W02ScalarMotion *m);
BOOL ov07_02222468(W02ScalarMotion *m);

BOOL ov07_02221CEC(int unused, W02Ctx *ctx) {
    W02Angle angle = { 0, 0, 0, 0 };
    Camera_SetAnglePos(&angle, ParticleSystem_GetCamera(ctx->emitter));
    return TRUE;
}
