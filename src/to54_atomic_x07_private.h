#ifndef TO54_X07_PRIVATE_H
#define TO54_X07_PRIVATE_H
#include "global.h"
typedef struct X07ScaleSprite {
    void *sprite;
    u8 scaleAnim[0x24];
    int flip;
    int delay;
    int start;
    int end;
} X07ScaleSprite;
typedef struct X07PicPulse {
    u8 pad00[0x10];
    void *pokepic;
    u8 anim[2];
    s16 value;
    u8 pad18[0x20];
    int state;
} X07PicPulse;
typedef struct X07Group {
    void *owner;
    u8 pad04[4];
    void *spriteSystem;
    int state;
    int timer;
    int index;
    void *pokepic;
    s16 x, y;
    u8 motion[0x24];
    X07ScaleSprite sprites[4];
} X07Group;
typedef struct X07Popup {
    void *owner;
    void *spriteSystem;
    int state;
    int timer;
    void *pokepic;
    void *sprite;
    s16 x, y;
    u8 motion[0x24];
    u8 anim[0x28];
    s16 attr;
} X07Popup;
typedef struct X07SpritePulse {
    u8 pad00[0x3c];
    void *sprite;
    int state;
    u8 anim0[0x24];
    u8 anim1[0x24];
    u8 anim2[0x28];
    s16 x;
    s16 y;
} X07SpritePulse;
BOOL BattleAnimUtil_UpdateScaleMotion(void *);
void BattleAnimUtil_GetScaleFloat(void *, f32 *, f32 *);
void BattleAnimUtil_InitScaleMotion(void *, int, int, int, int);
void ManagedSprite_SetAffineScale(void *, f32, f32);
f32 _fsub(f32, f32);
void Pokepic_SetAttr(void *, int, int);
void BattleAnimUtil_UpdateScaleMotionSprite(void *, void *);
BOOL BattleAnimUtil_UpdateLinearMotion(void *);
BOOL BattleAnimUtil_UpdateSinMotionSprite(void *, s16, s16, void *);
void ov07_0222207C(void *, void *, s16, s16);
void BattleAnimUtil_StartBlendAlphaMotion(void *, int, int, int, int, int);
BOOL BattleAnimUtil_IsBlendAlphaMotionDone(void *);
void ManagedSprite_SetDrawFlag(void *, BOOL);
BOOL BattleAnimUtil_UpdateShakePokepic(void *, s16, s16, void *);
void Sprite_DeleteAndFreeResources(void *);
void BattleAnimSystem_DestroyTask(void *, void *);
void Heap_Free(void *);
void SpriteSystem_DrawSprites(void *);
void ov07_0222D56C(X07ScaleSprite *);
void ov07_0222D578(X07ScaleSprite *);
void ov07_0222D60C(void *, X07Group *);
BOOL ov07_0222D590(X07ScaleSprite *);
void ov07_0222DA60(void *, X07Popup *);
BOOL ov07_0222DBD8(X07PicPulse *);
BOOL ov07_0222DC54(X07SpritePulse *);
#endif
