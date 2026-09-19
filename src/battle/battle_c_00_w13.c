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

void *ov12_02258D74(BattleSystem *battleSystem, const u8 *buffer) {
    u8 *opponentData = Heap_Alloc(HEAP_ID_BATTLE, 0x1AC);
    MI_CpuClearFast(opponentData, 0x1AC);
    opponentData[0x194] = buffer[0];
    opponentData[0x195] = buffer[1];
    *(void **)(opponentData + 0x1A4) = NARC_New(NARC_a_1_8_0, HEAP_ID_BATTLE);
    return opponentData;
}
