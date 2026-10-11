#include "global.h"
#include "overlay07_obj_window_init_private.h"

typedef struct Ov07Args {
    int value0;
    int value4;
    int value8;
    s16 xVel;
    s16 yVel;
    s16 x;
    s16 y;
    s16 alphaA;
    s16 alphaB;
    u8 state18;
    u8 pad19[3];
    void *task1C;
    u8 pad20[4];
    void *spriteSystem;
    u8 pad28[4];
    void *bgConfig;
    void *palette;
    void *sprites[3];
} Ov07Args;

typedef struct Work310E0 {
    void *owner;
    int state;
    void *spriteSystem;
    void *sprites[6];
    int hide[6];
    int show[6];
} Work310E0;

typedef struct Work30F98 {
    void *owner;
    int state;
    int palette;
    void *spriteSystem;
    void *sprite;
} Work30F98;

extern u32 ov07_0221C4A8(void *p, int i);
extern void *BattleAnimUtil_AllocWork(void *, int);
extern void BattleAnimUtil_InitCommonWork(void *, void *);
extern void *ov07_0221C4E8(void *, int);
extern int ov07_0221FB04(void *, int);
extern void *ov07_0221C468(void *);
extern void *ov07_0221C470(void *);
extern int BattleAnimUtil_GetBattlerType(void *, void *);
extern int ov07_0221BFC0(void *);
extern void ManagedSprite_SetDrawPriority(void *, int);
extern void ManagedSprite_SetPriority(void *, int);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern void ManagedSprite_SetOamMode(void *, int);
extern void ov07_0221C410(void *, void (*)(void *, void *), void *);
extern void ov07_022222F0(void *, void *);
extern int ov07_02222748(void *, void *);
extern void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
extern void ManagedSprite_SetPositionXY(void *, s16, s16);
extern void ManagedSprite_SetAnim(void *, int);
extern int ManagedSprite_GetPaletteOverrideOffset(void *);
extern void ManagedSprite_SetAffineOverwriteMode(void *, int);
extern void ov07_02222268(void *, s16, s16, int, int);
extern void ov07_02222508(void *, int, int, int, int);
extern void ov07_02230FE8(void *, void *);
extern void Sprite_DeleteAndFreeResources(void *);
extern void SpriteSystem_DrawSprites(void *);
extern void ov07_0221C448(void *, void *);
extern void Heap_Free(void *);

void ov07_02230F0C(void *, void *, void *, int);
void ov07_02230F98(void *, void *, void *, void *);
int ov07_02230F6C(void *, void *, void *);
void ov07_022311B0(void *, int *, int *, int, int);
int ov07_02231204(void *, int *, int *);
void ov07_02231234(void *, Work310E0 *);
void ov07_02231514(Ov07Init *, s16, s16);
void ov07_0223151C(Ov07Init *, s16, s16);
void ov07_0223172C(void *, Ov07Init *);
void ov07_02231874(void *);
void ov07_022318A0(void *);
void ov07_022318CC(void *);
void ov07_022318F8(void *);

int ov07_02230F6C(void *sprite, void *a, void *b) {
    ov07_022222F0(a, sprite);
    if (ov07_02222748(b, sprite) != 0) {
        return 0;
    }
    ManagedSprite_SetDrawFlag(sprite, 0);
    return 1;
}
