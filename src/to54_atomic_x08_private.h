#ifndef TO54_X08_PRIVATE_H
#define TO54_X08_PRIVATE_H
#include "global.h"
typedef struct X08Work {
    void *owner;
    u8 pad04[4];
    void *spriteSystem;
    int state;
    int index;
    u8 pad14[0x34];
    void *sprites[6];
    int animStates[6];
    int timers[6];
} X08Work;
void ov07_0222EDF0(void *);
BOOL ov07_0222EE14(void *);
void ov07_0222EF0C(void *, u32 *, fx32);
BOOL ov07_0222EF58(void *, u32 *);
void ManagedSprite_SetAnim(void *, u32);
void Sprite_DeleteAndFreeResources(void *);
void ov07_0221C448(void *, void *);
void Heap_Free(void *);
void SpriteSystem_DrawSprites(void *);
void ov07_0222EF88(void *, X08Work *);

typedef struct X08Work2 {
    void *owner;
    u8 pad04[4];
    void *spriteSystem;
    int state;
    void *pokepic;
    u8 anim0[0x24];
    u8 anim1[0x24];
    void *sprite;
    u8 movement[0x28];
    s16 x;
    s16 y;
} X08Work2;
void ManagedSprite_SetAnimateFlag(void *, BOOL);
BOOL ManagedSprite_IsAnimated(void *);
void ov07_02222508(void *, int, int, int, int);
int Pokepic_GetAttr(void *, int);
void ov07_02222268(void *, s16, s16, s16, s16, int);
void ov07_02222314(void *, void *);
BOOL ov07_02222768(void *, void *);
void Pokepic_SetAttr(void *, int, int);
void ManagedSprite_SetAnimSpeed(void *, fx32);
BOOL ov07_022222B4(void *);
void ManagedSprite_SetPositionXY(void *, s16, s16);
void ov07_0222F210(void *, X08Work2 *);
#endif
