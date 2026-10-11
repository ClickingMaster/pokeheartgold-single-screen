#include "to54_atomic_w02_private.h"

BOOL ov07_02221CEC(int unused, W02Ctx *ctx);
BOOL ov07_02221D10(int unused, W02Ctx *ctx);
BOOL ov07_02221D4C(int unused, W02Ctx *ctx);
BOOL ov07_02221D88(int unused, W02Ctx *ctx);
BOOL ov07_02221DC4(int unused, W02Ctx *ctx);
BOOL ov07_02221E24(int unused, W02Ctx *ctx);
BOOL ov07_02221E60(int unused, W02Ctx *ctx);
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

BOOL ov07_02221E9C(int unused, W02Ctx *ctx) {
    VecFx32 v;
    void *camera = ParticleSystem_GetCamera(ctx->emitter);
    ov07_02231D70(ctx->main, ctx->obj24, &v);
    Camera_SetLookAtCamTarget(&v, camera);
    return TRUE;
}

BOOL ov07_02221EC4(int unused, W02Ctx *ctx) {
    VecFx32 v;
    void *camera = ParticleSystem_GetCamera(ctx->emitter);
    ov07_02231D70(ctx->main, ctx->obj28, &v);
    Camera_SetLookAtCamTarget(&v, camera);
    return TRUE;
}

void ov07_02221EEC(int i, void *a, void *b) {
    ov07_02236444[i](a, b);
}

void ov07_02221F04(int set, int idx, s16 *a, s16 *b) {
    if (a != NULL) {
        *a = ov07_02236468[set][idx * 2];
    }
    if (b != NULL) {
        *b = ov07_0223646A[set][idx * 2];
    }
}

void ov07_02221F38(void *p, int idx, s16 *a, s16 *b) {
    if (ov07_0221BFC0(p) == 1) {
        ov07_02221F04(0, idx + 2, a, b);
    } else if (ov07_0221FAB0(p) == 1) {
        ov07_02221F04(1, idx - 2, a, b);
    } else {
        ov07_02221F04(0, idx, a, b);
    }
}

s16 ov07_02221F80(void *p, int unused, u32 which) {
    int idx = ov07_02231924(p);
    int set = ov07_0221FAB0(p) == 1;
    if (ov07_0221BFC0(p) == 1) {
        idx += 2;
    }
    switch (which) {
    case 0:
    case 2:
        return ov07_02236468[set][idx * 2];
    case 1:
    case 3:
        return ov07_0223646A[set][idx * 2];
    default:
        GF_AssertFail();
        return 0;
    }
}

u8 ov07_02221FF0(void **p) {
    return ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(*p), 1);
}

int ov07_02222004(void *p, int index) {
    int out = 1;
    int v = ov07_0223192C(p);
    if (ov07_0221BFC0(p) != 0) {
        if (v == 3) {
            out -= 2;
        }
    } else if (v == 4) {
        out -= 2;
    }
    return out;
}

int ov07_0222202C(void *p) {
    int out = 1;
    if (ov07_0223192C(p) == 4) {
        out -= 2;
    }
    return out;
}

int ov07_02222040(int start, int end, int duration) {
    return FX_Div(end - start, duration << FX32_SHIFT);
}

int ov07_0222204C(int start, int end, int duration) {
    int value = FX_Div(end - start, duration);
    if (FX_Modf(value, &value) != 0) {
        value += FX32_ONE;
    }
    value = value < 0 ? -value : value;
    return value >> FX32_SHIFT;
}

void ov07_0222207C(W02Point *off, void *sprite, s16 x, s16 y) {
    ManagedSprite_SetPositionXY(sprite, x + off->x, y + off->y);
}

void ov07_0222209C(void *anim, void *sprite) {
    u32 x, y;
    ov07_02222644(anim, &x, &y);
    ManagedSprite_SetAffineScale(sprite, x, y);
}

void ov07_022220B8(W02Point *off, void *pic, int x, int y) {
    Pokepic_SetAttr(pic, 0, x + off->x);
    Pokepic_SetAttr(pic, 1, y + off->y);
}

void ov07_022220DC(W02Point *off, void *pic) {
    Pokepic_SetAttr(pic, 12, off->x);
    Pokepic_SetAttr(pic, 13, off->y);
}

void ov07_022220FC(W02SinMotion *m, int ax, int axEnd, int ay, u16 ayEnd, int ampX, int ampY, int duration) {
    m->duration = duration;
    m->angleX = ax;
    m->ampX = ampX;
    m->angleY = ay;
    m->ampY = ampY;
    m->stepX = _s32_div_f(axEnd - ax, duration);
    m->stepY = _s32_div_f((u16)ayEnd - ay, duration);
}
