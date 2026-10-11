#include "global.h"

#pragma require_prototypes off

typedef struct Ov07Motion {
    u8 data[0x24];
} Ov07Motion;

typedef struct Ov07SpriteTask {
    void *owner;
    void *spriteSystem;
    Ov07Motion motion;
    void *sprite;
} Ov07SpriteTask;

typedef struct Ov07PicTaskA {
    void *owner;
    u32 state;
    s32 timer;
    s32 column;
    s32 row;
    void *pic;
    s16 x;
    s16 y;
    Ov07Motion motion;
    Ov07Motion motion2;
} Ov07PicTaskA;

typedef struct Ov07PicTaskB {
    void *owner;
    u32 state;
    s32 column;
    s32 row;
    void *pic;
    s16 x;
    s16 y;
    Ov07Motion motion;
    Ov07Motion motion2;
} Ov07PicTaskB;

extern void *BattleAnimUtil_AllocWork(void *, u32);
extern void *ov07_0221C528(void *);
extern u16 BattleAnimSystem_GetAttacker(void *);
extern int BattleAnimUtil_GetBattlerSide(void *, int);
extern void *BattleAnimSystem_GetSprite(void *, int);
extern void Sprite_DeleteAndFreeResources(void *);
extern s32 ov07_02222004(void *, int);
extern s32 BattleAnimUtil_GetBattlerSideSign(void *, int);
extern s32 BattleAnimSystem_GetBattlerScreenCoord(void *, int, int);
extern void ManagedSprite_SetPositionXY(void *, s16, s16);
extern void BattleAnimUtil_InitLinearMotion(void *, s16, s16, s16, s16, int);
extern void BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
extern void BattleAnimUtil_InitShake(void *, int, int, int, int);
extern BOOL BattleAnimUtil_UpdateShakePokepic(void *, int, int, void *);
extern BOOL BattleAnimUtil_UpdateLinearMotionPokepic(void *, void *);
extern void Pokepic_SetAttr(void *, int, int);
extern int Pokepic_GetAttr(void *, int);
extern void Heap_Free(void *);
extern void BattleAnimSystem_DestroyTask(void *, void *);
extern void *BattleAnimSystem_GetBattlerPokepic(void *, int);

void ov07_02230960(void *, void *);
void ov07_02230A70(void *, Ov07PicTaskA *);
void ov07_02230BF8(void *, Ov07PicTaskB *);

void ov07_02230994(void *owner) {
    Ov07SpriteTask *work = BattleAnimUtil_AllocWork(owner, sizeof(Ov07SpriteTask));
    u16 context;

    work->owner = owner;
    work->spriteSystem = ov07_0221C528(owner);
    context = BattleAnimSystem_GetAttacker(owner);
    if (BattleAnimUtil_GetBattlerSide(owner, context) == 3) {
        work->sprite = BattleAnimSystem_GetSprite(owner, 0);
        Sprite_DeleteAndFreeResources(BattleAnimSystem_GetSprite(owner, 1));
    } else {
        work->sprite = BattleAnimSystem_GetSprite(owner, 1);
        Sprite_DeleteAndFreeResources(BattleAnimSystem_GetSprite(owner, 0));
    }
    {
        int row = ov07_02222004(owner, context);
        int column = BattleAnimUtil_GetBattlerSideSign(owner, context);
        int xBase = BattleAnimSystem_GetBattlerScreenCoord(owner, context, 0);
        int yBase = BattleAnimSystem_GetBattlerScreenCoord(owner, context, 1);
        int x = xBase + row * 0x40;
        int deltaY = -(column * 0x10);
        int y = yBase + deltaY;
        ManagedSprite_SetPositionXY(work->sprite, x, y);
        BattleAnimUtil_InitLinearMotion(&work->motion, x, x + row * 0x30, y, y + deltaY, 6);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_02230960, work);
}

void ov07_02230A70(void *task, Ov07PicTaskA *work) {
    switch (work->state) {
    case 0:
        BattleAnimUtil_InitShake(&work->motion2, 4, 0, 1, 4);
        work->state++;
        break;
    case 1:
        if (!BattleAnimUtil_UpdateShakePokepic(&work->motion2, work->x, work->y, work->pic)) {
            int delta = 0x18;
            work->state++;
            BattleAnimUtil_InitLinearMotion(&work->motion, work->x, work->x + work->column * 0x28, work->y, work->y + work->row * (delta - 0x1F), 4);
        }
        break;
    case 2:
        if (!BattleAnimUtil_UpdateLinearMotionPokepic(&work->motion, work->pic)) {
            work->state++;
            work->timer = 8;
        }
        break;
    case 3:
        if (--work->timer < 0) {
            int delta = 0x28;
            work->state++;
            BattleAnimUtil_InitLinearMotion(&work->motion, work->x + work->column * delta, work->x, work->y + work->row * (delta - 0x2F), work->y, 4);
        }
        break;
    case 4:
        if (!BattleAnimUtil_UpdateLinearMotionPokepic(&work->motion, work->pic)) {
            work->state++;
        }
        break;
    case 5:
        Pokepic_SetAttr(work->pic, 0, work->x);
        Pokepic_SetAttr(work->pic, 1, work->y);
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->owner, task);
        break;
    }
}

void ov07_02230BA0(void *owner) {
    Ov07PicTaskA *work = BattleAnimUtil_AllocWork(owner, 0x64);
    u16 context;

    work->owner = owner;
    context = BattleAnimSystem_GetAttacker(owner);
    work->pic = BattleAnimSystem_GetBattlerPokepic(owner, context);
    work->x = Pokepic_GetAttr(work->pic, 0);
    work->y = Pokepic_GetAttr(work->pic, 1);
    work->column = ov07_02222004(owner, context);
    work->row = BattleAnimUtil_GetBattlerSideSign(owner, context);
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_02230A70, work);
}

void ov07_02230BF8(void *task, Ov07PicTaskB *work) {
    switch (work->state) {
    case 0: {
        int duration = 4;
        BattleAnimUtil_InitLinearMotion(&work->motion, work->x, work->x + work->column * (duration - 44), work->y, work->y + work->row * 0x10, duration);
        work->state++;
        break;
    }
    case 1:
        if (!BattleAnimUtil_UpdateLinearMotionPokepic(&work->motion, work->pic)) {
            work->state++;
            BattleAnimUtil_InitShake(&work->motion2, 4, 0, 1, 4);
        }
        break;
    case 2: {
        s16 x = Pokepic_GetAttr(work->pic, 0);
        s16 y = Pokepic_GetAttr(work->pic, 1);
        if (!BattleAnimUtil_UpdateShakePokepic(&work->motion2, x, y, work->pic)) {
            int duration = 4;
            work->state++;
            BattleAnimUtil_InitLinearMotion(&work->motion, work->x + work->column * (duration - 44), work->x, work->y + work->row * 0x10, work->y, duration);
        }
        break;
    }
    case 3:
        if (!BattleAnimUtil_UpdateLinearMotionPokepic(&work->motion, work->pic)) {
            work->state++;
        }
        break;
    case 4:
        Pokepic_SetAttr(work->pic, 0, work->x);
        Pokepic_SetAttr(work->pic, 1, work->y);
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->owner, task);
        break;
    }
}
