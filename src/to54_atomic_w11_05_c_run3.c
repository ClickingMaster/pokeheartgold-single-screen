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

void ov07_0222B2A8(void *task, A8Work *work) {
    int i;
    int hidden = 0;
    for (i = 0; i < 8; i++) {
        work->timer[i]++;
        if (work->timer[i] >= 32) {
            ov07_0222A8D8(work->sprite[i + 2]);
            if (work->timer[i] >= 160) {
                ManagedSprite_SetDrawFlag(work->sprite[i + 2], FALSE);
                hidden++;
            }
        }
    }
    if (hidden == 8) {
        for (i = 0; i < 8; i++) {
            Sprite_DeleteAndFreeResources(work->sprite[i + 2]);
        }
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
    } else {
        SpriteSystem_DrawSprites(work->spriteSystem);
    }
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

void ov07_0222B898(void *task, B8Work *work);
void ov07_0222B988(void *owner);
