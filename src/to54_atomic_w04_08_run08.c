#include "to54_atomic_w04_private.h"

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

void ov12_0225A604(BattleSystem *battleSystem, OpponentData *opponentData, u8 *msg) {
    u8 *work = Heap_Alloc(HEAP_ID_BATTLE, 0x24);
    int i = 0;
    work[0x20] = 0;
    U32(work, 0) = (u32)battleSystem;
    work[0x1C] = U8(opponentData, 0x94);
    work[0x1D] = U8(opponentData, 0x194);
    work[0x1E] = U8(opponentData, 0x195);
    U32(work, 4) = (u32)((u8 *)opponentData + 0x28);
    work[0x1F] = msg[1];
    {
        u8 *msgp = msg;
        u8 *workp = work;
        for (i = 0; i < 4; i++) {
            U16(workp, 0x0C) = U16(msgp, 4);
            work[0x14 + i] = msg[0x0C + i];
            work[0x18 + i] = msg[0x10 + i];
            msgp += 2;
            workp += 2;
        }
    }
    U16(work, 0x22) = U16(msg, 2);
    SysTask_CreateOnMainQueue((SysTaskFunc)U32(opponentData, 4), work, 0);
}
