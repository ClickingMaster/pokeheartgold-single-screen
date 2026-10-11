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

void ov12_022592D0(BattleSystem *battleSystem, OpponentData *opponentData) {
    u32 battleType = BattleSystem_GetBattleType(battleSystem);
    BattleInput *input = BattleSystem_GetBattleInput(battleSystem);
    u32 masked;
    if (RAW_OPP(opponentData)[0x196] == 0) {
        masked = battleType & 8;
        if (masked == 0) {
            if (masked != 0) {
                goto done;
            }
            if (RAW_OPP(opponentData)[0x195] == 4) {
                goto done;
            }
        }
        BattleInput_StartMenuScrollHorizontalTask(input, -0xD00, 0);
    }
done:
    BattleController_EmitCommandComplete(battleSystem, BATTLER_ID(opponentData), COMMAND(opponentData)[0]);
    ov12_02259928(opponentData);
}

void ov12_02259328(BattleSystem *battleSystem, OpponentData *opponentData) {
    ov12_02264EB4(RAW_OPP(opponentData) + 0x28);
    ov12_02262014(opponentData);
    BattleController_EmitCommandComplete(battleSystem, BATTLER_ID(opponentData), COMMAND(opponentData)[0]);
    ov12_02259928(opponentData);
}
