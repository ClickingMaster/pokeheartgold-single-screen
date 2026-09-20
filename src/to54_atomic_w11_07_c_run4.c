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

void ov07_0222B898(void *task, B8Work *work);
void ov07_0222B898(void *task, B8Work *work) {
    u8 temp[0x50];
    switch (work->state) {
    case 0:
        ov07_02222268(work->anim, work->x, (s16)work->yStart[work->index], (s16)work->yEnd[work->index], (s16)work->yEnd[work->index], 15);
        Pokepic_SetAttr(work->pokepic, 2, work->attr[work->index]);
        Pokepic_SetAttr(work->pokepic, 44, 1);
        work->state++;
    case 1:
        if (!ov07_022222B4(work->anim)) {
            work->index++;
            if (work->index > 2) {
                work->state++;
            } else {
                if (!work->same) {
                    ov07_0221C7B8(work->owner, temp, 3);
                    if (work->index == 2) {
                        ov07_0223494C(temp, ov07_0221BFD0(work->owner));
                    } else {
                        ov07_0223475C(temp, ov07_0221BFD0(work->owner));
                    }
                }
                work->state = 0;
            }
            work->x = Pokepic_GetAttr(work->pokepic, 0);
        }
        Pokepic_SetAttr(work->pokepic, 0, *(s16 *)&work->anim[0]);
        Pokepic_SetAttr(work->pokepic, 1, *(s16 *)&work->anim[2]);
        break;
    default:
        Pokepic_SetAttr(work->pokepic, 44, 0);
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
        break;
    }
}

void ov07_0222B988(void *owner);
void ov07_0222B988(void *owner) {
    u32 out;
    u32 first;
    u32 value1;
    u32 value2;
    int delta;
    B8Work *work = ov07_022324D8(owner, sizeof(B8Work));
    ov07_02231FE4(owner, work);
    first = ov07_0221C468(work->owner);
    ov07_02231A50(work->owner, ov07_0221C468(work->owner), (s16 *)work + 0x2E);
    ov07_02231A50(work->owner, ov07_0221C470(work->owner), (s16 *)work + 0x30);
    work->field1C = work->field1C;
    if (ov07_0223192C(work->owner, first) == 3) {
        *(s16 *)((u8 *)work + 0x54) = 1;
    } else {
        *(s16 *)((u8 *)work + 0x54) = -1;
    }
    value1 = ov07_0223192C(work->owner, ov07_0221C468(work->owner));
    value2 = ov07_0223192C(work->owner, ov07_0221C470(work->owner));
    work->same = value1 == value2;
    work->index = 0;
    ov07_02232020(owner, ov07_0221C4A8(owner, 0), &work->field1C, &out);
    delta = Pokepic_GetAttr(work->pokepic, 1) - *(s16 *)((u8 *)work + 0x5E);
    if (*(s16 *)((u8 *)work + 0x54) > 0) {
        work->yStart[0] = 335;
        work->yStart[1] = -80;
        work->yStart[2] = *(s16 *)((u8 *)work + 0x5C);
        work->yEnd[0] = *(s16 *)((u8 *)work + 0x5E) + delta;
        work->yEnd[1] = *(s16 *)((u8 *)work + 0x62) + delta;
        work->yEnd[2] = *(s16 *)((u8 *)work + 0x5E) + delta;
        work->attr[0] = 10;
        work->attr[1] = -650;
        work->attr[2] = Pokepic_GetAttr(work->pokepic, 2);
    } else {
        work->yStart[0] = -80;
        work->yStart[1] = 335;
        work->yStart[2] = *(s16 *)((u8 *)work + 0x5C);
        work->yEnd[0] = *(s16 *)((u8 *)work + 0x5E) + delta;
        work->yEnd[1] = *(s16 *)((u8 *)work + 0x62) + delta;
        work->yEnd[2] = *(s16 *)((u8 *)work + 0x5E) + delta;
        work->attr[0] = -650;
        work->attr[1] = 10;
        work->attr[2] = Pokepic_GetAttr(work->pokepic, 2);
    }
    ov07_0221C410(work->owner, (void (*)(void))ov07_0222B898, work);
}
