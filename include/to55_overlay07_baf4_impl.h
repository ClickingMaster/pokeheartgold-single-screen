#include "global.h"

#pragma require_prototypes off

typedef struct Ov07MosaicWork {
    int counter;
    int value4;
    int value8;
    int valueC;
    int step;
    int toggle;
    int savedAttr;
    void *pic;
    u8 state;
    u8 pad21[3];
    void *owner;
    u8 pad28[0x28];
} Ov07MosaicWork;

typedef struct Ov07MoveWork {
    void *owner;
    int state;
    s16 timer;
    u8 pad0A[2];
    void *pic;
    u8 movement[0x24];
    struct {
        int value;
        u8 pad04[4];
        int scale;
        u8 pad0C[8];
    } motion;
    s16 x;
    s16 y;
    u8 pad4C[0xA4];
} Ov07MoveWork;

typedef struct Ov07LargeWork {
    void *owner;
    void *spriteSystem;
    void *pic;
    struct {
        u8 pad00[0xC];
        int scale;
        u8 pad10[0x14];
    } move0;
    void *sprites[2];
    u8 move1[0xB0];
    s16 x;
    s16 y;
    s16 valueEC;
    u8 padEE[2];
} Ov07LargeWork;

typedef struct Ov07Copy9 {
    u32 v[9];
} Ov07Copy9;

typedef struct Ov07Copy5 {
    u32 v[5];
} Ov07Copy5;

typedef struct Ov07SpriteInit {
    u32 ignored0;
    Ov07Copy9 words04;
    Ov07Copy5 words28;
    s16 value3C;
    s16 value3E;
    s16 value40;
    u8 pad42[2];
    u32 value44;
} Ov07SpriteInit;

typedef struct Ov07Sprite {
    void *sprite;
    Ov07Copy9 words04;
    Ov07Copy5 words28;
    s16 value3C;
    s16 value3E;
    s16 value40;
    u8 pad42[2];
    u32 value44;
    u32 value48;
} Ov07Sprite;

extern void *BattleAnimUtil_AllocWork(void *, u32);
extern void BattleAnimUtil_InitCommonWork(void *, void *);
extern void *BattleAnimSystem_GetDefender(void *);
extern void *BattleAnimSystem_GetAttacker(void *);
extern void *BattleAnimSystem_GetBattlerSpriteManager(void *);
extern void *BattleAnimSystem_GetBattlerPokepic(void *, void *);
extern int BattleAnimSystem_GetHeapID(void *);
extern int ov07_02222004(void *, void *);
extern void *BattleAnimUtil_GetBattlerSide(void *, void *);
extern void *BattleAnimSystem_GetBattlerSprite(void *, int);
extern int BattleAnimUtil_GetBattlerType(void *, void *);
extern int ov07_0221FAE8(void *);
extern void BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
extern void BattleAnimSystem_DestroyTask(void *, void *);
extern int ov07_02222240(void *, s16, s16, void *);
extern int ov07_02222914(void *);
extern int ov07_022222B4(void *);
extern void ov07_022220B8(void *, void *, s16, s16);
extern void ov07_02222268(void *, s16, s16, int, int, int);
extern int ov07_02222440(void *);
extern void ov07_022223F0(void *, int, int, int);
extern void ov07_02222A44(void *, int, int);
extern void ov07_02222864(void *, void *, void *, int, int, int, int, int, void *, void *, int, int);
extern void BattleAnimUtil_SetBlendWithBattleBg(void *, int, int);
extern void ov07_02222180(void);
extern void *Heap_Alloc(int, int);
extern void *SpriteSystem_NewSprite(void *, void *, void *);
extern void SpriteSystem_DrawSprites(void *);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern void ManagedSprite_SetDrawPriority(void *, int);
extern void ManagedSprite_SetPriority(void *, int);
extern int Pokepic_GetAttr(void *, int);
extern void Pokepic_SetAttr(void *, int, int);
extern void Heap_Free(void *);
extern void SetBgPriority(int, int);

void ov07_0222BD90(void *task, Ov07LargeWork *work);
void ov07_0222BF18(void *task, Ov07MoveWork *work);

#if !defined(T04_ONLY) || defined(T04_BAF4)
void ov07_0222BAF4(void *task, Ov07MosaicWork *work) {
    u32 *dispcnt;
    vu16 *win0;
    vu16 *win1;

    switch (work->state) {
    case 0:
        work->step += 0x4F;
        if (work->step >= 0x50) {
            work->state++;
            work->toggle ^= 1;
            work->step = 0;
            return;
        }
        if (work->toggle == 0) {
            ov07_0222BADC(work->pic, 0, work->step, (work->counter + 2) * 2, 2);
        } else {
            ov07_0222BADC(work->pic, 0x50 - work->step, work->step, (work->counter + 2) * 2, 2);
        }
        return;
    case 1:
        work->value4--;
        work->valueC++;
        *(vu16 *)0x04000040 = ((work->value8 << 8) & 0xFF00) | (u8)(work->value8 + 0x50);
        *(vu16 *)0x04000044 = ((work->value4 << 8) & 0xFF00) | (u8)work->valueC;
        work->state++;
        return;
    case 2:
        work->value4--;
        work->valueC++;
        *(vu16 *)0x04000040 = ((work->value8 << 8) & 0xFF00) | (u8)(work->value8 + 0x50);
        *(vu16 *)0x04000044 = ((work->value4 << 8) & 0xFF00) | (u8)work->valueC;
        work->counter++;
        if (work->counter > 0x26) {
            work->state++;
        } else {
            work->state = 0;
        }
        return;
    case 3:
        Pokepic_SetAttr(work->pic, 0xE, 0);
        work->state++;
        return;
    }

    dispcnt = (u32 *)0x04000000;
    *dispcnt &= 0xFFFF1FFF;
    win0 = (vu16 *)0x04000048;
    win1 = (vu16 *)0x0400004A;
    *win0 &= ~0x3F;
    *win1 &= ~0x3F;
    *(vu16 *)0x04000040 = 0;
    *(vu16 *)0x04000044 = 0;
    BattleAnimSystem_DestroyTask(work->owner, task);
    Pokepic_SetAttr(work->pic, 0x17, work->savedAttr);
    Heap_Free(work);
}
#endif

#if !defined(T04_ONLY) || defined(T04_BC7C)
void ov07_0222BC7C(void *owner) {
    Ov07MosaicWork *work = BattleAnimUtil_AllocWork(owner, sizeof(Ov07MosaicWork));
    int kind;
    vu32 *base;

    BattleAnimUtil_InitCommonWork(owner, &work->state);
    work->pic = BattleAnimSystem_GetBattlerPokepic(owner, BattleAnimSystem_GetDefender(owner));
    work->counter = 0;
    kind = BattleAnimUtil_GetBattlerType(owner, BattleAnimSystem_GetDefender(owner));
    switch (kind) {
    case 3:
    case 4:
        SetBgPriority(2, (u8)(ov07_0221FAE8(owner) - 1));
        break;
    }
    work->value8 = (s16)Pokepic_GetAttr(work->pic, 0) - 0x28;
    work->value4 = (s16)Pokepic_GetAttr(work->pic, 1) - Pokepic_GetAttr(work->pic, 0x29) + 0x28;
    work->savedAttr = Pokepic_GetAttr(work->pic, 0x17);
    Pokepic_SetAttr(work->pic, 0x17, 8);
    work->step = 0;
    work->valueC = work->value4;
    work->toggle = 0;
    base = (vu32 *)0x04000000;
    base[0] = (base[0] & 0xFFFF1FFF) | 0x2000;
    *(vu16 *)((u8 *)base + 0x48) = (*(vu16 *)((u8 *)base + 0x48) & ~0x3F) | 0x3F;
    *(vu16 *)((u8 *)base + 0x4A) = (*(vu16 *)((u8 *)base + 0x4A) & ~0x3F) | 0x3B;
    *(vu16 *)((u8 *)base + 0x40) = ((work->value8 << 8) & 0xFF00) | (u8)(work->value8 + 0x50);
    *(vu16 *)((u8 *)base + 0x44) = ((work->value4 << 8) & 0xFF00) | (u8)work->valueC;
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_0222BAF4, work);
}
#endif

#if !defined(T04_ONLY) || defined(T04_BD90)
void ov07_0222BD90(void *task, Ov07LargeWork *work) {
    int results[3];
    int i;

    results[0] = ov07_02222240(&work->move0, work->x, work->y, work->pic);
    results[1] = ov07_02222914(work->move1);
    results[2] = 0;
    SpriteSystem_DrawSprites(work->spriteSystem);
    for (i = 0; i < 3; i++) {
        if (results[i] == 1) {
            return;
        }
    }
    Pokepic_SetAttr(work->pic, 0, work->x);
    Pokepic_SetAttr(work->pic, 1, work->y - 8);
    BattleAnimSystem_DestroyTask(work->owner, task);
    Heap_Free(work);
}
#endif

#if !defined(T04_ONLY) || defined(T04_BE00)
void ov07_0222BE00(void *owner) {
    Ov07LargeWork *work;
    struct {
        u8 pad00[0xC];
        int scale;
        u8 pad10[0x14];
    } localMove;
    int multiplier;
    int i;

    work = Heap_Alloc(BattleAnimSystem_GetHeapID(owner), 0xF0);
    work->owner = owner;
    work->spriteSystem = BattleAnimSystem_GetBattlerSpriteManager(work->owner);
    work->pic = BattleAnimSystem_GetBattlerPokepic(work->owner, BattleAnimSystem_GetAttacker(work->owner));
    work->x = Pokepic_GetAttr(work->pic, 0);
    work->y = Pokepic_GetAttr(work->pic, 1);
    work->valueEC = Pokepic_GetAttr(work->pic, 0x29);
    work->y += 8;
    ov07_02222A44(&work->move0, 1, 8);
    ov07_02222A44(&localMove, 1, 10);
    multiplier = ov07_02222004(work->owner, BattleAnimSystem_GetAttacker(work->owner));
    work->move0.scale *= multiplier;
    localMove.scale *= multiplier;
    for (i = 0; i < 2; i++) {
        work->sprites[i] = BattleAnimSystem_GetBattlerSprite(work->owner, i);
        ManagedSprite_SetDrawPriority(work->sprites[i], 100);
        ManagedSprite_SetPriority(work->sprites[i], 1);
        ManagedSprite_SetOamMode(work->sprites[i], 1);
    }
    ov07_02222864(work->move1, &localMove, ov07_02222180, work->x, (s16)(work->y - work->valueEC), 2, 2, 0, work->sprites[0], work->sprites[1], 0, 0);
    BattleAnimUtil_SetBlendWithBattleBg(work->owner, -1, -1);
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_0222BD90, work);
}
#endif

#if !defined(T04_ONLY) || defined(T04_BF18)
void ov07_0222BF18(void *task, Ov07MoveWork *work) {
    switch (work->state) {
    case 0:
        if (ov07_022222B4(work->movement)) {
            ov07_022220B8(work->movement, work->pic, work->x, work->y);
            return;
        }
        ov07_02222268(work->movement, *(s16 *)work->movement, 0, 0, 0, 2);
        work->state++;
        return;
    case 1:
        if (ov07_02222440(&work->motion.value)) {
            Pokepic_SetAttr(work->pic, 9, work->motion.value);
            return;
        }
        ov07_022223F0(&work->motion.value, work->motion.value, 0, 4);
        work->state++;
        work->timer = 2;
        return;
    case 2:
        if (--work->timer < 0) {
            work->state++;
        }
        return;
    case 3:
        if (ov07_022222B4(work->movement)) {
            ov07_022220B8(work->movement, work->pic, work->x, work->y);
            return;
        }
        work->state++;
        work->timer = 0x20;
        return;
    case 4:
        if (--work->timer < 0) {
            work->state++;
        }
        return;
    case 5:
        if (ov07_02222440(&work->motion.value)) {
            Pokepic_SetAttr(work->pic, 9, work->motion.value);
            return;
        }
        work->state++;
        return;
    case 6:
        Pokepic_SetAttr(work->pic, 0, work->x);
        Pokepic_SetAttr(work->pic, 1, work->y);
        Pokepic_SetAttr(work->pic, 9, 0);
        BattleAnimSystem_DestroyTask(work->owner, task);
        Heap_Free(work);
        return;
    }
}
#endif

#if !defined(T04_ONLY) || defined(T04_C074)
void ov07_0222C074(void *owner) {
    Ov07MoveWork *work;
    int multiplier;
    int kind0;
    int kind1;

    work = Heap_Alloc(BattleAnimSystem_GetHeapID(owner), 0xF0);
    work->owner = owner;
    work->state = 0;
    work->pic = BattleAnimSystem_GetBattlerPokepic(work->owner, BattleAnimSystem_GetAttacker(work->owner));
    work->x = Pokepic_GetAttr(work->pic, 0);
    work->y = Pokepic_GetAttr(work->pic, 1);
    ov07_02222268(work->movement, 0, -32, 0, 0, 3);
    ov07_022223F0(&work->motion.value, 0, 0xE38, 8);
    multiplier = ov07_02222004(work->owner, BattleAnimSystem_GetAttacker(work->owner));
    *(int *)(work->movement + 8) *= multiplier;
    work->motion.scale *= multiplier;
    multiplier = ov07_02222004(work->owner, BattleAnimSystem_GetDefender(work->owner));
    kind0 = (int)BattleAnimUtil_GetBattlerSide(owner, BattleAnimSystem_GetAttacker(work->owner));
    kind1 = (int)BattleAnimUtil_GetBattlerSide(owner, BattleAnimSystem_GetDefender(work->owner));
    if (kind0 == 3 && kind1 == 3) {
        Pokepic_SetAttr(work->pic, 0xA, -40);
        Pokepic_SetAttr(work->pic, 0xB, 40);
    } else if (kind0 == 4 && kind1 == 4) {
        Pokepic_SetAttr(work->pic, 0xA, 40);
        Pokepic_SetAttr(work->pic, 0xB, 40);
    } else {
        Pokepic_SetAttr(work->pic, 0xA, 40 * multiplier);
        Pokepic_SetAttr(work->pic, 0xB, 40);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_0222BF18, work);
}
#endif

#if !defined(T04_ONLY) || defined(T04_C18C)
void ov07_0222C18C(Ov07Sprite *out, void *spriteSystem, void *resource, void *position, const Ov07SpriteInit *init) {
    out->sprite = SpriteSystem_NewSprite(spriteSystem, resource, position);
    out->words04 = init->words04;
    out->words28 = init->words28;
    out->value3C = init->value3C;
    out->value3E = init->value3E;
    out->value40 = init->value40;
    out->value44 = init->value44;
    out->value48 = 0;
    ManagedSprite_SetDrawFlag(out->sprite, 0);
    ManagedSprite_SetDrawPriority(out->sprite, 0x64);
    ManagedSprite_SetPriority(out->sprite, 1);
}
#endif
