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

void ov12_022594F4(BattleSystem *battleSystem, OpponentData *opponentData) {
    int offset = 0x65;
    BattleSystem_SetBackground(battleSystem);
    BattleController_EmitCommandComplete(battleSystem, ((u8 *)opponentData)[offset * 4], 0x2E);
    ov12_02259928(opponentData);
}
