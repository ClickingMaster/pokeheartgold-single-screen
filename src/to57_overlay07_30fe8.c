#include "global.h"

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
    u8 motion[0x28];
} Work30F98;

typedef struct Ov07Init {
    int value0;
    int value4;
    int value8;
    s16 xVel;
    s16 yVel;
    s16 x;
    s16 y;
    s16 alphaA;
    s16 alphaB;
} Ov07Init;

extern int ov07_0221C4A8(void *, int);
extern void *ov07_022324D8(void *, int);
extern void ov07_02231FE4(void *, void *);
extern void *ov07_0221C4E8(void *, int);
extern int ov07_0221FB04(void *, int);
void ov07_022310E0(void *, void *, void *, void *);
extern void *ov07_0221C468(void *);
extern void *ov07_0221C470(void *);
extern int ov07_02231924(void *, void *);
extern int ov07_0221BFC0(void *);
extern void ManagedSprite_SetDrawPriority(void *, int);
extern void ManagedSprite_SetPriority(void *, int);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern void ManagedSprite_SetOamMode(void *, int);
extern void ov07_0221C410(void *, void (*)(void *, void *), void *);
extern void ov07_022315C0(void *, void *);
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
void ov07_0223174C(void *, const Ov07Init *);
void ov07_02231874(void *);
void ov07_022318A0(void *);
void ov07_022318CC(void *);
void ov07_022318F8(void *);

extern void ov07_02222AC4(void *, int, int, int, int, int);
extern BOOL ov07_02222AF4(void *);
extern void ManagedSprite_SetAnimateFlag(void *, int);
extern void ManagedSprite_SetAnimSpeed(void *, int);
extern u16 ManagedSprite_GetAnimationFrame(void *);
extern void ManagedSprite_SetPaletteOverrideOffset(void *, int);
extern BOOL ManagedSprite_IsAnimated(void *);
extern void ov07_02231E08(void *, int, int);

void ov07_022310E0(void *, void *, void *, void *);
extern void *ov07_0221C468(void *);
extern int ov07_02222004(void *, void *);
extern void ov07_0221F9E8(void *, void *);
extern int ov07_02221F80(void *, void *, int);
extern void *SpriteSystem_NewSprite(void *, void *, const void *);

void ov07_02230FE8(void *task, void *work_) {
    Work30F98 *work = work_;
    switch (work->state) {
    case 0:
        ov07_02222AC4(work->motion, 0, 16, 20, 4, 10);
        ManagedSprite_SetDrawFlag(work->sprite, 1);
        ov07_02231E08(work->owner, 0, 20);
        work->state++;
        break;
    case 1:
        if (ov07_02222AF4(work->motion)) {
            work->state++;
            ManagedSprite_SetAnimateFlag(work->sprite, 1);
            ManagedSprite_SetAnimSpeed(work->sprite, 1 << 12);
        }
        break;
    case 2:
        ManagedSprite_SetPaletteOverrideOffset(work->sprite, work->palette + ManagedSprite_GetAnimationFrame(work->sprite) % 3);
        if (!ManagedSprite_IsAnimated(work->sprite)) {
            work->state++;
            ov07_02222AC4(work->motion, 16, 0, 4, 20, 8);
        }
        break;
    case 3:
        if (ov07_02222AF4(work->motion)) {
            work->state++;
            ManagedSprite_SetDrawFlag(work->sprite, 0);
        }
        break;
    case 4:
        Sprite_DeleteAndFreeResources(work->sprite);
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteSystem);
}

void ov07_022310E0(void *owner, void *spriteSystem, void *spriteManager, void *firstSprite) {
    u8 template[0x34];
    Work310E0 *work = ov07_022324D8(owner, sizeof(Work310E0));
    void *object;
    int multiplier;
    int i;
    work->owner = owner;
    work->spriteSystem = spriteManager;
    object = ov07_0221C468(work->owner);
    multiplier = ov07_02222004(owner, object);
    ov07_0221F9E8(&template, work->owner);
    *(s16 *)&template[0] = ov07_02221F80(owner, object, 0);
    *(s16 *)&template[2] = ov07_02221F80(owner, object, 1);
    for (i = 5; i >= 0; i--) {
        if (i == 5) {
            work->sprites[i] = firstSprite;
            ManagedSprite_SetPositionXY(work->sprites[i], *(s16 *)&template[0], *(s16 *)&template[2]);
        } else {
            work->sprites[i] = SpriteSystem_NewSprite(spriteSystem, spriteManager, template);
        }
        ManagedSprite_SetDrawPriority(work->sprites[i], 100);
        ManagedSprite_SetPriority(work->sprites[i], 1);
        ManagedSprite_SetDrawFlag(work->sprites[i], 0);
        ov07_022311B0(work->sprites[i], &work->hide[i], &work->show[i], i, multiplier);
    }
    ov07_0221C410(work->owner, (void (*)(void *, void *))ov07_02231234, work);
}

void ov07_022311B0(void *sprite, int *a, int *b, int index, int mul) {
    s16 x, y;
    ManagedSprite_SetAnim(sprite, (index + (int)((u32)index >> 31)) >> 1);
    *b = index * 2;
    *a = 16;
    ManagedSprite_GetPositionXY(sprite, &x, &y);
    {
        int delta;
        delta = 2;
        delta = (delta - 14) * index + 40;
        x += mul * delta;
    }
    y += 40;
    ManagedSprite_SetPositionXY(sprite, x, y);
}
