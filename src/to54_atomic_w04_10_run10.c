#include "to54_atomic_w04_private.h"

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

void ov12_0225A818(BattleSystem *battleSystem, OpponentData *opponentData, u8 *msg) {
    u8 *work = Heap_Alloc(HEAP_ID_BATTLE, 0x1C);
    work[0xE] = 0;
    U32(work, 0) = (u32)battleSystem;
    work[0xC] = msg[0];
    work[0xD] = U8(opponentData, 0x194);
    U32(work, 4) = (u32)((u8 *)opponentData + 0x28);
    U32(work, 0x10) = U16(msg, 2);
    work[0xF] = msg[1];
    U16(work, 0x18) = U32(msg, 4);
    U32(work, 0x14) = U32(msg, 8);
    SysTask_CreateOnMainQueue((SysTaskFunc)U32(opponentData, 0x14), work, 0);
}
