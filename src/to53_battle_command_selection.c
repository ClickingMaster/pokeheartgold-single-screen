#include "global.h"

#include "battle/battle.h"
#include "battle/battle_controller.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"

#include "party.h"
#include "pokemon.h"

typedef struct CommandSetMessageW01 {
    u8 command;
    u8 partySlot;
    u8 expPercents[6];
    u8 ballStatus[2][6];
    u16 moves[4];
    u8 curPP[4];
    u8 maxPP[4];
    s16 curHP;
    u16 maxHP;
    u8 ballStatusBattler;
    u8 switchingOrCanPickCommandMask;
    u16 padding;
} CommandSetMessageW01;

extern void ov12_02262240(BattleSystem *, int, int, const void *, int);

void ov12_02262B80(BattleSystem *battleSystem, BattleContext *ctx, int battlerId, int index) {
    CommandSetMessageW01 message;
    int i;
    int battlerType;
    int species;
    int count;
    Party *party;
    Pokemon *mon;
    u32 battleType;
    int unavailableMask;

    MI_CpuClearFast(&message, sizeof(message));
    BattleBuffer_Clear(BattleSystem_GetBattleContext(battleSystem), battlerId);

    unavailableMask = 0;
    for (i = 0; i < BattleSystem_GetMaxBattlers(battleSystem); i++) {
        if (Battler_CanSelectAction(ctx, i) == 0) {
            unavailableMask |= MaskOfFlagNo(i);
        }
    }

    message.command = 14;
    message.partySlot = index;
    message.switchingOrCanPickCommandMask = ctx->switchInFlag | unavailableMask;
    battleType = BattleSystem_GetBattleType(battleSystem);

    if ((battleType & 2) && !(battleType & 8)) {
        battlerType = battlerId & 1;
    } else {
        battlerType = battlerId;
    }

    party = BattleSystem_GetParty(battleSystem, battlerType);
    count = 0;
    for (i = 0; i < Party_GetCount(party); i++) {
        mon = Party_GetMonByIndex(party, ctx->unk_312C[battlerType][i]);
        species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);
        if (species != 0 && species != SPECIES_EGG) {
            if (GetMonData(mon, MON_DATA_HP, NULL)) {
                if (GetMonData(mon, MON_DATA_STATUS, NULL)) {
                    message.ballStatus[0][count] = 3;
                } else {
                    message.ballStatus[0][count] = 1;
                }
            } else {
                message.ballStatus[0][count] = 2;
            }
            if (battleType & 0x2A4) {
                message.expPercents[count] = 0;
            } else {
                message.expPercents[count] = GetPercentProgressTowardsNextLevel(mon);
            }
            count++;
        }
    }

    if (((battleType & 0xC) == 0xC) || (battleType & 0x10) || battleType == 0x4B || battleType == 0xCB) {
        if (BattleSystem_GetFieldSide(battleSystem, battlerId)) {
            battlerType = BattleSystem_GetBattlerFromBattlerType(battleSystem, 2);
        } else {
            battlerType = BattleSystem_GetBattlerFromBattlerType(battleSystem, 3);
        }
        party = BattleSystem_GetParty(battleSystem, battlerType);
        count = 0;
        for (i = 0; i < Party_GetCount(party); i++) {
            mon = Party_GetMonByIndex(party, ctx->unk_312C[battlerType][i]);
            species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);
            if (species != 0 && species != SPECIES_EGG) {
                if (GetMonData(mon, MON_DATA_HP, NULL)) {
                    if (GetMonData(mon, MON_DATA_STATUS, NULL)) {
                        message.ballStatus[1][count] = 3;
                    } else {
                        message.ballStatus[1][count] = 1;
                    }
                } else {
                    message.ballStatus[1][count] = 2;
                }
                count++;
            }
        }

        if (BattleSystem_GetFieldSide(battleSystem, battlerId)) {
            battlerType = BattleSystem_GetBattlerFromBattlerType(battleSystem, 4);
        } else {
            battlerType = BattleSystem_GetBattlerFromBattlerType(battleSystem, 5);
        }
        party = BattleSystem_GetParty(battleSystem, battlerType);
        count = 3;
        for (i = 0; i < Party_GetCount(party); i++) {
            mon = Party_GetMonByIndex(party, ctx->unk_312C[battlerType][i]);
            species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);
            if (species != 0 && species != SPECIES_EGG) {
                if (GetMonData(mon, MON_DATA_HP, NULL)) {
                    if (GetMonData(mon, MON_DATA_STATUS, NULL)) {
                        message.ballStatus[1][count] = 3;
                    } else {
                        message.ballStatus[1][count] = 1;
                    }
                } else {
                    message.ballStatus[1][count] = 2;
                }
                count++;
            }
        }
    } else {
        battlerType = ov12_0223ABB8(battleSystem, battlerId, 2);
        party = BattleSystem_GetParty(battleSystem, battlerType);
        count = 0;
        for (i = 0; i < Party_GetCount(party); i++) {
            mon = Party_GetMonByIndex(party, ctx->unk_312C[battlerType][i]);
            species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);
            if (species != 0 && species != SPECIES_EGG) {
                if (GetMonData(mon, MON_DATA_HP, NULL)) {
                    if (GetMonData(mon, MON_DATA_STATUS, NULL)) {
                        message.ballStatus[1][count] = 3;
                    } else {
                        message.ballStatus[1][count] = 1;
                    }
                } else {
                    message.ballStatus[1][count] = 2;
                }
                count++;
            }
        }
    }

    for (i = 0; i < 4; i++) {
        message.moves[i] = GetBattlerVar(ctx, battlerId, 6 + i, NULL);
        message.curPP[i] = GetBattlerVar(ctx, battlerId, 0x1F + i, NULL);
        message.maxPP[i] = GetBattlerVar(ctx, battlerId, 0x27 + i, NULL);
    }
    message.curHP = ctx->battleMons[battlerId].hp;
    message.maxHP = ctx->battleMons[battlerId].maxHp;
    if (message.curHP) {
        if (ctx->battleMons[battlerId].status) {
            message.ballStatusBattler = 3;
        } else {
            message.ballStatusBattler = 1;
        }
    } else {
        message.ballStatusBattler = 2;
    }
    ov12_02262240(battleSystem, 1, battlerId, &message, sizeof(message));
}
