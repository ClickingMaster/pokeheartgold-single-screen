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


DEFINE_HANDLER3(ov12_022593D4, ov12_0225B028)

DEFINE_HANDLER3(ov12_022593E8, ov12_0225B060)

