#include "to54_atomic_w11_private.h"

typedef struct A8Work {
    u8 pad00[4];
    void *owner;
    void *spriteManager;
    u8 pad0C[4];
    SpriteSystem *spriteSystem;
    u8 pad14[8];
    s16 delay;
    s16 timer[4];
    u8 pad26[2];
    ManagedSprite *sprite[4];
} A8Work;

void ov07_0222A8EC(void *task, A8Work *work);
void ov07_0222A988(void *, int, int, ManagedSprite *);
void ov07_0222B2A8(void *task, A8Work *work);

typedef struct AEWork {
    u8 state;
    u8 pad01[3];
    void *owner;
    u8 pad08[4];
    SpriteSystem *spriteSystem;
    u8 pad10[0x14];
    Pokepic *pokepic;
    ManagedSprite *sprite0;
    u8 pad2C[0x10];
    ManagedSprite *sprite1;
    u8 pad40[0x10];
    ManagedSprite *sprite2;
    u8 pad54[4];
    s16 attr;
    s16 species;
    s16 attrCopy;
    u8 pad5E[2];
    u8 anim[0x14];
    u32 field74;
    u8 pad78[0x0C];
} AEWork;

void ov07_0222AE14(void *task, AEWork *work);
void ov07_0222AEA0(void *owner);

void ov07_0222AE14(void *task, AEWork *work) {
    s32 x;
    s32 y;
    if (work->state == 0) {
        if (!ov07_0222260C(work->anim)) {
            ManagedSprite_SetDrawFlag(work->sprite0, FALSE);
            ManagedSprite_SetDrawFlag(work->sprite1, FALSE);
            ManagedSprite_SetDrawFlag(work->sprite1, FALSE);
            BattleAnimSystem_DestroyTask(work->owner, task);
            BattleAnimUtil_FreeWork(work);
            return;
        }
        ov07_02222644(work->anim, &x, &y);
        ManagedSprite_SetAffineScale(work->sprite0, x, y);
        ov07_022226FC(work->sprite0, work->attr, work->species, work->field74, 0);
        ManagedSprite_TickFrame(work->sprite0);
        ManagedSprite_TickFrame(work->sprite1);
        ManagedSprite_TickFrame(work->sprite2);
        SpriteSystem_DrawSprites(work->spriteSystem);
    }
}

void ov07_0222AEA0(void *owner) {
    u32 mode;
    AEWork *work = BattleAnimUtil_AllocWork(owner, sizeof(AEWork));
    BattleAnimUtil_InitCommonWork(owner, work);
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->owner, BattleAnimSystem_GetDefender(work->owner));
    work->attr = Pokepic_GetAttr(work->pokepic, 1);
    work->attrCopy = work->attr;
    work->species = ov07_0221FAA0(work->owner, BattleAnimSystem_GetDefender(work->owner));
    work->sprite0 = BattleAnimSystem_GetBattlerSprite(owner, 0);
    work->sprite1 = BattleAnimSystem_GetBattlerSprite(owner, 1);
    work->sprite2 = BattleAnimSystem_GetBattlerSprite(owner, 2);
    ManagedSprite_SetAffineOverwriteMode(work->sprite0, 2);
    ManagedSprite_SetPriority(work->sprite0, ov07_0221FAE8(owner));
    ManagedSprite_SetPriority(work->sprite1, ov07_0221FAE8(owner));
    ManagedSprite_SetPriority(work->sprite2, ov07_0221FAE8(owner));
    mode = BattleAnimSystem_GetBattlerType(work->owner, BattleAnimSystem_GetDefender(work->owner));
    if (mode == 3 || mode == 4) {
        ManagedSprite_SetDrawPriority(work->sprite0, 30);
        ManagedSprite_SetDrawPriority(work->sprite1, 50);
        ManagedSprite_SetDrawPriority(work->sprite2, 70);
    } else {
        ManagedSprite_SetDrawPriority(work->sprite0, 60);
        ManagedSprite_SetDrawPriority(work->sprite1, 70);
        ManagedSprite_SetDrawPriority(work->sprite2, 50);
    }
    ov07_02222590(work->anim, 100, 20, 100, 20, 100, 10);
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, (void *)ov07_0222AE14, work);
}

typedef struct B8Work {
    u8 state;
    u8 pad01[3];
    void *owner;
    u8 pad08[0x14];
    s16 field1C;
    s16 pad1E;
    s16 x;
    u8 pad22[2];
    Pokepic *pokepic;
    u8 pad28[8];
    u8 anim[0x28];
    u32 same;
    u8 pad5C[0x0C];
    int index;
    int yStart[3];
    int yEnd[3];
    int attr[3];
} B8Work;

void ov07_0222B898(SysTask *task, void *data);
void ov07_0222B988(void *owner);
