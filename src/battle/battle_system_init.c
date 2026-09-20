#include "global.h"

#include "constants/battle.h"
#include "constants/pokemon.h"

#include "battle/battle.h"
#include "battle/battle_controller_player.h"
#include "battle/battle_setup.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"

#include "bag.h"
#include "battle_link_internal.h"
#include "math_util.h"
#include "party.h"
#include "player_data.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_mood.h"

typedef struct BattlerInitData {
    u8 battler;
    u8 battlerType;
} BattlerInitData;

typedef struct BattleSystemTail {
    u8 padding[0x2478];
    u32 flags;
    u32 value;
} BattleSystemTail;

extern const u8 _0226BFD8[4];
extern const u8 ov12_0226BFDC[4];
extern const u8 ov12_0226BFE0[2][4];
extern const u8 ov12_0226C008[4][4];
extern const u8 ov12_0226C2DC[4];

OpponentData *ov12_02258D74(BattleSystem *battleSystem, const u8 *data);
void ov12_02260EA4(BattleSystem *battleSystem, OpponentData *opponentData);
int ov12_022581D4(BattleSystem *battleSystem, BattleContext *ctx, int var, int battlerId);
void ov12_0223A664(BattleSystem *battleSystem, BattleSetup *setup);
BOOL ov12_022395BC(u16 trainerClass);
void ov12_02238A68(BattleSystem *battleSystem, BattleSetup *setup);

void ov12_02238A68(BattleSystem *battleSystem, BattleSetup *setup) {
    int i, slot;
    BattlerInitData battlerInitData;
    Pokemon *mon;

    battleSystem->battleType = setup->battleType;

    for (i = 0; i < BATTLER_MAX; i++) {
        battleSystem->playerProfile[i] = PlayerProfile_New(HEAP_ID_BATTLE);
        PlayerProfile_Copy(setup->profile[i], battleSystem->playerProfile[i]);
        battleSystem->chatotVoice[i] = setup->chatot[i];
    }

    battleSystem->unk2446 = setup->unk1B0;

    for (i = 0; i < BATTLER_MAX; i++) {
        battleSystem->unk2464[i] = setup->unk_1A0[i];
    }

    battleSystem->unk2434 = GetLCRNGSeed();
    battleSystem->rand = setup->unk_19C;
    battleSystem->randTemp = setup->unk_19C;
    battleSystem->battleSpecial = setup->battleSpecial;
    battleSystem->bag = Save_Bag_New(HEAP_ID_BATTLE);
    Save_Bag_Copy(setup->bag, battleSystem->bag);
    battleSystem->pokedex = Pokedex_New(HEAP_ID_BATTLE);
    Pokedex_Copy(setup->pokedex, battleSystem->pokedex);

    battleSystem->storage = setup->storagePC;
    battleSystem->options = setup->options;
    battleSystem->unk1B8 = (u32 *)setup->palPad;
    battleSystem->bagCursor = setup->bagCursor;
    battleSystem->unk1C0 = setup->unk1B8;
    battleSystem->unk98 = setup->unk_128;
    battleSystem->unk2424 = setup->evolutionLocation;
    battleSystem->unk9C = (u32 *)setup->unk_134;
    battleSystem->safariBallCnt = setup->safariBalls;
    battleSystem->terrain = setup->terrain;
    battleSystem->backgroundId = setup->battleBg;
    battleSystem->location = setup->mapSection;
    battleSystem->timezone = setup->timeOfDay;
    battleSystem->unk241C = setup->fixedDamaageMovesBanned;
    battleSystem->unk2428 = setup->unk_164;
    battleSystem->metBill = setup->metBill;
    battleSystem->unk2474_1 = setup->momsSavingsActive;
    battleSystem->isFishing = setup->unk_1D0;
    battleSystem->weather = setup->weatherType;
    battleSystem->gameStats = setup->gameStats;

    GF_ASSERT(setup->gameStats != NULL);
    battleSystem->bugContestCaughtMon = setup->bugContestMon;

    for (i = 0; i < BATTLER_MAX; i++) {
        battleSystem->trainerId[i] = setup->trainerId[i];
        battleSystem->trainers[i] = setup->trainer[i];
    }

    battleSystem->ctx = BattleContext_New(battleSystem);

    for (i = 0; i < BATTLER_MAX; i++) {
        battleSystem->trainerParty[i] = SaveArray_Party_Alloc(HEAP_ID_BATTLE);
        battleSystem->unk248C[i] = setup->unk1CC[i];
    }

    int gender;

    for (i = 0; i < BATTLER_MAX; i++) {
        for (slot = 0; slot < Party_GetCount(setup->party[i]); slot++) {
            mon = Party_GetMonByIndex(setup->party[i], slot);
            gender = GetMonGender(mon);
            SetMonData(mon, MON_DATA_GENDER, &gender);
        }
    }

    if (battleSystem->battleType & BATTLE_TYPE_LINK) {
        sub_02074E5C(battleSystem);
        u8 networkID = ov12_0223BFC0(battleSystem);

        ov12_0223A664(battleSystem, setup);

        if (battleSystem->battleType & BATTLE_TYPE_FRONTIER) {
            for (i = 0; i < BATTLER_MAX; i++) {
                battlerInitData.battler = i;
                battlerInitData.battlerType = ov12_0226C2DC[i];
                battleSystem->opponentData[i] = ov12_02258D74(battleSystem, (u8 *)&battlerInitData);
            }

            battleSystem->maxBattlers = i;

            for (i = 0; i < BATTLER_MAX; i++) {
                ov12_02260EA4(battleSystem, battleSystem->opponentData[i]);
                Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
            }

            for (i = 0; i < battleSystem->maxBattlers; i++) {
                for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i]); slot++) {
                    mon = Party_GetMonByIndex(battleSystem->trainerParty[i], slot);
                    if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                        && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                        && GetMonData(mon, MON_DATA_HP, NULL)) {
                        break;
                    }
                }
                ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
            }
            ov12_02256F28(battleSystem, battleSystem->ctx);
        } else if (battleSystem->battleType & BATTLE_TYPE_MULTI) {
            for (i = 0; i < BATTLER_MAX; i++) {
                battlerInitData.battler = i;
                battlerInitData.battlerType = ov12_0226C008[ov12_0223BFCC(battleSystem, networkID)][ov12_0223BFCC(battleSystem, i)];
                battleSystem->opponentData[i] = ov12_02258D74(battleSystem, (u8 *)&battlerInitData);
            }
            battleSystem->maxBattlers = i;
            for (i = 0; i < BATTLER_MAX; i++) {
                ov12_02260EA4(battleSystem, battleSystem->opponentData[i]);
                Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
            }
            for (i = 0; i < battleSystem->maxBattlers; i++) {
                for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i]); slot++) {
                    mon = Party_GetMonByIndex(battleSystem->trainerParty[i], slot);
                    if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                        && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                        && GetMonData(mon, MON_DATA_HP, NULL)) {
                        break;
                    }
                }
                ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
            }
            ov12_02256F28(battleSystem, battleSystem->ctx);
        } else if (battleSystem->battleType & BATTLE_TYPE_DOUBLES) {
            for (i = 0; i < BATTLER_MAX; i++) {
                battlerInitData.battler = i;
                battlerInitData.battlerType = ov12_0226BFE0[networkID][i];
                battleSystem->opponentData[i] = ov12_02258D74(battleSystem, (u8 *)&battlerInitData);
                ov12_02260EA4(battleSystem, battleSystem->opponentData[i]);
            }
            battleSystem->maxBattlers = i;
            for (i = 0; i < BATTLER_MAX; i++) {
                Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
            }
            for (i = 0; i < battleSystem->maxBattlers; i++) {
                for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i & 1]); slot++) {
                    mon = Party_GetMonByIndex(battleSystem->trainerParty[i & 1], slot);
                    if (i > 1) {
                        if (ov12_022581D4(battleSystem, battleSystem->ctx, 2, i & 1) != slot
                            && GetMonData(mon, MON_DATA_SPECIES, NULL)
                            && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                            && GetMonData(mon, MON_DATA_HP, NULL)) {
                            break;
                        }
                    } else if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                        && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                        && GetMonData(mon, MON_DATA_HP, NULL)) {
                        break;
                    }
                }
                ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
            }
        } else {
            for (i = 0; i < BATTLER_MAX / 2; i++) {
                battlerInitData.battler = i;
                battlerInitData.battlerType = ov12_0226BFDC[networkID * 2 + i];
                battleSystem->opponentData[i] = ov12_02258D74(battleSystem, (u8 *)&battlerInitData);
                ov12_02260EA4(battleSystem, battleSystem->opponentData[i]);
            }
            battleSystem->maxBattlers = i;
            for (i = 0; i < BATTLER_MAX; i++) {
                Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
                for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i]); slot++) {
                    mon = Party_GetMonByIndex(battleSystem->trainerParty[i], slot);
                    if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                        && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                        && GetMonData(mon, MON_DATA_HP, NULL)) {
                        break;
                    }
                }
                ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
            }
        }
        ov12_02256F28(battleSystem, battleSystem->ctx);
    } else if (battleSystem->battleType & BATTLE_TYPE_TAG) {
        int playerSlot;
        for (i = 0; i < BATTLER_MAX; i++) {
            battlerInitData.battler = i;
            battlerInitData.battlerType = ov12_0226C2DC[i];
            battleSystem->opponentData[i] = ov12_02258D74(battleSystem, (u8 *)&battlerInitData);
            ov12_02260EA4(battleSystem, battleSystem->opponentData[i]);
        }
        battleSystem->maxBattlers = i;
        for (i = 0; i < BATTLER_MAX; i++) {
            Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
        }
        for (i = 0; i < battleSystem->maxBattlers; i++) {
            for (slot = 0; slot < BattleSystem_GetPartySize(battleSystem, i); slot++) {
                mon = BattleSystem_GetPartyMon(battleSystem, i, slot);
                if (i == 2) {
                    if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                        && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                        && GetMonData(mon, MON_DATA_HP, NULL)
                        && playerSlot != slot) {
                        break;
                    }
                } else if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                    && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                    && GetMonData(mon, MON_DATA_HP, NULL)) {
                    break;
                }
            }
            ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
            if (i == 0) {
                playerSlot = slot;
            }
        }
        ov12_02256F28(battleSystem, battleSystem->ctx);
        battleSystem->unk23FC = 1;
    } else if (battleSystem->battleType & BATTLE_TYPE_MULTI) {
        for (i = 0; i < BATTLER_MAX; i++) {
            battlerInitData.battler = i;
            battlerInitData.battlerType = ov12_0226C2DC[i];
            battleSystem->opponentData[i] = ov12_02258D74(battleSystem, (u8 *)&battlerInitData);
            ov12_02260EA4(battleSystem, battleSystem->opponentData[i]);
        }
        battleSystem->maxBattlers = i;
        for (i = 0; i < BATTLER_MAX; i++) {
            Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
        }
        for (i = 0; i < battleSystem->maxBattlers; i++) {
            for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i]); slot++) {
                mon = Party_GetMonByIndex(battleSystem->trainerParty[i], slot);
                if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                    && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                    && GetMonData(mon, MON_DATA_HP, NULL)) {
                    break;
                }
            }
            ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
        }
        ov12_02256F28(battleSystem, battleSystem->ctx);
        battleSystem->unk23FC = 1;
    } else if (battleSystem->battleType & BATTLE_TYPE_DOUBLES) {
        for (i = 0; i < BATTLER_MAX; i++) {
            battlerInitData.battler = i;
            battlerInitData.battlerType = ov12_0226C2DC[i];
            battleSystem->opponentData[i] = ov12_02258D74(battleSystem, (u8 *)&battlerInitData);
            ov12_02260EA4(battleSystem, battleSystem->opponentData[i]);
        }
        battleSystem->maxBattlers = i;
        for (i = 0; i < BATTLER_MAX; i++) {
            Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
        }
        for (i = 0; i < battleSystem->maxBattlers; i++) {
            for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i & 1]); slot++) {
                mon = Party_GetMonByIndex(battleSystem->trainerParty[i & 1], slot);
                if (i > 1) {
                    if (ov12_022581D4(battleSystem, battleSystem->ctx, 2, i & 1) != slot
                        && GetMonData(mon, MON_DATA_SPECIES, NULL)
                        && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                        && GetMonData(mon, MON_DATA_HP, NULL)) {
                        break;
                    }
                } else if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                    && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                    && GetMonData(mon, MON_DATA_HP, NULL)) {
                    break;
                }
            }
            ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
        }
        ov12_02256F28(battleSystem, battleSystem->ctx);
        battleSystem->unk23FC = 1;
    } else {
        for (i = 0; i < BATTLER_MAX / 2; i++) {
            battlerInitData.battler = i;
            battlerInitData.battlerType = _0226BFD8[i];
            battleSystem->opponentData[i] = ov12_02258D74(battleSystem, (u8 *)&battlerInitData);
            ov12_02260EA4(battleSystem, battleSystem->opponentData[i]);
        }
        battleSystem->maxBattlers = i;
        for (i = 0; i < BATTLER_MAX; i++) {
            Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
            for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i]); slot++) {
                mon = Party_GetMonByIndex(battleSystem->trainerParty[i], slot);
                if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                    && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                    && GetMonData(mon, MON_DATA_HP, NULL)) {
                    break;
                }
            }
            ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
        }
        ov12_02256F28(battleSystem, battleSystem->ctx);
        battleSystem->unk23FC = 1;
    }

    if (battleSystem->battleType & BATTLE_TYPE_PAL_PARK) {
        mon = Party_GetMonByIndex(battleSystem->trainerParty[1], 0);
        GetMonData(mon, MON_DATA_OT_NAME, (u8 *)&battleSystem->trainers[1].name);
    }

    if (battleSystem->battleType & BATTLE_TYPE_TRAINER) {
        if (ov12_022395BC(battleSystem->trainers[1].data.trainerClass) == TRUE
            || ov12_022395BC(battleSystem->trainers[3].data.trainerClass) == TRUE) {
            for (i = 0; i < Party_GetCount(battleSystem->trainerParty[0]); i++) {
                mon = Party_GetMonByIndex(battleSystem->trainerParty[0], i);
                MonApplyFriendshipMod(mon, FRIENDSHIP_EVENT_LEAGUE_BATTLE, battleSystem->location);
                ApplyMonMoodModifier(mon, 2);
            }
            for (i = 0; i < Party_GetCount(battleSystem->trainerParty[2]); i++) {
                mon = Party_GetMonByIndex(battleSystem->trainerParty[2], i);
                MonApplyFriendshipMod(mon, FRIENDSHIP_EVENT_LEAGUE_BATTLE, battleSystem->location);
                ApplyMonMoodModifier(mon, 2);
            }
        }
    }

    BattleSystemTail *tail = (BattleSystemTail *)battleSystem;
    tail->value = 0;
    tail->flags &= ~4;
    int incompatible = 0;
    for (i = 0; i < battleSystem->maxBattlers; i++) {
        int version = PlayerProfile_GetVersion(BattleSystem_GetPlayerProfile(battleSystem, i));
        if (version != GAME_VERSION) {
            incompatible = 1;
        }
        if (version != VERSION_HEARTGOLD && version != VERSION_SOULSILVER) {
            tail->flags |= 4;
        }
    }
    if (battleSystem->unk23FC != 0) {
        tail->value = incompatible;
        return;
    }
    tail->value = 0;
}
