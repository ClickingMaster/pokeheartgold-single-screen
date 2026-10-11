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
extern void *BattleAnimUtil_AllocWork(void *, int);
extern int ov07_0221C514(void *);
extern int ov07_0221C4A0(void *);

extern u32 ov07_0221C4A8(void *p, int i);
extern void *ov07_0221C410(void *, void (*)(void *, void *), void *);
extern void BattleAnimUtil_InitCommonWork(void *, void *);
extern void *ov07_0221C4E8(void *, int);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern void ManagedSprite_SetMosaicFlag(void *, int);
extern void ov07_02225DC4(void *, void *);
extern void ov07_02225F18(void *, MosaicCallbackWork *);
extern void ManagedSprite_TickFrame(void *);
extern void SpriteSystem_DrawSprites(void *);

void ov07_02225F94(void *owner) {
    MosaicCallbackWork *work = BattleAnimUtil_AllocWork(owner, sizeof(MosaicCallbackWork));
    int index;

    BattleAnimUtil_InitCommonWork(owner, work);
    work->delta = ((s32)ov07_0221C4A8(work->owner, 1));
    work->target = ((s32)ov07_0221C4A8(work->owner, 2));
    work->unused28 = ((s32)ov07_0221C4A8(work->owner, 3));
    index = ((s32)ov07_0221C4A8(work->owner, 0));
    work->sprite = ov07_0221C4E8(work->owner, index);
    ManagedSprite_SetDrawFlag(work->sprite, TRUE);
    work->mosaic = work->delta < 0 ? 0 : 15;
    ManagedSprite_SetMosaicFlag(work->sprite, TRUE);
    ov07_02225F18(ov07_0221C410(work->owner, (void (*)(void *, void *))ov07_02225F18, work), work);
}
