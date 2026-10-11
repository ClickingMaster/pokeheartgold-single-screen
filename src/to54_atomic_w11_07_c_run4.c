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

typedef struct B8Animation {
    W02LinearMotion motion;
    u8 reserved[12];
    s16 sign;
    u8 trailing[2];
} B8Animation;
typedef struct B8Work {
    Ov07T10Work prefix;
    Ov07T10Slot slot;
    B8Animation animation;
    u32 same;
    s16 battlerXY[2][2];
    u8 pad64[4];
    int index;
    int yStart[3];
    int yEnd[3];
    int attr[3];
} B8Work;

void ov07_0222B898(SysTask *task, void *data);
void ov07_0222B898(SysTask *task, void *data) {
    B8Work *work = data;
    u8 temp[0x50];
    switch (work->prefix.state[0]) {
    case 0:
        ov07_02222268(&work->animation.motion, work->slot.xy[0], (s16)work->yStart[work->index], (s16)work->yEnd[work->index], (s16)work->yEnd[work->index], 15);
        Pokepic_SetAttr(work->slot.pokepic, 2, work->attr[work->index]);
        Pokepic_SetAttr(work->slot.pokepic, 44, 1);
        work->prefix.state[0]++;
    case 1:
        if (!ov07_022222B4(&work->animation.motion)) {
            work->index++;
            if (work->index > 2) {
                work->prefix.state[0]++;
            } else {
                if (!work->same) {
                    BattleAnimSystem_BuildBattlerPicParams(work->prefix.owner, temp, 3);
                    if (work->index == 2) {
                        ov07_0223494C(temp, BattleAnimSystem_GetHeapID(work->prefix.owner));
                    } else {
                        ov07_0223475C(temp, BattleAnimSystem_GetHeapID(work->prefix.owner));
                    }
                }
                work->prefix.state[0] = 0;
            }
            work->slot.xy[0] = Pokepic_GetAttr(work->slot.pokepic, 0);
        }
        Pokepic_SetAttr(work->slot.pokepic, 0, work->animation.motion.x);
        Pokepic_SetAttr(work->slot.pokepic, 1, work->animation.motion.y);
        break;
    default:
        Pokepic_SetAttr(work->slot.pokepic, 44, 0);
        BattleAnimSystem_DestroyTask(work->prefix.owner, task);
        Heap_Free(work);
        break;
    }
}

void ov07_0222B988(void *owner);
void ov07_0222B988(void *owner) {
    int out;
    u32 first;
    u32 value1;
    u32 value2;
    int delta;
    B8Work *work = BattleAnimUtil_AllocWork(owner, sizeof(B8Work));
    BattleAnimUtil_InitCommonWork(owner, work);
    first = BattleAnimSystem_GetAttacker(work->prefix.owner);
    BattleAnimUtil_GetBattlerPosition(work->prefix.owner, BattleAnimSystem_GetAttacker(work->prefix.owner), work->battlerXY[0]);
    BattleAnimUtil_GetBattlerPosition(work->prefix.owner, BattleAnimSystem_GetDefender(work->prefix.owner), work->battlerXY[1]);
    if (BattleAnimUtil_GetBattlerSide(work->prefix.owner, first) == 3) {
        work->animation.sign = 1;
    } else {
        work->animation.sign = -1;
    }
    value1 = BattleAnimUtil_GetBattlerSide(work->prefix.owner, BattleAnimSystem_GetAttacker(work->prefix.owner));
    value2 = BattleAnimUtil_GetBattlerSide(work->prefix.owner, BattleAnimSystem_GetDefender(work->prefix.owner));
    work->same = value1 == value2;
    work->index = 0;
    BattleAnimUtil_CollectPicsByFlags(owner, (BattleAnimSystem_GetArg(owner, 0)), &work->slot, &out);
    delta = Pokepic_GetAttr(work->slot.pokepic, 1) - work->battlerXY[0][1];
    if (work->animation.sign > 0) {
        work->yStart[0] = 335;
        work->yStart[1] = -80;
        work->yStart[2] = work->battlerXY[0][0];
        work->yEnd[0] = work->battlerXY[0][1] + delta;
        work->yEnd[1] = work->battlerXY[1][1] + delta;
        work->yEnd[2] = work->battlerXY[0][1] + delta;
        work->attr[0] = 10;
        work->attr[1] = -650;
        work->attr[2] = Pokepic_GetAttr(work->slot.pokepic, 2);
    } else {
        work->yStart[0] = -80;
        work->yStart[1] = 335;
        work->yStart[2] = work->battlerXY[0][0];
        work->yEnd[0] = work->battlerXY[0][1] + delta;
        work->yEnd[1] = work->battlerXY[1][1] + delta;
        work->yEnd[2] = work->battlerXY[0][1] + delta;
        work->attr[0] = -650;
        work->attr[1] = 10;
        work->attr[2] = Pokepic_GetAttr(work->slot.pokepic, 2);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->prefix.owner, (void *)ov07_0222B898, work);
}
