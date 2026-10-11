#pragma force_active on
#include "global.h"
#include "heap.h"
#include "pokepic.h"
typedef struct Work {
    u8 state;
    u8 unk01;
    s16 y;
    int offset;
    u8 unk08;
    u8 unk09;
    u8 unk0A;
    u8 unk0B;
    int unk0C;
    int unk10;
    int unk14;
    void *owner;
    Pokepic *pic;
    u8 movement[0x24];
    u8 scale[0x24];
} Work;
enum HeapID BattleAnimSystem_GetHeapID(void *owner);
u32 BattleAnimSystem_GetAttacker(void *owner);
Pokepic *BattleAnimSystem_GetBattlerPokepic(void *owner, u32 battler);
u32 BattleAnimSystem_GetArg(void *owner, int index);
void ov07_02221F38(void *owner, u32 battler, int a2, s16 *out);
int ov07_0221FAA0(void *owner, u32 battler);
void ov07_02222590(void *movement, int x0, int x1, int y0, int y1, int a5, int frames);
void ov07_022227A8(void *scale, int a1, int a2, int a3, int a4);
void BattleAnimSystem_CreateTaskDefaultPriority(void *owner, void *func, void *data);
void ov07_02224EF4(void *task, Work *work);
void ov07_02225098(void *owner);

// @fn ov07_02225098
void ov07_02225098(void *owner) {
    Work *work;
    s16 x0, y0, x1, y1;
    work = Heap_Alloc(BattleAnimSystem_GetHeapID(owner), sizeof(Work));
    work->unk08 = 0;
    work->state = 0;
    work->owner = owner;
    work->pic = BattleAnimSystem_GetBattlerPokepic(work->owner, BattleAnimSystem_GetAttacker(work->owner));
    work->unk09 = 0;
    work->unk0A = BattleAnimSystem_GetArg(owner, 6);
    work->unk0C = BattleAnimSystem_GetArg(work->owner, 3);
    work->unk10 = BattleAnimSystem_GetArg(work->owner, 4);
    work->unk14 = BattleAnimSystem_GetArg(work->owner, 5);
    ov07_02221F38(owner, BattleAnimSystem_GetAttacker(work->owner), 0, &work->y);
    work->offset = ov07_0221FAA0(work->owner, BattleAnimSystem_GetAttacker(work->owner));
    work->y += work->offset;
    x0 = (s32)BattleAnimSystem_GetArg(owner, 0) >> 16;
    x1 = BattleAnimSystem_GetArg(owner, 0) & 0xFF;
    y0 = (s32)BattleAnimSystem_GetArg(owner, 1) >> 16;
    y1 = BattleAnimSystem_GetArg(owner, 1) & 0xFF;
    ov07_02222590(work->movement, x0, x1, y0, y1, 100, BattleAnimSystem_GetArg(owner, 2));
    ov07_022227A8(work->scale, 2, 0, 0, 10);
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_02224EF4, work);
}
