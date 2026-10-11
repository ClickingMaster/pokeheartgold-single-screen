#ifndef T13_PRIVATE_H
#define T13_PRIVATE_H
#include "global.h"
typedef struct SinMotion {
    s16 x, y;
    int duration, angleX, ampX, angleY, ampY, stepX, stepY;
} SinMotion;
typedef struct Linear1 {
    s32 value;
    u32 frames, start, unused, delta;
} Linear1;
typedef struct Linear2 {
    s16 x, y;
    u32 frames;
    s32 dx, dy, curX, curY;
} Linear2;
typedef struct Child {
    void *sprite;
    SinMotion sin;
    u8 sinPad[4];
    Linear1 linear;
    s16 offX, offY, offZ;
    u16 pad42;
    int state;
    int active;
} Child;
typedef struct MainWork {
    void *owner;
    void *unk4;
    void *spriteSystem;
    void *sprite;
    SinMotion sin;
    u8 sinPad[4];
    Linear1 linear;
    s16 baseX, x, y;
    u16 pad4e;
    int unk50;
    int unk54;
    Child child[2];
    int state;
    int timer;
    int index;
    int blend;
} MainWork;
typedef struct PicWork {
    void *owner;
    void *spriteSystem;
    int state;
    int unkC;
    SinMotion sin;
    u8 sinPad[4];
    Linear2 linear;
    u8 pad4C[0xC];
    void *sprite;
    void *pokepic;
    s16 baseX, baseY;
    s16 pad64;
    s16 deltaY;
} PicWork;
void *Heap_Alloc(int, int);
void Heap_Free(void *);
int ov07_0221BFD0(void *);
int ov07_0221C468(void *);
int ov07_0221C470(void *);
void ov07_0221C410(void *, void *, void *);
void ov07_0221C448(void *, void *);
void *ov07_0221C4E8(void *, int);
void *ov07_0221C514(void *);
void *ov07_0221FA48(void *, void *);
int ov07_02222004(void *, int);
s16 ov07_02221F80(void *, int, int);
void ov07_0222212C(SinMotion *, u16, u16, int, int, int, int, s16);
BOOL ov07_02222180(SinMotion *);
void ov07_02222494(Linear1 *, u32, u32, u32, u32);
BOOL ov07_022224AC(Linear1 *);
void ov07_02222590(void *, int, int, int, int, int, int);
BOOL ov07_0222260C(Linear2 *);
void ov07_02222644(Linear2 *, f32 *, f32 *);
void ov07_022227A8(SinMotion *, int, int, int, int);
BOOL ov07_022227D8(SinMotion *);
void ov07_0222C18C(Child *, void *, void *, void *, void *);
void ov07_0222C1FC(Child *);
void ov07_0222C208(Child *, int);
void BattleAnimUtil_SetBlendWithBattleBg(void *, int, int);
void ManagedSprite_SetPositionXY(void *, s16, s16);
void ManagedSprite_SetOamMode(void *, int);
void ManagedSprite_SetDrawFlag(void *, BOOL);
void ManagedSprite_TickFrame(void *);
void ManagedSprite_SetDrawPriority(void *, int);
void ManagedSprite_SetPriority(void *, int);
void ManagedSprite_SetAffineOverwriteMode(void *, int);
void ManagedSprite_SetAffineScale(void *, f32, f32);
void SpriteSystem_DrawSprites(void *);
void Sprite_DeleteAndFreeResources(void *);
int Sprite_IsAnimated(void *);
void Pokepic_SetAttr(void *, int, int);
int Pokepic_GetAttr(void *, int);
BOOL ov07_0222C214(Child *);
void ov07_0222C2D4(void *, MainWork *);
void ov07_0222C4C0(void *, void *, void *, void *);
void ov07_0222C610(PicWork *);
void ov07_0222C6A8(void *, PicWork *);
void ov07_0222C780(void *);
#endif
