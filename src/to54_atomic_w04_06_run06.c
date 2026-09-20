#include "to54_atomic_w04_private.h"

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

void ov12_0225A4DC(BattleSystem *battleSystem, OpponentData *opponentData, void *unused) {
    u8 *hpBar = (u8 *)opponentData + 0x28;
    MIi_CpuClearFast(0, (u32 *)hpBar, 1);
    U32(hpBar, 0x0C) = (u32)battleSystem;
    hpBar[0x24] = U8(opponentData, 0x194);
    hpBar[0x4C] = U8(opponentData, 0x94);
    ov12_02264FB0(hpBar, 1);
    U32(hpBar, 0x10) = (u32)SysTask_CreateOnMainQueue(ov12_0225DA8C, hpBar, 1000);
}
