#include "global.h"

#pragma require_prototypes off

typedef struct CallbackWork {
    u8 state;
    u8 filler_01[3];
    void *owner;
    u8 filler_08[0x14];
    s16 delta;
    u8 filler_1E[2];
    int interval;
    int counter;
    u8 filler_28[8];
    void *pokepic;
    u8 filler_34[8];
    int position;
    int limit;
    int accum;
    int attr;
} CallbackWork;

typedef struct ListNode {
    struct ListNode *next;
    u8 filler_04[0x20];
    u16 value1;
    u16 value2;
} ListNode;

typedef struct ListOwner {
    u8 filler_00[8];
    ListNode *list;
} ListOwner;

typedef struct BgCallbackWork {
    u8 state;
    u8 count;
    u8 filler_02[2];
    int arg0;
    int arg1;
    int arg2;
    int arg3;
    u8 filler_14[4];
    int limit;
    int mode;
    int active0;
    int active1;
    void *bgConfig;
    void *owner;
    void *resource;
    u8 filler_34[0xA0];
    u8 interpolation[0x24];
} BgCallbackWork;

typedef struct MosaicCallbackWork {
    u8 filler_00[4];
    void *owner;
    u8 filler_08[4];
    void *spriteSystem;
    u8 filler_10[0xC];
    void *sprite;
    s16 delta;
    u8 filler_22[2];
    int target;
    int unused28;
    int mosaic;
} MosaicCallbackWork;

extern void Pokepic_SetAttr(void *, int, int);
extern void Pokepic_SetVisible(void *, int, int, int, int);
extern void ov07_02223224(void *);
extern void ov07_0221C448(void *, void *);
extern void Heap_Free(void *);
extern void *ov07_022324D8(void *, int);
extern int ov07_0221C514(void *);
extern int ov07_0221C4A0(void *);

extern u32 ov07_0221C4A8(void *p, int i);
extern void *ov07_0221C410(void *, void (*)(void *, void *), void *);
extern void ov07_02231FE4(void *, void *);
extern void *ov07_0221C4E8(void *, int);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern void ManagedSprite_SetMosaicFlag(void *, int);
extern void ov07_02225DC4(void *, void *);
extern void ov07_02225F18(void *, MosaicCallbackWork *);
extern void ManagedSprite_TickFrame(void *);
extern void SpriteSystem_DrawSprites(void *);

void ov07_02225B38(void *task, CallbackWork *work) {
    switch (work->state) {
    case 0:
        work->counter++;
        if (work->counter < work->interval) {
            break;
        }
        work->counter = 0;
        work->accum += work->delta;
        Pokepic_SetAttr(work->pokepic, 1, work->accum);
        work->position -= work->delta;
        if (work->position > work->limit) {
            work->position = work->limit;
            work->state++;
        }
        Pokepic_SetAttr(work->pokepic, 0x12, work->position);
        break;
    case 1:
        Pokepic_SetAttr(work->pokepic, 0xe, 0);
        Pokepic_SetAttr(work->pokepic, 1, work->attr);
        Pokepic_SetAttr(work->pokepic, 0x12, work->limit);
        ov07_02223224(work->pokepic);
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
        break;
    }
}

void ov07_02225BC4(void *task, CallbackWork *work) {
    switch (work->state) {
    case 0:
        work->counter++;
        if (work->counter < work->interval) {
            break;
        }
        work->counter = 0;
        work->accum += work->delta;
        Pokepic_SetAttr(work->pokepic, 1, work->accum);
        work->position -= work->delta;
        if (work->position < 0) {
            work->position = 0;
            work->state++;
        }
        Pokepic_SetAttr(work->pokepic, 0x12, work->position);
        break;
    case 1:
        Pokepic_SetAttr(work->pokepic, 0xe, 0);
        Pokepic_SetAttr(work->pokepic, 1, work->attr);
        Pokepic_SetAttr(work->pokepic, 0x12, work->limit);
        ov07_02223224(work->pokepic);
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
        break;
    }
}

void ov07_02225C50(void *task, CallbackWork *work) {
    switch (work->state) {
    case 0:
        work->counter++;
        if (work->counter < work->interval) {
            break;
        }
        work->counter = 0;
        work->position -= work->delta;
        work->limit += work->delta;
        if (work->position < 0) {
            work->position = 0;
            work->state++;
        }
        Pokepic_SetVisible(work->pokepic, 0, work->position, 0x50, work->limit);
        break;
    case 1:
        Pokepic_SetAttr(work->pokepic, 0xe, 0);
        ov07_02223224(work->pokepic);
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
        break;
    }
}
