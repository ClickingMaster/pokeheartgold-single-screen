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
                    ov07_0221C7B8(work->prefix.owner, temp, 3);
                    if (work->index == 2) {
                        ov07_0223494C(temp, ov07_0221BFD0(work->prefix.owner));
                    } else {
                        ov07_0223475C(temp, ov07_0221BFD0(work->prefix.owner));
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
        ov07_0221C448(work->prefix.owner, task);
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
    B8Work *work = ov07_022324D8(owner, sizeof(B8Work));
    ov07_02231FE4(owner, work);
    first = ov07_0221C468(work->prefix.owner);
    ov07_02231A50(work->prefix.owner, ov07_0221C468(work->prefix.owner), work->battlerXY[0]);
    ov07_02231A50(work->prefix.owner, ov07_0221C470(work->prefix.owner), work->battlerXY[1]);
    if (ov07_0223192C(work->prefix.owner, first) == 3) {
        work->animation.sign = 1;
    } else {
        work->animation.sign = -1;
    }
    value1 = ov07_0223192C(work->prefix.owner, ov07_0221C468(work->prefix.owner));
    value2 = ov07_0223192C(work->prefix.owner, ov07_0221C470(work->prefix.owner));
    work->same = value1 == value2;
    work->index = 0;
    ov07_02232020(owner, (ov07_0221C4A8(owner, 0)), &work->slot, &out);
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
    ov07_0221C410(work->prefix.owner, (void *)ov07_0222B898, work);
}
