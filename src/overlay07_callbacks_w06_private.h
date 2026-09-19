#ifndef TO53_SOL_R1_W06_PRIVATE_H
#define TO53_SOL_R1_W06_PRIVATE_H

#include "global.h"

typedef struct W06Sprite {
    void *sprite;
} W06Sprite;

typedef struct W06Point {
    u8 pad00[0x10];
    s16 x;
    s16 y;
} W06Point;

typedef struct W06ScaleSprite {
    void *sprite;
    u8 scaleAnim[0x24];
    int flip;
    int delay;
    int start;
    int end;
} W06ScaleSprite;

typedef struct W06PicPulse {
    u8 pad00[0x10];
    void *pokepic;
    u8 anim[2];
    s16 value;
    u8 pad18[0x20];
    int state;
} W06PicPulse;

typedef struct W06SpritePulse {
    u8 pad00[0x3c];
    void *sprite;
    int state;
    u8 anim0[0x24];
    u8 anim1[0x24];
    u8 anim2[0x28];
    s16 x;
    s16 y;
} W06SpritePulse;

void Sprite_DeleteAndFreeResources(void *sprite);
void ManagedSprite_SetDrawFlag(void *sprite, BOOL draw);
void ManagedSprite_SetAnimateFlag(void *sprite, BOOL animate);
void ManagedSprite_SetPositionXY(void *sprite, s16 x, s16 y);
void ManagedSprite_SetAffineOverwriteMode(void *sprite, int mode);
void ManagedSprite_SetDrawPriority(void *sprite, int priority);
void ManagedSprite_SetPriority(void *sprite, int priority);
void ManagedSprite_SetAffineScale(void *sprite, f32 x, f32 y);
void Pokepic_SetVisible(void *pokepic, int arg1, int arg2, int arg3, int arg4);
void Pokepic_SetAttr(void *pokepic, int attr, int value);
void ov07_02222508(void *anim, int start, int end, int duration, int mode);
BOOL ov07_02222558(void *anim);
void ov07_02222748(void *anim, void *sprite);
BOOL ov07_022222B4(void *anim);
void ov07_0222207C(void *anim, void *sprite, s16 x, s16 y);
void ov07_02222AC4(void *anim, int start, int end, int duration, int mode);
BOOL ov07_02222AF4(void *anim);

void ov07_0222BADC(void *pokepic, int arg1, int arg2, int arg3, int arg4);
void ov07_0222C1FC(W06Sprite *work);
void ov07_0222C208(W06Sprite *work, BOOL draw);
void ov07_0222C850(W06Point *point, u32 *dest);
void ov07_0222D56C(W06Sprite *work);
void ov07_0222D578(W06Sprite *work);
void ov07_0222D4F4(W06ScaleSprite *work, void *sprite, s16 x, s16 y, BOOL flip);
BOOL ov07_0222DBD8(W06PicPulse *work);
BOOL ov07_0222DC54(W06SpritePulse *work);

#endif
