#include "global.h"

#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/sprites.h"
#include "constants/trainer_class.h"

#include "battle/battle_setup.h"
#include "frontier/frontier_trainer_record.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0193.h"

#include "comm_state.h"
#include "communication_core.h"
#include "communication_player_state.h"
#include "filesystem.h"
#include "heap.h"
#include "map_header.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "party.h"
#include "player_data.h"
#include "pm_string.h"
#include "pm_version.h"
#include "pokemon.h"
#include "save_wifi_history.h"
#include "scrcmd_9.h"
#include "sprite.h"
#include "string_util.h"
#include "trainer_memo.h"
#include "ui_util.h"

// Frontier trainer and Pokemon generation helpers shared by the facilities: trainers (narc 204) list the ids of their
// Pokemon sets (narc 205); a set becomes a FrontierMon (the record the facilities store) and then a Pokemon.

// First u16s of a trainer's narc member.
typedef struct FrontierTrainerEntry {
    u16 trainerClass;
    u16 numMons;
    u16 monIds[];
} FrontierTrainerEntry;

// Pokemon set (narc 205 member).
typedef struct FrontierMonSet {
    u16 species;
    u16 moves[4];
    u8 evMask;
    u8 nature;
    u16 item;
    u16 form;
} FrontierMonSet;

typedef struct FrontierMonParam {
    u8 pp;
    u8 value;
    u16 move;
} FrontierMonParam;

void *FrontierData_AllocMember(s32 fileId, enum HeapID heapID, NarcId narcId);
void FrontierData_ReadMember(void *dest, s32 fileId, NarcId narcId);
u32 FrontierMon_Generate(FrontierMon *mon, u32 setId, u32 otId, u32 pid, u8 iv, u8 itemIndex, BOOL useItemTable, enum HeapID heapID, NarcId narcId);
void FrontierMon_ToPokemon(FrontierMon *frontierMon, Pokemon *mon, int level);
u16 FrontierTrainer_GetClassSprite(u16 value);
void Frontier_SetRentalMonOt(SaveData *saveData, Pokemon *mon);
void Frontier_AddRentalMonToParty(SaveData *saveData, Party *party, Pokemon *mon);
void Frontier_SetSpriteAnim(Sprite *sprite, int anim);
void Frontier_SetSpritePosition(Sprite *sprite, int x, int y, BOOL bounce);
int Frontier_GetHpGaugeFrame(u16 hp, u16 maxHp);
void FrontierTrainer_LoadRecord(FrontierTrainerRecord *record, int trainerId, enum HeapID heapID, NarcId narcId);
void FrontierTrainer_SetBattleTrainer(BattleSetup *battleSetup, FrontierTrainerRecord *record, int unused, int battler);
u32 FrontierMon_GenerateWithRandomOtId(FrontierMon *mon, u16 setId, int itemIndex, u8 iv, int pid, enum HeapID heapID, NarcId narcId);
void FrontierMon_GenerateList(FrontierMon *mons, u16 *setIds, u8 *ivs, u32 *pids, u32 *outPids, int count, enum HeapID heapID, NarcId narcId);
int FrontierTrainer_GetMonIv(u32 trainerIndex);
BOOL FrontierTrainer_PickMonSets(FrontierTrainerEntry *trainer, const u16 *excludedSpecies, const u16 *excludedItems, int numExcluded, int count, u16 *setIds, enum HeapID heapID);
void FrontierTrainer_GenerateMons(u8 count, int trainerIndex, int partnerIndex, u16 *setIds, FrontierMon *mons, u8 *ivs, u32 *outPids, u8 hasPartner);
void Frontier_BufferPartnerName(MessageFormat *msgFmt, int field);
int Frontier_GetPlayerSpriteId(PlayerProfile *profile);
void Frontier_RecordLinkPlayerLocations(SaveData *saveData);

void *FrontierData_AllocMember(s32 fileId, enum HeapID heapID, NarcId narcId) {
    return AllocAndReadWholeNarcMemberByIdPair(narcId, fileId, heapID);
}

void FrontierData_ReadMember(void *dest, s32 fileId, NarcId narcId) {
    ReadWholeNarcMemberByIdPair(dest, narcId, fileId);
}

void *FrontierTrainer_Load(FrontierTrainerRecord *record, int trainerId, enum HeapID heapID, NarcId narcId) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0027_bin, heapID);
    FrontierTrainerEntry *trainer;
    String *string;

    MI_CpuFill8(record, 0, sizeof(FrontierTrainerRecord));
    trainer = FrontierData_AllocMember((u16)trainerId, heapID, narcId);
    record->id = trainerId;
    record->introMsgBank = 0xFFFF;
    record->introMsgNo = trainerId * 3;
    record->trainerClass = trainer->trainerClass;
    string = NewString_ReadMsgData(msgData, trainerId);
    CopyStringToU16Array(string, record->name, 8);
    String_Delete(string);
    DestroyMsgData(msgData);
    return trainer;
}

static const u16 sFrontierMonItems[] = { ITEM_BRIGHTPOWDER, ITEM_LUM_BERRY, ITEM_LEFTOVERS, ITEM_QUICK_CLAW };

u32 FrontierMon_Generate(FrontierMon *mon, u32 setId, u32 otId, u32 pid, u8 iv, u8 itemIndex, BOOL useItemTable, enum HeapID heapID, NarcId narcId) {
    FrontierMonSet set;
    int friendship;

    MI_CpuFill8(mon, 0, sizeof(FrontierMon));
    FrontierData_ReadMember(&set, setId, narcId);
    mon->species = set.species;
    mon->form = set.form;
    if (useItemTable) {
        if (itemIndex >= 4) {
            itemIndex = itemIndex & 3;
        }
        mon->item = sFrontierMonItems[itemIndex];
    } else {
        mon->item = set.item;
    }
    friendship = 255;
    for (int i = 0; i < 4; i++) {
        mon->moves[i] = set.moves[i];
        if (set.moves[i] == MOVE_FRUSTRATION) {
            friendship = 0;
        }
    }
    mon->otId = otId;
    if (pid == 0) {
        do {
            u32 low = LCRandom();
            pid = low | (LCRandom() << 16);
        } while (GetNatureFromPersonality(pid) != set.nature || CalcShininessByOtIdAndPersonality(otId, pid) == TRUE);
        mon->pid = pid;
    } else {
        mon->pid = pid;
    }
    mon->hpIv = iv;
    mon->atkIv = iv;
    mon->defIv = iv;
    mon->spdIv = iv;
    mon->spAtkIv = iv;
    mon->spDefIv = iv;
    int count = 0;
    for (int i = 0; i < 6; i++) {
        if (MaskOfFlagNo(i) & set.evMask) {
            count++;
        }
    }
    int evPerStat = 510 / count;
    if (evPerStat > 255) {
        evPerStat = 255;
    }
    int i = 0;
    u8 ev = evPerStat;
    for (; i < 6; i++) {
        if (MaskOfFlagNo(i) & set.evMask) {
            mon->evs[i] = ev;
        }
    }
    mon->ppUp = 0;
    mon->language = gGameLanguage;
    u32 ability = GetMonBaseStat(mon->species, BASE_ABILITY_2);
    if (ability) {
        if (mon->pid & 1) {
            mon->ability = ability;
        } else {
            mon->ability = GetMonBaseStat(mon->species, BASE_ABILITY_1);
        }
    } else {
        mon->ability = GetMonBaseStat(mon->species, BASE_ABILITY_1);
    }
    mon->friendship = friendship;
    GetSpeciesNameIntoArray(mon->species, heapID, mon->nickname);
    return pid;
}

void FrontierMon_ToPokemon(FrontierMon *frontierMon, Pokemon *mon, int level) {
    u32 ivs;
    u32 otId;
    FrontierMonParam param;
    FrontierMon *iter;

    ZeroMonData(mon);
    if (level == 120) {
        level = 50;
    } else if (level == 121) {
        level = 100;
    }
    ivs = frontierMon->ivsWord & 0x3FFFFFFF;
    CreateMon(mon, frontierMon->species, level, ivs, TRUE, frontierMon->pid, OT_ID_RANDOM_NO_SHINY, 0);
    SetMonData(mon, MON_DATA_COMBINED_IVS, &ivs);
    CalcMonLevelAndStats(mon);
    param.value = frontierMon->form;
    SetMonData(mon, MON_DATA_FORM, &param.value);
    SetMonData(mon, MON_DATA_HELD_ITEM, &frontierMon->item);
    iter = frontierMon;
    int i = 0;
    int shift = 0;
    for (; i < 4; i++, iter = (FrontierMon *)((u8 *)iter + 2), shift += 2) {
        param.move = iter->moves[0];
        SetMonData(mon, MON_DATA_MOVE1 + i, &param.move);
        param.value = (frontierMon->ppUp >> shift) & 3;
        SetMonData(mon, MON_DATA_MOVE1_PP_UPS + i, &param.value);
        param.pp = GetMonData(mon, MON_DATA_MOVE1_MAX_PP + i, NULL);
        SetMonData(mon, MON_DATA_MOVE1_PP + i, &param.pp);
    }
    otId = frontierMon->otId;
    SetMonData(mon, MON_DATA_OT_ID, &otId);
    param.value = frontierMon->evs[0];
    SetMonData(mon, MON_DATA_HP_EV, &param.value);
    param.value = frontierMon->evs[1];
    SetMonData(mon, MON_DATA_ATK_EV, &param.value);
    param.value = frontierMon->evs[2];
    SetMonData(mon, MON_DATA_DEF_EV, &param.value);
    param.value = frontierMon->evs[3];
    SetMonData(mon, MON_DATA_SPEED_EV, &param.value);
    param.value = frontierMon->evs[4];
    SetMonData(mon, MON_DATA_SPATK_EV, &param.value);
    param.value = frontierMon->evs[5];
    SetMonData(mon, MON_DATA_SPDEF_EV, &param.value);
    SetMonData(mon, MON_DATA_ABILITY, &frontierMon->ability);
    SetMonData(mon, MON_DATA_FRIENDSHIP, &frontierMon->friendship);
    if (frontierMon->hasNickname) {
        MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0237_bin, HEAP_ID_FIELD1);
        String *name = NewString_ReadMsgData(msgData, frontierMon->species);
        SetMonData(mon, MON_DATA_NICKNAME_STRING, name);
        String_Delete(name);
        DestroyMsgData(msgData);
    } else {
        SetMonData(mon, MON_DATA_NICKNAME, frontierMon->nickname);
    }
    SetMonData(mon, MON_DATA_LANGUAGE, &frontierMon->language);
    CalcMonLevelAndStats(mon);
}

static const u16 sFrontierTrainerClassSprites[][2] = {
    { TRAINERCLASS_PKMN_TRAINER_CHERYL, SPRITE_SEVEN1       },
    { TRAINERCLASS_PKMN_TRAINER_RILEY,  SPRITE_SEVEN2       },
    { TRAINERCLASS_PKMN_TRAINER_BUCK,   SPRITE_SEVEN3       },
    { TRAINERCLASS_PKMN_TRAINER_MIRA,   SPRITE_SEVEN4       },
    { TRAINERCLASS_PKMN_TRAINER_MARLEY, SPRITE_SEVEN5       },
    { TRAINERCLASS_YOUNGSTER,           SPRITE_BOY2         },
    { TRAINERCLASS_LASS,                SPRITE_GIRL1        },
    { TRAINERCLASS_SCHOOL_KID_M,        SPRITE_BOY1         },
    { TRAINERCLASS_SCHOOL_KID_F,        SPRITE_GIRL3        },
    { TRAINERCLASS_RICH_BOY,            SPRITE_GORGGEOUSM   },
    { TRAINERCLASS_LADY,                SPRITE_GORGGEOUSW   },
    { TRAINERCLASS_CAMPER,              SPRITE_CAMPBOY      },
    { TRAINERCLASS_PICNICKER,           SPRITE_PICNICGIRL   },
    { TRAINERCLASS_TUBER_M,             SPRITE_BABYBOY1     },
    { TRAINERCLASS_TUBER_F,             SPRITE_BABYGIRL1    },
    { TRAINERCLASS_POKEFAN_M,           SPRITE_MIDDLEMAN1   },
    { TRAINERCLASS_POKEFAN,             SPRITE_MIDDLEWOMAN1 },
    { TRAINERCLASS_WAITER,              SPRITE_WAITER       },
    { TRAINERCLASS_WAITRESS,            SPRITE_WAITRESS     },
    { TRAINERCLASS_PKMN_BREEDER_M,      SPRITE_MAN1         },
    { TRAINERCLASS_PKMN_BREEDER_F,      SPRITE_WOMAN1       },
    { TRAINERCLASS_CAMERAMAN,           SPRITE_CAMERAMAN    },
    { TRAINERCLASS_REPORTER,            SPRITE_REPORTER     },
    { TRAINERCLASS_RANCHER,             SPRITE_FARMER       },
    { TRAINERCLASS_COWGIRL,             SPRITE_COWGIRL      },
    { TRAINERCLASS_CYCLIST_M,           SPRITE_CYCLEM       },
    { TRAINERCLASS_CYCLIST_F,           SPRITE_CYCLEW       },
    { TRAINERCLASS_BLACK_BELT,          SPRITE_FIGHTER      },
    { TRAINERCLASS_BATTLE_GIRL,         SPRITE_GIRL2        },
    { TRAINERCLASS_VETERAN,             SPRITE_OLDMAN1      },
    { TRAINERCLASS_SOCIALITE,           SPRITE_LADY         },
    { TRAINERCLASS_PSYCHIC_M,           SPRITE_MYSTERY      },
    { TRAINERCLASS_PSYCHIC_F,           SPRITE_MYSTERY      },
    { TRAINERCLASS_PKMN_RANGER_M,       SPRITE_MAN3         },
    { TRAINERCLASS_PKMN_RANGER_F,       SPRITE_WOMAN3       },
    { TRAINERCLASS_ACE_TRAINER_M,       SPRITE_MAN3         },
    { TRAINERCLASS_ACE_TRAINER_F,       SPRITE_WOMAN3       },
    { TRAINERCLASS_ACE_TRAINER_M_GS,    SPRITE_MAN5         },
    { TRAINERCLASS_ACE_TRAINER_F_GS,    SPRITE_WOMAN5       },
    { TRAINERCLASS_DRAGON_TAMER,        SPRITE_MAN3         },
    { TRAINERCLASS_BUG_CATCHER,         SPRITE_BOY3         },
    { TRAINERCLASS_NINJA_BOY,           SPRITE_BABYBOY1     },
    { TRAINERCLASS_JOGGER,              SPRITE_SPORTSMAN    },
    { TRAINERCLASS_FISHERMAN,           SPRITE_FISHING      },
    { TRAINERCLASS_SAILOR,              SPRITE_SEAMAN       },
    { TRAINERCLASS_HIKER,               SPRITE_MOUNT        },
    { TRAINERCLASS_RUIN_MANIAC,         SPRITE_EXPLORE      },
    { TRAINERCLASS_GUITARIST,           SPRITE_MAN2         },
    { TRAINERCLASS_COLLECTOR,           SPRITE_BIGMAN       },
    { TRAINERCLASS_ROUGHNECK,           SPRITE_BADMAN       },
    { TRAINERCLASS_SUPER_NERD,          SPRITE_ASSISTANTM   },
    { TRAINERCLASS_GENTLEMAN,           SPRITE_GENTLEMAN    },
    { TRAINERCLASS_WORKER,              SPRITE_WORKMAN      },
    { TRAINERCLASS_CLOWN,               SPRITE_CLOWN        },
    { TRAINERCLASS_POLICEMAN,           SPRITE_POLICEMAN    },
    { TRAINERCLASS_PI,                  SPRITE_GORGGEOUSM   },
    { TRAINERCLASS_BIRD_KEEPER,         SPRITE_WOMAN3       },
    { TRAINERCLASS_PARASOL_LADY,        SPRITE_AMBRELLA     },
    { TRAINERCLASS_BEAUTY,              SPRITE_WOMAN2       },
    { TRAINERCLASS_AROMA_LADY,          SPRITE_WOMAN1       },
    { TRAINERCLASS_IDOL,                SPRITE_IDOL         },
    { TRAINERCLASS_ARTIST,              SPRITE_ARTIST       },
    { TRAINERCLASS_POKE_KID,            SPRITE_PIKACHU      },
};

u16 FrontierTrainer_GetClassSprite(u16 value) {
    u32 i = 0;
    const u16 *key = sFrontierTrainerClassSprites[0];

    do {
        if (value == *key) {
            return sFrontierTrainerClassSprites[i][1];
        }
        i++;
        key += 2;
    } while (i < 63);

    return 3;
}

void Frontier_SetRentalMonOt(SaveData *saveData, Pokemon *mon) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(saveData);
    u32 otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
    MsgData *msgData;
    String *string;
    int mapSec;

    MonSetCatchDataNoHealBall(mon, Save_PlayerData_GetProfile(saveData), 4, 0, 0, HEAP_ID_FIELD2);
    mapSec = MapHeader_GetMapSec(MAP_BATTLE_FRONTIER_BATTLE_FACTORY);
    MonSetTrainerMemo(mon, profile, 0, mapSec, HEAP_ID_FIELD2);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0193_bin, HEAP_ID_FIELD2);
    string = NewString_ReadMsgData(msgData, msg_0193_00000);
    SetMonData(mon, MON_DATA_OT_NAME_STRING, string);
    SetMonData(mon, MON_DATA_OT_ID, &otId);
    String_Delete(string);
    DestroyMsgData(msgData);
}

void Frontier_AddRentalMonToParty(SaveData *saveData, Party *party, Pokemon *mon) {
    Frontier_SetRentalMonOt(saveData, mon);
    Party_AddMon(party, mon);
}

void Frontier_SetSpriteAnim(Sprite *sprite, int anim) {
    if (anim != Sprite_GetAnimationNumber(sprite)) {
        Sprite_SetAnimationFrame(sprite, 0);
        Sprite_SetAnimCtrlSeq(sprite, anim);
        Sprite_UpdateAnim(sprite, FX32_ONE);
    }
}

void Frontier_SetSpritePosition(Sprite *sprite, int x, int y, BOOL bounce) {
    VecFx32 position;
    position.x = x << FX32_SHIFT;
    position.y = y << FX32_SHIFT;
    position.z = 0;
    if (bounce == TRUE) {
        if (Sprite_GetAnimationFrame(sprite) == 0) {
            position.y = (y - 3) << FX32_SHIFT;
        } else {
            position.y = (y + 1) << FX32_SHIFT;
        }
    }
    Sprite_SetMatrix(sprite, &position);
}

int Frontier_GetHpGaugeFrame(u16 hp, u16 maxHp) {
    switch (CalculateHpBarColor(hp, maxHp, 48)) {
    case 4:
        return 1;
    case 3:
        return 2;
    case 2:
        return 3;
    case 1:
        return 4;
    default:
        return 1;
    }
}

void FrontierTrainer_LoadRecord(FrontierTrainerRecord *record, int trainerId, enum HeapID heapID, NarcId narcId) {
    Heap_Free(FrontierTrainer_Load(record, trainerId, heapID, narcId));
}

void FrontierTrainer_SetBattleTrainer(BattleSetup *battleSetup, FrontierTrainerRecord *record, int unused, int battler) {
    battleSetup->trainerId[battler] = record->id;
    battleSetup->trainer[battler].data.trainerClass = record->trainerClass;
    CopyU16StringArray(battleSetup->trainer[battler].name, record->name);
    battleSetup->trainer[battler].winMessage.msg_bank = record->messages[0];
    battleSetup->trainer[battler].winMessage.msg_no = record->messages[1];
    battleSetup->trainer[battler].winMessage.fields[0] = record->messages[2];
    battleSetup->trainer[battler].winMessage.fields[1] = record->messages[3];
    battleSetup->trainer[battler].loseMessage.msg_bank = record->messages[4];
    battleSetup->trainer[battler].loseMessage.msg_no = record->messages[5];
    battleSetup->trainer[battler].loseMessage.fields[0] = record->messages[6];
    battleSetup->trainer[battler].loseMessage.fields[1] = record->messages[7];
}

u32 FrontierMon_GenerateWithRandomOtId(FrontierMon *mon, u16 setId, int itemIndex, u8 iv, int pid, enum HeapID heapID, NarcId narcId) {
    u32 otId = LCRandom();
    otId |= (u32)LCRandom() << 16;
    return FrontierMon_Generate(mon, setId, otId, pid, iv, (u8)itemIndex, FALSE, heapID, narcId);
}

void FrontierMon_GenerateList(FrontierMon *mons, u16 *setIds, u8 *ivs, u32 *pids, u32 *outPids, int count, enum HeapID heapID, NarcId narcId) {
    int i;
    u32 pidIter;
    u32 outPidIter;
    u32 generatedPid;
    u8 iv;
    u32 pid;

    i = 0;
    if (count > 0) {
        outPidIter = (u32)outPids;
        pidIter = (u32)pids;
        do {
            iv = (u8)(ivs == NULL ? 0 : ivs[i]);
            if (pids == NULL) {
                pid = 0;
            } else {
                pid = *(u32 *)pidIter;
            }
            generatedPid = FrontierMon_GenerateWithRandomOtId(mons, *setIds, i, iv, pid, heapID, narcId);
            if (outPids) {
                *(u32 *)outPidIter = generatedPid;
            }
            i++;
            mons++;
            pidIter += 4;
            setIds++;
            outPidIter += 4;
        } while (i < count);
    }
}

int FrontierTrainer_GetMonIv(u32 trainerIndex) {
    if (trainerIndex < 100) {
        return 3;
    }
    if (trainerIndex < 120) {
        return 6;
    }
    if (trainerIndex < 140) {
        return 9;
    }
    if (trainerIndex < 160) {
        return 12;
    }
    if (trainerIndex < 180) {
        return 15;
    }
    if (trainerIndex < 200) {
        return 18;
    }
    if (trainerIndex < 220) {
        return 21;
    }
    return 31;
}

// Picks count different sets (no repeated species or item) for a trainer, avoiding the excluded species and items
// for the first 50 tries. Returns TRUE if the exclusion had to be dropped.
// heapID is unused.
BOOL FrontierTrainer_PickMonSets(FrontierTrainerEntry *trainer, const u16 *excludedSpecies, const u16 *excludedItems, int numExcluded, int count, u16 *setIds, enum HeapID heapID) {
    FrontierMonSet sets[6];
    u16 setId;
    int tries;
    int i, j;

    GF_ASSERT(count <= 6);
    i = 0;
    tries = 0;
    while (i != count) {
        setId = trainer->monIds[LCRandom() % trainer->numMons];
        FrontierData_ReadMember(&sets[i], setId, NARC_a_2_0_3);
        for (j = 0; j < i; j++) {
            if (sets[j].species == sets[i].species || sets[j].item == sets[i].item) {
                break;
            }
        }
        if (j != i) {
            continue;
        }
        if (tries < 50) {
            for (j = 0; j < numExcluded; j++) {
                if (sets[i].species == excludedSpecies[j] || sets[i].item == excludedItems[j]) {
                    break;
                }
            }
            if (j != numExcluded) {
                tries++;
                continue;
            }
        }
        setIds[i] = setId;
        i++;
    }
    return tries >= 50;
}

// Generates count Pokemon for a trainer; with hasPartner, the second half comes from partnerIndex's sets and avoids
// the species and items of the first half.
void FrontierTrainer_GenerateMons(u8 count, int trainerIndex, int partnerIndex, u16 *setIds, FrontierMon *mons, u8 *ivs, u32 *outPids, u8 hasPartner) {
    FrontierTrainerRecord record;
    FrontierMonSet set;
    u16 species[4];
    u16 items[4];
    FrontierTrainerEntry *trainer;
    int i;

    trainer = FrontierTrainer_Load(&record, trainerIndex, HEAP_ID_FIELD2, NARC_a_2_0_2);
    for (i = 0; i < count; i++) {
        ivs[i] = FrontierTrainer_GetMonIv(trainerIndex);
    }
    if (hasPartner == FALSE) {
        FrontierTrainer_PickMonSets(trainer, species, items, 0, count, setIds, HEAP_ID_FIELD2);
    } else {
        FrontierTrainer_PickMonSets(trainer, species, items, 0, count / 2, setIds, HEAP_ID_FIELD2);
        for (i = 0; i < count / 2; i++) {
            FrontierData_ReadMember(&set, setIds[i], NARC_a_2_0_3);
            species[i] = set.species;
            items[i] = set.item;
        }
        Heap_Free(trainer);
        trainer = FrontierTrainer_Load(&record, partnerIndex, HEAP_ID_FIELD2, NARC_a_2_0_2);
        FrontierTrainer_PickMonSets(trainer, species, items, count / 2, count / 2, setIds + count / 2, HEAP_ID_FIELD2);
        for (i = 0; i < count / 2; i++) {
            ivs[count / 2 + i] = FrontierTrainer_GetMonIv(partnerIndex);
        }
    }
    Heap_Free(trainer);
    FrontierMon_GenerateList(mons, setIds, ivs, NULL, outPids, count, HEAP_ID_FIELD2, NARC_a_2_0_3);
}

void Frontier_BufferPartnerName(MessageFormat *msgFmt, int field) {
    int netId = Comm_GetLocalNetId() ^ 1;
    BufferPlayersName(msgFmt, field, CommPlayerInfo_GetProfile(netId));
}

int Frontier_GetPlayerSpriteId(PlayerProfile *profile) {
    int gender = PlayerProfile_GetTrainerGender(profile);
    switch (PlayerProfile_GetVersion(profile)) {
    default:
    case 7:
    case 8:
        return gender == 0 ? 0 : 0x61;
    case 0:
        return gender == 0 ? 0xEE : 0xEF;
    case 12:
        return gender == 0 ? 0x127 : 0x128;
    }
}

void Frontier_RecordLinkPlayerLocations(SaveData *saveData) {
    Comm_RecordPlayerLocations(Save_WiFiHistory_Get(saveData));
}
