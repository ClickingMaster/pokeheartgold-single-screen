#include "to54_atomic_w04_private.h"

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

void ov12_0225A8C4(BattleSystem *battleSystem, OpponentData *opponentData, BattleMessage *msg) {
    MsgData *loader = BattleSystem_GetMessageLoader(battleSystem);
    u8 *work = Heap_Alloc(HEAP_ID_BATTLE, 8);
    U32(work, 0) = (u32)battleSystem;
    work[4] = U8(opponentData, 0x94);
    work[5] = U8(opponentData, 0x194);
    work[6] = BattleSystem_PrintBattleMessage(battleSystem, loader, msg, BattleSystem_GetTextFrameDelay(battleSystem));
    SysTask_CreateOnMainQueue(ov12_022605D0, work, 0);
}
