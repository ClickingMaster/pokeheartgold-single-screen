#ifndef TO50_BATTLE_OPP_R1_PRIVATE_H
#define TO50_BATTLE_OPP_R1_PRIVATE_H
#include "global.h"

#include "battle/battle_system.h"

#define U8(p, n)  (*(u8 *)((u8 *)(p) + (n)))
#define U16(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define U32(p, n) (*(u32 *)((u8 *)(p) + (n)))

#pragma require_prototypes off
#endif
