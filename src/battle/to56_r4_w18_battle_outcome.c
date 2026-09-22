#include "battle/to56_r4_w18_battle_outcome_private.h"
#define RAW(p) ((u8 *)(p))
void ov12_022597EC(BattleSystem *battleSystem, OpponentDataR4W18 *opponentData) {
    BattleCommandR4W18 *command = (BattleCommandR4W18 *)(RAW(opponentData) + 0x94);
    u32 left = 0;
    int i;
    u32 right = 0;
    int j;
    Party *party;
    Pokemon *mon;

    ov12_0223BF14(battleSystem, command->arg, command->payload);
    if (BattleSystem_GetBattleType(battleSystem) & 0x80) {
        BattleSystem_SetBattleOutcomeFlags(battleSystem, (u8)command->outcome);
    } else {
        for (i = 0; i < BattleSystem_GetMaxBattlers(battleSystem); i++) {
            party = BattleSystem_GetParty(battleSystem, i);
            for (j = 0; j < Party_GetCount(party); j++) {
                mon = Party_GetMonByIndex(party, j);
                if (GetMonData(mon, MON_DATA_SPECIES, NULL) != 0 && GetMonData(mon, MON_DATA_IS_EGG, NULL) == 0) {
                    if (BattleSystem_GetFieldSide(battleSystem, i) != 0) {
                        right += GetMonData(mon, MON_DATA_HP, NULL);
                    } else {
                        left += GetMonData(mon, MON_DATA_HP, NULL);
                    }
                }
            }
        }
        if (left == 0 && right == 0) {
            BattleSystem_SetBattleOutcomeFlags(battleSystem, 3);
        } else if (left == 0) {
            BattleSystem_SetBattleOutcomeFlags(battleSystem, 2);
        } else {
            BattleSystem_SetBattleOutcomeFlags(battleSystem, 1);
        }
    }
    ov12_0226430C(battleSystem, RAW(opponentData)[0x194], command->command);
    ov12_02259928(opponentData);
}
