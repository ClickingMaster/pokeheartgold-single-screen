#include "to54_atomic_w04_private.h"

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

void ov12_0225A018(BattleSystem *battleSystem, OpponentData *opponentData, u8 *msg) {
    u8 *work = Heap_Alloc(HEAP_ID_BATTLE, 0x10);
    U32(work, 0) = (u32)battleSystem;
    U32(work, 4) = U32(opponentData, 0x20);
    work[8] = msg[0];
    work[9] = U8(opponentData, 0x194);
    work[10] = U8(opponentData, 0x195);
    work[11] = 0;
    work[12] = msg[1];
    Pokepic_StartPaletteFade((Pokepic *)U32(work, 4), 0, 0x10, 0, ov12_0226D15A[U16(msg, 2)]);
    Pokepic_SetAttr((Pokepic *)U32(work, 4), (PokepicAttr)45, 1);
    SysTask_CreateOnMainQueue(ov12_0225CDB8, work, 0);
}
