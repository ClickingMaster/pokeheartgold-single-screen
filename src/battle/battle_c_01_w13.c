#include "w13_private.h"

#define RAW_OPP(p)    ((u8 *)(p))
#define COMMAND(p)    (RAW_OPP(p) + 0x94)
#define BATTLER_ID(p) RAW_OPP(p)[0x65 * 4]

#define DEFINE_HANDLER3(name, worker)                                   \
    void worker(BattleSystem *, OpponentData *, u8 *);                  \
    void name(BattleSystem *battleSystem, OpponentData *opponentData) { \
        worker(battleSystem, opponentData, COMMAND(opponentData));      \
        ov12_02259928(opponentData);                                    \
    }

#define DEFINE_HANDLER2(name, worker)                                   \
    void worker(BattleSystem *, OpponentData *);                        \
    void name(BattleSystem *battleSystem, OpponentData *opponentData) { \
        worker(battleSystem, opponentData);                             \
        ov12_02259928(opponentData);                                    \
    }

void ov12_02258E54(BattleSystem *battleSystem, OpponentData *opponentData) {
    if (COMMAND(opponentData)[0] != 0) {
        RAW_OPP(opponentData)[0x1A8] = 0;
        ov12_0226D010[COMMAND(opponentData)[0]](battleSystem, opponentData);
    }
}

void ov12_02258E7C(BattleSystem *battleSystem, OpponentData *opponentData, int mode) {
    if (mode != 2) {
        BattleHpBar_FreeResources((BattleHpBar *)(RAW_OPP(opponentData) + 0x28));
    }
    if (*(void **)(RAW_OPP(opponentData) + 0x18) != NULL) {
        Sprite_DeleteAndFreeResources(*(void **)(RAW_OPP(opponentData) + 0x18));
    }
    ov12_02262014(opponentData);
    NARC_Delete(*(NARC **)(RAW_OPP(opponentData) + 0x1A4));
    Heap_Free(opponentData);
}

void ov12_02258EB0(BattleSystem *battleSystem, OpponentData *opponentData) {
}

void ov12_02258EB4(BattleSystem *battleSystem, OpponentData *opponentData) {
    BattleSystem_SetRandTemp(battleSystem, *(u32 *)(COMMAND(opponentData) + 4));
    ov12_02259944(battleSystem, opponentData);
    ov12_0226430C(battleSystem, BATTLER_ID(opponentData), 1);
    ov12_02259928(opponentData);
}


DEFINE_HANDLER3(ov12_02258EE0, ov12_02259968)

DEFINE_HANDLER3(ov12_02258EF4, ov12_02259BA8)

DEFINE_HANDLER3(ov12_02258F08, ov12_02259D48)

DEFINE_HANDLER3(ov12_02258F1C, ov12_02259F30)

DEFINE_HANDLER3(ov12_02258F30, ov12_0225A018)


void ov12_02258F44(BattleSystem *battleSystem, OpponentData *opponentData) {
    int offset = 0x65;
    Pokepic_Delete(opponentData->pokepic);
    ov12_0226430C(battleSystem, ((u8 *)opponentData)[offset * 4], 7);
    ov12_02259928(opponentData);
}


DEFINE_HANDLER3(ov12_02258F68, ov12_0225A07C)

DEFINE_HANDLER3(ov12_02258F7C, ov12_0225A2A0)

DEFINE_HANDLER2(ov12_02258F90, ov12_0225A334)

DEFINE_HANDLER3(ov12_02258FA0, ov12_0225A37C)

DEFINE_HANDLER3(ov12_02258FB4, ov12_0225A414)

DEFINE_HANDLER2(ov12_02258FC8, ov12_0225A4DC)


void ov12_02258FD8(BattleSystem *battleSystem, OpponentData *opponentData) {
    u8 *command = COMMAND(opponentData);
    ov12_0223BB6C(battleSystem, command[0x29]);
    ov12_0225A524(battleSystem, opponentData, command);
    ov12_02259928(opponentData);
}


DEFINE_HANDLER3(ov12_02259000, ov12_0225A604)

DEFINE_HANDLER3(ov12_02259014, ov12_0225A674)

DEFINE_HANDLER3(ov12_02259028, ov12_0225A700)

DEFINE_HANDLER3(ov12_0225903C, ov12_0225A7AC)

DEFINE_HANDLER3(ov12_02259050, ov12_0225A818)

DEFINE_HANDLER3(ov12_02259064, ov12_0225A85C)

DEFINE_HANDLER3(ov12_02259078, ov12_0225A8C4)

DEFINE_HANDLER3(ov12_0225908C, ov12_0225A914)


void ov12_022590A0(BattleSystem *battleSystem, OpponentData *opponentData) {
    if (Pokepic_GetAttr(opponentData->pokepic, 6) == 1) {
        ov12_0226430C(battleSystem, BATTLER_ID(opponentData), 0x17);
    } else {
        ov12_0225A9B0(battleSystem, opponentData);
    }
    ov12_02259928(opponentData);
}


DEFINE_HANDLER3(ov12_022590D4, ov12_0225A9E0)

DEFINE_HANDLER3(ov12_022590E8, ov12_0225AA6C)

DEFINE_HANDLER3(ov12_022590FC, ov12_0225AAE0)

DEFINE_HANDLER3(ov12_02259110, ov12_0225ABB8)

DEFINE_HANDLER2(ov12_02259124, ov12_0225ABE8)

DEFINE_HANDLER3(ov12_02259134, ov12_0225AC1C)

DEFINE_HANDLER3(ov12_02259148, ov12_0225ACB0)

DEFINE_HANDLER3(ov12_0225915C, ov12_0225ACE8)

DEFINE_HANDLER3(ov12_02259170, ov12_0225AD44)

DEFINE_HANDLER3(ov12_02259184, ov12_0225AD9C)

DEFINE_HANDLER2(ov12_02259198, ov12_0225ADF4)

DEFINE_HANDLER3(ov12_022591A8, ov12_0225AE48)

DEFINE_HANDLER2(ov12_022591BC, ov12_0225AEA0)

DEFINE_HANDLER3(ov12_022591CC, ov12_0225AED8)

DEFINE_HANDLER3(ov12_022591E0, ov12_0225AF74)

