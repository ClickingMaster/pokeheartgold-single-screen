#ifndef TO56_R2_W19_PRIVATE_H
#define TO56_R2_W19_PRIVATE_H
#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "sys_task_api.h"
typedef struct BattleSystem BattleSystem;
typedef struct PaletteData PaletteData;
typedef struct Party Party;
typedef struct R2Child {
    Party *party;
    u32 pad04;
    BattleSystem *battleSystem;
    u32 unk0C;
    u8 pad10;
    u8 unk11;
    u8 unk12;
    u8 pad13[0xF];
    u16 unk22;
    u16 move;
    u32 unk28;
    u8 pad2C[6];
    u8 unk32;
    u8 pad33;
    u8 result;
    u8 unk35;
    u8 done;
    u8 pad37;
} R2Child;
typedef struct R2Work {
    BattleSystem *battleSystem;
    R2Child *child;
    u8 command;
    u8 battler;
    u8 state;
    u8 pad0B;
    u16 move;
    u8 param;
    u8 pad0F;
} R2Work;
PaletteData *BattleSystem_GetPaletteData(BattleSystem *);
Party *BattleSystem_GetParty(BattleSystem *, int);
void PaletteData_BeginPaletteFade(PaletteData *, u32, u16, s8, u8, u8, u16);
u32 PaletteData_GetSelectedBuffersBitmask(PaletteData *);
void ov12_02237B0C(BattleSystem *);
void ov12_02237BB8(BattleSystem *);
void ov10_0221BE20(R2Child *);
void BattleController_ReplyPartySelection(BattleSystem *, u32, u32);
void BattleController_EmitCommandComplete(BattleSystem *, u32, u32);
void ov12_022609F8(SysTask *, R2Work *);
#endif
