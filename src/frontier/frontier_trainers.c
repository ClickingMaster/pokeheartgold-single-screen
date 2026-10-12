#include "global.h"

#include "constants/moves.h"

#include "battle/battle_setup.h"

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
#include "sprite.h"
#include "string_util.h"
#include "trainer_memo.h"
#include "communication_player_state.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_0208805C.h"

// Frontier trainer and Pokemon generation helpers shared by the facilities: trainers (narc 204) list the ids of their
// Pokemon sets (narc 205); a set becomes a FrontierMon (the record the facilities store) and then a Pokemon.

// First u16s of a trainer's narc member.
typedef struct FrontierTrainer {
    u16 trainerClass;
    u16 numMons;
    u16 monIds[];
} FrontierTrainer;

typedef struct FrontierTrainerRecord {
    u32 id;
    u16 trainerClass;
    u16 unk_06;
    u16 name[8];
    u16 unk_18;
    u16 unk_1A;
    u8 unk_1C[4];
    u16 messages[8]; // win and lose easy chat messages
} FrontierTrainerRecord; // size: 0x30

// Pokemon set (narc 205 member).
typedef struct FrontierMonSet {
    u16 species;
    u16 moves[4];
    u8 evMask;
    u8 nature;
    u16 item;
    u16 form;
} FrontierMonSet;

typedef struct FrontierMon {
    u16 species : 11;
    u16 form : 5;
    u16 item;
    u16 moves[4];
    u32 otId;
    u32 pid;
    u32 hpIv : 5;
    u32 atkIv : 5;
    u32 defIv : 5;
    u32 speedIv : 5;
    u32 spAtkIv : 5;
    u32 spDefIv : 5;
    u32 hasNickname : 1;
    u32 unused : 1;
    u8 evs[6];
    u8 ppUps;
    u8 language;
    u8 ability;
    u8 friendship;
    u16 nickname[11];
} FrontierMon; // size: 0x38

typedef struct FrontierMonParam {
    u8 pp;
    u8 value;
    u16 move;
} FrontierMonParam;

extern const u16 _0223B620[];
extern const u16 gFrontierTrainerClassSprites[];
extern const u16 gFrontierTrainerClassSpriteValues[];

void *FrontierData_AllocMember(s32 fileId, enum HeapID heapID, NarcId narcId);
void FrontierData_ReadMember(void *dest, s32 fileId, NarcId narcId);
void *FrontierTrainer_Load(FrontierTrainerRecord *record, int trainerId, enum HeapID heapID, NarcId narcId);
u32 FrontierMon_Generate(FrontierMon *mon, u32 setId, u32 otId, u32 pid, u8 iv, u8 itemIndex, BOOL useItemTable, enum HeapID heapID, NarcId narcId);
void FrontierMon_ToPokemon(FrontierMon *frontierMon, Pokemon *mon, int level);
u16 FrontierTrainer_GetClassSprite(u16 value);
void ov80_0222A334(SaveData *saveData, Pokemon *mon);
void ov80_0222A3BC(SaveData *saveData, Party *party, Pokemon *mon);
void Frontier_SetSpriteAnim(Sprite *sprite, int anim);
void Frontier_SetSpritePosition(Sprite *sprite, int x, int y, BOOL bounce);
int Frontier_GetHpGaugeFrame(u16 hp, u16 maxHp);
void FrontierTrainer_LoadRecord(FrontierTrainerRecord *record, int trainerId, enum HeapID heapID, NarcId narcId);
void FrontierTrainer_SetBattleTrainer(BattleSetup *battleSetup, FrontierTrainerRecord *record, int unused, int battler);
u32 FrontierMon_GenerateWithRandomOtId(FrontierMon *mon, u16 setId, int itemIndex, u8 iv, int pid, enum HeapID heapID, NarcId narcId);
void FrontierMon_GenerateList(FrontierMon *mons, u16 *setIds, u8 *ivs, u32 *pids, u32 *otIds, int count, enum HeapID heapID, NarcId narcId);
int FrontierTrainer_GetMonIv(u32 trainerIndex);
BOOL FrontierTrainer_PickMonSets(FrontierTrainer *trainer, const u16 *excludedSpecies, const u16 *excludedItems, int numExcluded, int count, u16 *setIds, enum HeapID heapID);
void FrontierTrainer_GenerateMons(u8 count, int trainerIndex, int partnerIndex, u16 *setIds, FrontierMon *mons, u8 *ivs, u32 *otIds, u8 hasPartner);
void Frontier_BufferPartnerName(MessageFormat *msgFmt, int field);
int ov80_0222A7EC(PlayerProfile *profile);
void ov80_0222A840(SaveData *saveData);

void *FrontierData_AllocMember(s32 fileId, enum HeapID heapID, NarcId narcId) {
    return AllocAndReadWholeNarcMemberByIdPair(narcId, fileId, heapID);
}

void FrontierData_ReadMember(void *dest, s32 fileId, NarcId narcId) {
    ReadWholeNarcMemberByIdPair(dest, narcId, fileId);
}

void *FrontierTrainer_Load(FrontierTrainerRecord *record, int trainerId, enum HeapID heapID, NarcId narcId) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 27, heapID);
    FrontierTrainer *trainer;
    String *string;

    MI_CpuFill8(record, 0, sizeof(FrontierTrainerRecord));
    trainer = FrontierData_AllocMember((u16)trainerId, heapID, narcId);
    record->id = trainerId;
    record->unk_18 = 0xFFFF;
    record->unk_1A = trainerId * 3;
    record->trainerClass = trainer->trainerClass;
    string = NewString_ReadMsgData(msgData, trainerId);
    CopyStringToU16Array(string, record->name, 8);
    String_Delete(string);
    DestroyMsgData(msgData);
    return trainer;
}

#pragma force_active on
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
        mon->item = _0223B620[itemIndex];
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
    mon->speedIv = iv;
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
    mon->ppUps = 0;
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
    ivs = *(u32 *)((u8 *)frontierMon + 0x14) & 0x3FFFFFFF;
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
        param.value = (frontierMon->ppUps >> shift) & 3;
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
        MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 237, HEAP_ID_FIELD1);
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
#pragma force_active reset

u16 FrontierTrainer_GetClassSprite(u16 value) {
    u32 i = 0;
    const u16 *key = gFrontierTrainerClassSprites;

    do {
        if (value == *key) {
            return gFrontierTrainerClassSpriteValues[i * 2];
        }
        i++;
        key += 2;
    } while (i < 63);

    return 3;
}

void ov80_0222A334(SaveData *saveData, Pokemon *mon) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(saveData);
    u32 otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
    MsgData *msgData;
    String *string;
    int mapSec;

    sub_0207217C(mon, Save_PlayerData_GetProfile(saveData), 4, 0, 0, HEAP_ID_FIELD2);
    mapSec = MapHeader_GetMapSec(275);
    MonSetTrainerMemo(mon, profile, 0, mapSec, HEAP_ID_FIELD2);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 193, HEAP_ID_FIELD2);
    string = NewString_ReadMsgData(msgData, 0);
    SetMonData(mon, MON_DATA_OT_NAME_STRING, string);
    SetMonData(mon, MON_DATA_OT_ID, &otId);
    String_Delete(string);
    DestroyMsgData(msgData);
}

void ov80_0222A3BC(SaveData *saveData, Party *party, Pokemon *mon) {
    ov80_0222A334(saveData, mon);
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
    FrontierTrainerRecord *source = record;
    u8 *setup = (u8 *)battleSetup;
    int offset;
    u8 *nameDest;
    u16 *nameSource;
    *(u32 *)(setup + 0x18 + battler * 4) = source->id;
    offset = battler;
    offset *= sizeof(Trainer);
    (setup + offset)[0x29] = source->trainerClass;
    nameDest = setup;
    nameDest += 0x3C;
    nameSource = (u16 *)source;
    nameDest += offset;
    nameSource += 4;
    CopyU16StringArray((u16 *)nameDest, nameSource);
    *(u16 *)(setup + offset + 0x4C) = source->messages[0];
    *(u16 *)(setup + offset + 0x4E) = source->messages[1];
    *(u16 *)(setup + offset + 0x50) = source->messages[2];
    *(u16 *)(setup + offset + 0x52) = source->messages[3];
    *(u16 *)(setup + offset + 0x54) = source->messages[4];
    *(u16 *)(setup + offset + 0x56) = source->messages[5];
    *(u16 *)(setup + offset + 0x58) = source->messages[6];
    *(u16 *)(setup + offset + 0x5A) = source->messages[7];
}

#pragma force_active on
u32 FrontierMon_GenerateWithRandomOtId(FrontierMon *mon, u16 setId, int itemIndex, u8 iv, int pid, enum HeapID heapID, NarcId narcId) {
    u32 otId = LCRandom();
    otId |= (u32)LCRandom() << 16;
    return FrontierMon_Generate(mon, setId, otId, pid, iv, (u8)itemIndex, FALSE, heapID, narcId);
}

void FrontierMon_GenerateList(FrontierMon *mons, u16 *setIds, u8 *ivs, u32 *pids, u32 *otIds, int count, enum HeapID heapID, NarcId narcId) {
    int i;
    u32 pidIter;
    u32 otIdIter;
    u32 otId;
    u8 iv;
    u32 pid;

    i = 0;
    if (count > 0) {
        otIdIter = (u32)otIds;
        pidIter = (u32)pids;
        do {
            iv = (u8)(ivs == NULL ? 0 : ivs[i]);
            if (pids == NULL) {
                pid = 0;
            } else {
                pid = *(u32 *)pidIter;
            }
            otId = FrontierMon_GenerateWithRandomOtId(mons, *setIds, i, iv, pid, heapID, narcId);
            if (otIds) {
                *(u32 *)otIdIter = otId;
            }
            i++;
            mons++;
            pidIter += 4;
            setIds++;
            otIdIter += 4;
        } while (i < count);
    }
}
#pragma force_active reset

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

#pragma force_active on
// Picks count different sets (no repeated species or item) for a trainer, avoiding the excluded species and items
// for the first 50 tries. Returns TRUE if the exclusion had to be dropped.
// heapID is unused.
BOOL FrontierTrainer_PickMonSets(FrontierTrainer *trainer, const u16 *excludedSpecies, const u16 *excludedItems, int numExcluded, int count, u16 *setIds, enum HeapID heapID) {
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
void FrontierTrainer_GenerateMons(u8 count, int trainerIndex, int partnerIndex, u16 *setIds, FrontierMon *mons, u8 *ivs, u32 *otIds, u8 hasPartner) {
    FrontierTrainerRecord record;
    FrontierMonSet set;
    u16 species[4];
    u16 items[4];
    FrontierTrainer *trainer;
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
    FrontierMon_GenerateList(mons, setIds, ivs, NULL, otIds, count, HEAP_ID_FIELD2, NARC_a_2_0_3);
}
#pragma force_active reset

void Frontier_BufferPartnerName(MessageFormat *msgFmt, int field) {
    int netId = Comm_GetLocalNetId() ^ 1;
    BufferPlayersName(msgFmt, field, CommPlayerInfo_GetProfile(netId));
}

int ov80_0222A7EC(PlayerProfile *profile) {
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

void ov80_0222A840(SaveData *saveData) {
    sub_02039F68(Save_WiFiHistory_Get(saveData));
}
