#include "to54_atomic_w04_private.h"

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

void ov12_0225A334(BattleSystem *battleSystem, OpponentData *opponentData, void *unused) {
    u8 *work = Heap_Alloc(HEAP_ID_BATTLE, 0x10);
    work[10] = 0;
    U32(work, 0) = (u32)battleSystem;
    work[8] = U8(opponentData, 0x94);
    work[9] = U8(opponentData, 0x194);
    U32(work, 4) = (u32)opponentData;
    if (U8(opponentData, 0x195) & 1) {
        work[11] = 2;
    } else {
        work[11] = 0;
    }
    SysTask_CreateOnMainQueue(ov12_0225D890, work, 0);
}
