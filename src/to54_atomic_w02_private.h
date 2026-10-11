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
void ov07_02221CE0(void *, void *);
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
void ov07_02222644(void *, u32 *, u32 *);
int _s32_div_f(int, int);
long long _ll_mul(int, int, int, int);
int FX_Modf(int, int *);
extern const s16 FX_SinCosTable_[];
extern W02Angle ov07_02236414[];
extern void (*ov07_02236444[])(void *, void *);
extern s16 ov07_02236468[][12];
extern s16 ov07_0223646A[][12];

BOOL ov07_02221CEC(int, W02Ctx *);
BOOL ov07_02221D10(int, W02Ctx *);
BOOL ov07_02221D4C(int, W02Ctx *);
BOOL ov07_02221D88(int, W02Ctx *);
BOOL ov07_02221DC4(int, W02Ctx *);
BOOL ov07_02221E24(int, W02Ctx *);
BOOL ov07_02221E60(int, W02Ctx *);
BOOL ov07_02221E9C(int, W02Ctx *);
BOOL ov07_02221EC4(int, W02Ctx *);
void ov07_02221EEC(int, void *, void *);
void ov07_02221F04(int, int, s16 *, s16 *);
void ov07_02221F38(void *, int, s16 *, s16 *);
s16 ov07_02221F80(void *, int, u32);
u8 ov07_02221FF0(void **);
int ov07_02222004(void *, int);
int ov07_0222202C(void *);
int ov07_02222040(int, int, int);
int ov07_0222204C(int, int, int);
void ov07_0222207C(W02Point *, void *, s16, s16);
void ov07_0222209C(void *, void *);
void ov07_022220B8(W02Point *, void *, int, int);
void ov07_022220DC(W02Point *, void *);
void ov07_022220FC(W02SinMotion *, int, int, int, u16, int, int, int);
void ov07_0222212C(W02SinMotion *, u16, u16, int, int, int, int, s16);
BOOL ov07_02222180(W02SinMotion *);
BOOL ov07_02222218(W02SinMotion *, s16, s16, void *);
BOOL ov07_02222240(W02SinMotion *, s16, s16, void *);
void ov07_02222268(W02LinearMotion *, s16, s16, s16, s16, u16);
BOOL ov07_022222B4(W02LinearMotion *);
BOOL ov07_022222F0(W02LinearMotion *, void *);
BOOL ov07_02222314(W02LinearMotion *, void *);
BOOL ov07_02222384(W02LinearMotion *, W02SinMotion *);
BOOL ov07_022223CC(W02LinearMotion *, W02SinMotion *, void *);
void ov07_022223F0(W02ScalarMotion *, int, int, int);
void ov07_02222418(W02ScalarMotion *, int, int, int);
BOOL ov07_02222440(W02ScalarMotion *);
BOOL ov07_02222468(W02ScalarMotion *);
#endif
