#include "frontier/battle_castle_data.h"

#include "global.h"

#include "constants/pokemon.h"

#include "frontier/frontier_map.h"
#include "frontier/frontier_system.h"
#include "frontier/frontier_trainers.h"

#include "bg_window.h"
#include "frontier_save_access.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "party.h"
#include "pokemon.h"
#include "render_window.h"
#include "save.h"
#include "save_vars_flags.h"
#include "sys_vars.h"
#include "text.h"

// The Battle Castle's script data (frontier/battle_castle_data.h): the challenge (party, opponents, saved records),
// the castle points and their windows.

u32 BattleCastle_GetRankStat(u8 type, u8 slot);
u32 BattleCastle_GetWinStreakStat(u32 type);
u32 BattleCastle_GetBestStreakStat(u32 type);
u32 BattleCastle_GetPointsStat(u32 type);
u32 BattleCastle_GetSpentPointsStat(u32 type);
u32 BattleCastle_GetBestStreakPointsStat(u32 type);
u32 FrontierStat_GetFriendIndex(u32 stat);
u32 CastleChallenge_GetField(void *challenge, u32 field, u8 index, u32 a3, u32 a4);
void CastleChallenge_SetField(void *challenge, int field, u8 index, int a3, void *value);
void CastleChallenge_SetSaved(void *challenge, int saved);
void CastleChallenge_Clear(void *challenge);
void FrontierSave_AddStat(FrontierSave *frontierSave, int stat, int substat, u32 amount);
void Frontier_RecordLinkPlayerLocations(SaveData *saveData);
u32 FrontierTrainer_GetClassSprite(u8 trainerClass);
void *FrontierTrainer_Load(void *dst, u16 trainerId, u32 heapId, u32 size);
void FrontierTrainer_GenerateMons(int a0, u16 trainerId, u16 partnerTrainerId, u16 *setIds, void *mons, u8 *ivs, void **personalities, u8 a7);
void FrontierMon_ToPokemon(void *frontierMon, void *mon, int level);
void *CommPlayerInfo_GetProfile(int netId);
u32 Comm_GetLocalNetId(void);
int BattleCastle_GetPartySize(u8 type, int a1);
int BattleCastle_GetOpponentMonCount(int type, int a1);
int BattleCastle_GetMonLevel(void *data);
BOOL BattleCastle_IsMultiType(int type);
void BattleCastle_ReviveParty(void *party);
void BattleCastle_GetRoundTrainerIds(u32 type, u32 round, u16 *trainerIds, int count);
void BattleCastle_AddMonToParty(void *data, void *party, void *mon);
void BattleCastle_SetOpponentParty(void *data);
u16 BattleCastle_GetRound(void *data);
BOOL BattleCastleLink_SendRecord(BattleCastleData *data);
BOOL BattleCastleLink_SendTrainerIds(BattleCastleData *data);
BOOL BattleCastleLink_SendChoice(BattleCastleData *data);
BOOL BattleCastleLink_SendOpponentMons(BattleCastleData *data);
BOOL ov80_0222B8D8(BattleCastleData *data, u16 value);
BOOL ov80_0222B920(BattleCastleData *data, u16 value);
BOOL BattleCastleLink_SendPartyMons(BattleCastleData *data);

extern const u8 ov80_0223BE54[];
extern const u8 ov80_0223BE60[];

void BattleCastleData_SetupNewChallenge(BattleCastleData *data);
void BattleCastleData_RestoreChallenge(BattleCastleData *data);
void BattleCastleData_RestoreParty(BattleCastleData *data);
u16 BattleCastleAppArgs_GetSelection(BattleCastleAppArgs *args, u32 index);
void BattleCastle_DrawPointsWindowFrame(BgConfig *bgConfig, Window *window);
void BattleCastle_PrintPlayerPoints(FrontierSystem *system, Window *window, PlayerProfile *profile, u16 points);

#pragma force_active on
BattleCastleData *BattleCastleData_Alloc(SaveData *saveData, BOOL resume, u8 type, int slot0, u16 slot1, u16 slot2, u16 *resultVar) {
    static BattleCastleData *sBattleCastleData;
    BattleCastleData *data;
    void *challenge;
    Party *party;
    Pokemon *mon;
    FrontierSave *frontierSave;
    u32 castleFlags;
    u16 i;
    u32 item, exp, stat;
    int count;

    sBattleCastleData = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleCastleData));
    MI_CpuFill8(sBattleCastleData, 0, sizeof(BattleCastleData));
    data = sBattleCastleData;
    data->challenge = Save_Frontier_GetCastleChallenge(saveData);
    data->saveData = saveData;
    data->heapId = HEAP_ID_FIELD2;
    data = sBattleCastleData;
    data->playerParty = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
    data->opponentParty = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
    data->resultVar = resultVar;
    challenge = data->challenge;
    castleFlags = Save_Frontier_GetCastleFlags(saveData);
    if (resume == FALSE) {
        u8 hasStreak;
        data = sBattleCastleData;
        data->type = type;
        data->battleNo = 0;
        CastleChallenge_Clear(challenge);
        if (sBattleCastleData->type == 3) {
            hasStreak = Save_VarsFlags_GetVar4052(Save_VarsFlags_Get(sBattleCastleData->saveData));
        } else {
            hasStreak = CastleFlags_GetField(castleFlags, 9, sBattleCastleData->type, 0, 0);
        }
        if (hasStreak == 1) {
            frontierSave = Save_Frontier_GetStatic(sBattleCastleData->saveData);
            stat = BattleCastle_GetWinStreakStat(sBattleCastleData->type);
            sBattleCastleData->winStreak = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetWinStreakStat(sBattleCastleData->type)));
        } else {
            data = sBattleCastleData;
            data->winStreak = 0;
            frontierSave = Save_Frontier_GetStatic(data->saveData);
            stat = BattleCastle_GetPointsStat(sBattleCastleData->type);
            FrontierSave_SetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(data->type)), 0);
            frontierSave = Save_Frontier_GetStatic(sBattleCastleData->saveData);
            stat = BattleCastle_GetSpentPointsStat(type);
            FrontierSave_SetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetSpentPointsStat(type)), 0);
            for (i = 0; i < 3; i++) {
                frontierSave = Save_Frontier_GetStatic(sBattleCastleData->saveData);
                stat = BattleCastle_GetRankStat(sBattleCastleData->type, (u8)i);
                FrontierSave_SetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetRankStat(sBattleCastleData->type, (u8)i)), 1);
            }
        }
        data = sBattleCastleData;
        data->round = data->winStreak / 7;
        data->unk_018 = 0;
        data->partySlots[0] = slot0;
        data->partySlots[1] = slot1;
        data->partySlots[2] = slot2;
        frontierSave = Save_Frontier_GetStatic((*(BattleCastleData *volatile *)&sBattleCastleData)->saveData);
        stat = BattleCastle_GetPointsStat(sBattleCastleData->type);
        data->points = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(data->type)));
        frontierSave = Save_Frontier_GetStatic(sBattleCastleData->saveData);
        stat = BattleCastle_GetPointsStat(sBattleCastleData->type);
        FrontierSave_AddStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(data->type)), 10);
    } else {
        BattleCastleData *saved;
        u32 zero = 0;
        data = sBattleCastleData;
        data->type = CastleChallenge_GetField(challenge, zero, zero, zero, zero);
        data->battleNo = CastleChallenge_GetField(challenge, 1, zero, zero, zero);
        saved = sBattleCastleData;
        frontierSave = Save_Frontier_GetStatic(saved->saveData);
        stat = BattleCastle_GetWinStreakStat(sBattleCastleData->type);
        saved->winStreak = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetWinStreakStat(data->type)));
        saved->round = saved->winStreak / 7;
        for (i = 0; i < 3; i++) {
            u8 slot = CastleChallenge_GetField(challenge, 7, (u8)i, zero, zero);
            sBattleCastleData->partySlots[i] = slot;
        }
    }
    for (i = 0; i < 3; i++) {
        data = sBattleCastleData;
        mon = Party_GetMonByIndex(SaveArray_Party_Get(data->saveData), data->partySlots[i]);
        data = sBattleCastleData;
        data->heldItems[i] = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    }
    party = SaveArray_Party_Get(data->saveData);
    count = BattleCastle_GetPartySize(sBattleCastleData->type, 0);
    for (i = 0; i < count; i++) {
        mon = Party_GetMonByIndex(party, sBattleCastleData->partySlots[i]);
        Party_AddMon(sBattleCastleData->playerParty, mon);
        mon = Party_GetMonByIndex(sBattleCastleData->playerParty, i);
        item = 0;
        SetMonData(mon, MON_DATA_HELD_ITEM, &item);
        if (GetMonData(mon, MON_DATA_LEVEL, NULL) > 50) {
            exp = GetMonExpBySpeciesAndLevel(GetMonData(mon, MON_DATA_SPECIES, NULL), 50);
            SetMonData(mon, MON_DATA_EXPERIENCE, &exp);
            CalcMonLevelAndStats(mon);
        }
    }
    if (BattleCastle_IsMultiType(sBattleCastleData->type) == TRUE) {
        Frontier_RecordLinkPlayerLocations(sBattleCastleData->saveData);
    }
    return sBattleCastleData;
}
#pragma force_active reset

void BattleCastleData_Init(BattleCastleData *data, u32 resume) {
    if (resume == FALSE) {
        BattleCastleData_SetupNewChallenge(data);
    } else {
        BattleCastleData_RestoreChallenge(data);
    }
}

#pragma opt_movepostops off
#pragma opt_common_subs off
#pragma force_active on
void BattleCastleData_SetupNewChallenge(BattleCastleData *data) {
    int count = (u8)Party_GetCount(data->playerParty), i, battleNo;
    u8 *cursor;
    Pokemon *mon;
    int a0;
    u32 isLink;
    i = 0;
    if (count > 0) {
        cursor = (u8 *)data;
        do {
            mon = Party_GetMonByIndex(data->playerParty, i);
            ((BattleCastleData *)cursor)->startPP[0][0] = GetMonData(mon, MON_DATA_MOVE1_PP, NULL);
            ((BattleCastleData *)cursor)->startPP[0][1] = GetMonData(mon, MON_DATA_MOVE2_PP, NULL);
            ((BattleCastleData *)cursor)->startPP[0][2] = GetMonData(mon, MON_DATA_MOVE3_PP, NULL);
            ((BattleCastleData *)cursor)->startPP[0][3] = GetMonData(mon, MON_DATA_MOVE4_PP, NULL);
            i++;
            cursor += sizeof(data->startPP[0]);
        } while (i < count);
    }
    BattleCastle_GetRoundTrainerIds(data->type, BattleCastle_GetRound(data), data->trainerIds, 14);
    battleNo = data->battleNo;
    a0 = BattleCastle_GetOpponentMonCount(data->type, 1);
    isLink = BattleCastle_IsMultiType(data->type);
    FrontierTrainer_GenerateMons(a0, data->trainerIds[battleNo], data->trainerIds[battleNo + 7], data->opponentSetIds, data->opponentMons, data->opponentIVs, (void **)data->opponentPersonalities, (u8)isLink);
}
#pragma force_active reset
#pragma opt_common_subs reset
#pragma opt_movepostops reset

#pragma force_active on
void BattleCastleData_RestoreChallenge(BattleCastleData *data) {
    struct {
        u8 ivs[8];
        u32 personalities[6];
        u16 setIds[6];
        FrontierMon mons[6];
    } opponents;
    int i, j, k;
    u8 *dst, *frontierMon;
    Pokemon *mon;

    BattleCastleData_RestoreParty(data);
    dst = (u8 *)data;
    for (i = 0; i < 14; i++, dst += 2) {
        ((BattleCastleData *)dst)->trainerIds[0] = CastleChallenge_GetField(data->challenge, 6, (u8)i, 0, 0);
    }
    j = 0;
    u16 *setId = opponents.setIds;
    dst = (u8 *)data;
    for (; j < 4; j++, setId++, dst += 2) {
        *setId = CastleChallenge_GetField(data->challenge, 8, (u8)j, 0, 0);
        ((BattleCastleData *)dst)->opponentSetIds[0] = *setId;
    }
    FrontierMon_GenerateList(opponents.mons, opponents.setIds, opponents.ivs, NULL, opponents.personalities, 4, HEAP_ID_FIELD2, 205);
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    k = 0;
    frontierMon = (u8 *)opponents.mons;
    for (; k < 4; k++, frontierMon += sizeof(FrontierMon)) {
        FrontierMon_ToPokemon(frontierMon, mon, BattleCastle_GetMonLevel(data));
        BattleCastle_AddMonToParty(data, data->opponentParty, mon);
    }
    Heap_Free(mon);
}
#pragma force_active reset

void BattleCastleData_RestoreParty(BattleCastleData *data) {
    int count = (u8)Party_GetCount(data->playerParty);
    int i;
    u8 byteValue;
    u16 halfValue;
    u32 wordValue;
    for (i = 0; i < count; i++) {
        Pokemon *mon = Party_GetMonByIndex(data->playerParty, i);
        halfValue = CastleChallenge_GetField(data->challenge, 2, (u8)i, 0, 0);
        SetMonData(mon, MON_DATA_HP, &halfValue);
        byteValue = CastleChallenge_GetField(data->challenge, 3, (u8)i, 0, 0);
        SetMonData(mon, MON_DATA_MOVE1_PP, &byteValue);
        byteValue = CastleChallenge_GetField(data->challenge, 3, (u8)i, 1, 0);
        SetMonData(mon, MON_DATA_MOVE2_PP, &byteValue);
        byteValue = CastleChallenge_GetField(data->challenge, 3, (u8)i, 2, 0);
        SetMonData(mon, MON_DATA_MOVE3_PP, &byteValue);
        byteValue = CastleChallenge_GetField(data->challenge, 3, (u8)i, 3, 0);
        SetMonData(mon, MON_DATA_MOVE4_PP, &byteValue);
        wordValue = CastleChallenge_GetField(data->challenge, 4, (u8)i, 0, 0);
        SetMonData(mon, MON_DATA_STATUS, &wordValue);
        halfValue = CastleChallenge_GetField(data->challenge, 5, (u8)i, 0, 0);
        SetMonData(mon, MON_DATA_HELD_ITEM, &halfValue);
    }
}

void BattleCastleData_Free(BattleCastleData *data) {
    if (data != NULL) {
        if (data->playerParty != NULL) {
            Heap_Free(data->playerParty);
        }
        if (data->opponentParty != NULL) {
            Heap_Free(data->opponentParty);
        }
        MI_CpuFill8(data, 0, sizeof(BattleCastleData));
        Heap_Free(data);
    }
}

#pragma force_active on
void BattleCastleData_ReadAppResult(BattleCastleData *data, BattleCastleAppArgs *args) {
    int i;
    for (i = 0; i < 6; i++) {
        data->selections[i] = BattleCastleAppArgs_GetSelection(args, (u8)i);
    }
    for (i = 0; i < 4; i++) {
        data->unk_370[i] = args->unk_08[i];
        data->unk_374[i] = args->unk_0C[i];
        data->unk_378[i] = args->unk_10[i];
        data->unk_37C[i] = args->unk_14[i];
    }
    data->partnerPoints = args->partnerPoints;
}
#pragma force_active reset

u16 BattleCastleAppArgs_GetSelection(BattleCastleAppArgs *args, u32 index) {
    if (index >= 6) {
        GF_AssertFail();
        return 0;
    }
    return args->selection;
}

#pragma force_active on
void BattleCastleData_Save(BattleCastleData *data, int mode) {
    u32 count;
    u32 castleFlags = Save_Frontier_GetCastleFlags(data->saveData);
    FrontierSave *frontierSave = Save_Frontier_GetStatic(data->saveData);
    struct {
        u16 half;
        u8 filler_02[6];
        u8 byte;
        u8 filler_09[3];
        u32 word;
        u8 filler_10[12];
    } value;
    u16 i;
    u32 best;
    u32 oldBest, points, stat;
    BattleCastle_GetOpponentMonCount(data->type, 1);
    value.byte = data->type;
    CastleChallenge_SetField(data->challenge, 0, 0, 0, &value.byte);
    CastleChallenge_SetSaved(data->challenge, 1);
    value.byte = data->battleNo;
    CastleChallenge_SetField(data->challenge, 1, 0, 0, &value.byte);
    stat = BattleCastle_GetWinStreakStat(data->type);
    FrontierSave_SetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetWinStreakStat(data->type)), data->winStreak);
    if (mode != 2) {
        stat = BattleCastle_GetBestStreakStat(data->type);
        oldBest = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetBestStreakStat(data->type)));
        stat = BattleCastle_GetBestStreakStat(data->type);
        FrontierSave_RaiseStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetBestStreakStat(data->type)), data->winStreak);
        stat = BattleCastle_GetBestStreakStat(data->type);
        best = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetBestStreakStat(data->type)));
        stat = BattleCastle_GetPointsStat(data->type);
        points = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(data->type)));
        if (data->winStreak == oldBest) {
            stat = BattleCastle_GetBestStreakPointsStat(data->type);
            FrontierSave_RaiseStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetBestStreakPointsStat(data->type)), (u16)points);
        } else if (oldBest < best) {
            stat = BattleCastle_GetBestStreakPointsStat(data->type);
            FrontierSave_SetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetBestStreakPointsStat(data->type)), (u16)points);
        }
        value.byte = data->hasStreak;
        CastleFlags_SetField(castleFlags, 9, data->type, 0, &value.byte);
        if (data->type == 3) {
            FrontierSave_SetStat(frontierSave, 108, FrontierStat_GetFriendIndex(108), data->hasStreak);
        }
    }
    for (i = 0; i < 14; i++) {
        value.half = data->trainerIds[i];
        CastleChallenge_SetField(data->challenge, 6, (u8)i, 0, &value.half);
    }
    for (i = 0; i < 3; i++) {
        value.byte = data->partySlots[i];
        CastleChallenge_SetField(data->challenge, 7, (u8)i, 0, &value.byte);
    }
    count = Party_GetCount(data->playerParty);
    for (i = 0; i < count; i++) {
        Pokemon *mon = Party_GetMonByIndex(data->playerParty, i);
        value.half = GetMonData(mon, MON_DATA_HP, NULL);
        CastleChallenge_SetField(data->challenge, 2, (u8)i, 0, &value.half);
        value.byte = GetMonData(mon, MON_DATA_MOVE1_PP, NULL);
        CastleChallenge_SetField(data->challenge, 3, (u8)i, 0, &value.byte);
        value.byte = GetMonData(mon, MON_DATA_MOVE2_PP, NULL);
        CastleChallenge_SetField(data->challenge, 3, (u8)i, 1, &value.byte);
        value.byte = GetMonData(mon, MON_DATA_MOVE3_PP, NULL);
        CastleChallenge_SetField(data->challenge, 3, (u8)i, 2, &value.byte);
        value.byte = GetMonData(mon, MON_DATA_MOVE4_PP, NULL);
        CastleChallenge_SetField(data->challenge, 3, (u8)i, 3, &value.byte);
        value.word = GetMonData(mon, MON_DATA_STATUS, NULL);
        CastleChallenge_SetField(data->challenge, 4, (u8)i, 0, &value.word);
        value.half = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
        CastleChallenge_SetField(data->challenge, 5, (u8)i, 0, &value.half);
    }
    count = Party_GetCount(data->opponentParty);
    for (i = 0; i < count; i++) {
        Party_GetMonByIndex(data->opponentParty, i);
        value.half = data->opponentSetIds[i];
        CastleChallenge_SetField(data->challenge, 8, (u8)i, 0, &value.half);
    }
}
#pragma force_active reset

u8 BattleCastleData_NextBattle(BattleCastleData *data) {
    data->battleNo++;
    return data->battleNo;
}

u8 BattleCastleData_GetBattleNo(BattleCastleData *data) {
    return data->battleNo;
}

u32 BattleCastleData_GetTrainerSprite(BattleCastleData *data, u32 index) {
    u8 trainer[0x30];
    u8 trainerNo = data->battleNo + index * 7;
    void *record = FrontierTrainer_Load(trainer, data->trainerIds[trainerNo], HEAP_ID_FIELD2, 0xCC);
    Heap_Free(record);
    return FrontierTrainer_GetClassSprite((u8) * (u16 *)(trainer + 4));
}

void BattleCastleData_ResetRanks(BattleCastleData *data) {
    int i;
    for (i = 0; i < 3; i++) {
        FrontierSave *frontierSave = Save_Frontier_GetStatic(data->saveData);
        u32 stat = BattleCastle_GetRankStat(data->type, (u8)i);
        u32 substat = FrontierStat_GetFriendIndex(BattleCastle_GetRankStat(data->type, (u8)i));
        FrontierSave_SetStat(frontierSave, stat, substat, 1);
    }
    BattleCastleData_Save(data, 1);
}

void BattleCastleData_EndRound(BattleCastleData *data) {
    data->hasStreak = TRUE;
    if (data->round < 8) {
        data->round++;
    }
    data->battleNo = 0;
    BattleCastleData_Save(data, 0);
}

void BattleCastleData_SetPartyBeforeBattle(BattleCastleData *data) {
    BattleCastle_SetOpponentParty(data);
}

void BattleCastleData_GenerateOpponents(BattleCastleData *data) {
    int a0;
    int isLink;
    int battleNo = data->battleNo;
    a0 = BattleCastle_GetOpponentMonCount(data->type, 1);
    isLink = BattleCastle_IsMultiType(data->type);
    FrontierTrainer_GenerateMons(a0,
        data->trainerIds[battleNo],
        data->trainerIds[battleNo + 7],
        data->opponentSetIds,
        data->opponentMons,
        data->opponentIVs,
        (void **)data->opponentPersonalities,
        (u8)isLink);
    BattleCastle_SetOpponentParty(data);
}

void BattleCastleData_RecordLinkPartyPP(BattleCastleData *data) {
    s32 start = Comm_GetLocalNetId() == 0 ? 0 : 2;
    s32 count = BattleCastle_GetPartySize(data->type, 0);
    s32 end;
    s32 i;
    Party_GetCount(data->playerParty);
    i = start;
    end = count + start;
    for (; i < end; i++) {
        Pokemon *mon = Party_GetMonByIndex(data->playerParty, i);
        u8 *dst = (u8 *)data + (i - start) * sizeof(data->startPP[0]);
        ((BattleCastleData *)dst)->startPP[0][0] = GetMonData(mon, MON_DATA_MOVE1_PP, NULL);
        ((BattleCastleData *)dst)->startPP[0][1] = GetMonData(mon, MON_DATA_MOVE2_PP, NULL);
        ((BattleCastleData *)dst)->startPP[0][2] = GetMonData(mon, MON_DATA_MOVE3_PP, NULL);
        ((BattleCastleData *)dst)->startPP[0][3] = GetMonData(mon, MON_DATA_MOVE4_PP, NULL);
    }
    BattleCastle_ReviveParty(data->playerParty);
    for (i = 0; i < 4; i++) {
        data->unk_370[i] = 0;
        data->unk_374[i] = 0;
        data->unk_378[i] = 0;
        data->unk_37C[i] = 0;
    }
}

#pragma force_active on
// Castle points earned by the last battle: from the party's HP, status and PP left (and data->unk_374).
int BattleCastleData_CalcPointsEarned(BattleCastleData *data) {
    int points;
    int ppLeft;
    int i, j, k;
    int start;
    int end;
    int numMons;
    int numOpponents;
    int ppTotal;
    int ppUsed;
    Pokemon *mon;
    u32 hp, maxHp;
    u8 counts[20];

    points = 0;
    for (i = 0; i < 20; i++) {
        counts[i] = 0;
    }
    ppLeft = 0;
    data->monFainted = FALSE;
    if (Comm_GetLocalNetId() == 0) {
        start = 0;
    } else {
        start = 2;
    }
    numMons = BattleCastle_GetPartySize(data->type, 0);
    numOpponents = BattleCastle_GetOpponentMonCount(data->type, 1);
    end = numMons + start;
    for (i = start; i < end; i++) {
        mon = Party_GetMonByIndex(data->playerParty, i);
        if (GetMonData(mon, MON_DATA_SPECIES_EXISTS, NULL) == 0) {
            continue;
        }
        hp = GetMonData(mon, MON_DATA_HP, NULL);
        maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
        if (hp != 0) {
            counts[0]++;
            if (hp == maxHp) {
                counts[1]++;
            } else if (hp >= maxHp / 2) {
                counts[2]++;
            } else {
                counts[3]++;
            }
            if (GetMonData(mon, MON_DATA_STATUS, NULL) == 0) {
                counts[4]++;
            }
        } else {
            data->monFainted = TRUE;
        }
        ppLeft += GetMonData(mon, MON_DATA_MOVE1_PP, NULL);
        ppLeft += GetMonData(mon, MON_DATA_MOVE2_PP, NULL);
        ppLeft += GetMonData(mon, MON_DATA_MOVE3_PP, NULL);
        ppLeft += GetMonData(mon, MON_DATA_MOVE4_PP, NULL);
    }
    for (i = 0; i < numOpponents; i++) {
        if (data->unk_374[i] == 1) {
            counts[8]++;
        }
    }
    ppTotal = 0;
    for (j = 0; j < numMons; j++) {
        for (k = 0; k < 4; k++) {
            ppTotal += data->startPP[j][k];
        }
    }
    ppUsed = ppTotal - ppLeft;
    if (ppUsed <= 5) {
        counts[5]++;
    } else if (ppUsed <= 10) {
        counts[6]++;
    } else if (ppUsed <= 15) {
        counts[7]++;
    }
    points += counts[0] * 3;
    points += counts[1] * 3;
    points += counts[2] * 2;
    points += counts[3];
    points += counts[4];
    points += counts[5] * 8;
    points += counts[6] * 6;
    points += counts[7] * 4;
    points += counts[8] * 7;
    if (points <= 0) {
        points = 1;
    }
    return points;
}
#pragma force_active reset

void BattleCastle_AddPoints(SaveData *saveData, u32 type, s32 amount) {
    FrontierSave *frontierSave = Save_Frontier_GetStatic(saveData);
    u32 stat = BattleCastle_GetPointsStat(type);
    u32 substat = FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(type));
    {
        s32 points = FrontierSave_GetStat(frontierSave, stat, substat);
        points += amount;
        if (points > 9999) {
            frontierSave = Save_Frontier_GetStatic(saveData);
            stat = BattleCastle_GetPointsStat(type);
            substat = FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(type));
            FrontierSave_SetStat(frontierSave, stat, substat, 9999);
        } else {
            frontierSave = Save_Frontier_GetStatic(saveData);
            stat = BattleCastle_GetPointsStat(type);
            substat = FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(type));
            FrontierSave_AddStat(frontierSave, stat, substat, amount);
        }
    }
    GameStats_Add(Save_GameStats_Get(saveData), 0x42, amount);
}

BOOL BattleCastleData_SendLinkMessage(BattleCastleData *data, u32 kind, u16 value) {
    switch (kind) {
    case 0:
        BattleCastleLink_SendRecord(data);
        break;
    case 1:
        BattleCastleLink_SendTrainerIds(data);
        break;
    case 2:
        BattleCastleLink_SendChoice(data);
        break;
    case 3:
        BattleCastleLink_SendOpponentMons(data);
        break;
    case 4:
        ov80_0222B8D8(data, value);
        break;
    case 5:
        ov80_0222B920(data, value);
        break;
    case 6:
        BattleCastleLink_SendPartyMons(data);
        break;
    }
}

#pragma force_active on
void BattleCastle_ShowPointsWindows(FrontierSystem *system, BattleCastleData *data) {
    FrontierMap *map = FrontierSystem_GetFrontierMap(system);
    if (system->castlePointsWindows[0] != NULL) {
        GF_AssertFail();
    }
    if (system->castlePointsWindows[1] != NULL) {
        GF_AssertFail();
    }
    if (BattleCastle_IsMultiType(data->type) == FALSE) {
        system->castlePointsWindows[0] = AllocWindows(HEAP_ID_FIELD2, 1);
        AddWindowParameterized(map->bgConfig, system->castlePointsWindows[0], GF_BG_LYR_MAIN_1, 1, 1, 10, 4, 14, 595);
        BattleCastle_DrawPointsWindowFrame(map->bgConfig, system->castlePointsWindows[0]);
    } else {
        system->castlePointsWindows[0] = AllocWindows(HEAP_ID_FIELD2, 1);
        system->castlePointsWindows[1] = AllocWindows(HEAP_ID_FIELD2, 1);
        AddWindowParameterized(map->bgConfig, system->castlePointsWindows[0], GF_BG_LYR_MAIN_1, 1, 1, 10, 4, 14, 595);
        AddWindowParameterized(map->bgConfig, system->castlePointsWindows[1], GF_BG_LYR_MAIN_1, 21, 1, 10, 4, 14, 555);
        BattleCastle_DrawPointsWindowFrame(map->bgConfig, system->castlePointsWindows[0]);
        BattleCastle_DrawPointsWindowFrame(map->bgConfig, system->castlePointsWindows[1]);
    }
    BattleCastle_PrintPoints(system, data);
}
#pragma force_active reset

void BattleCastle_DrawPointsWindowFrame(BgConfig *bgConfig, Window *window) {
    DrawFrameAndWindow1(window, TRUE, 0x3D9, 12);
    FillWindowPixelBuffer(window, 15);
}

void BattleCastle_HidePointsWindows(FrontierSystem *system, BattleCastleData *data) {
    Window *first;
    Window *second;
    if (BattleCastle_IsMultiType(data->type) == FALSE) {
        GF_ASSERT(system->castlePointsWindows[0] != NULL);
        first = system->castlePointsWindows[0];
        EraseFrameAndWindow1(first, FALSE);
        WindowArray_Delete(first, 1);
    } else {
        GF_ASSERT(system->castlePointsWindows[0] != NULL);
        GF_ASSERT(system->castlePointsWindows[1] != NULL);
        if (Comm_GetLocalNetId() == 0) {
            first = system->castlePointsWindows[0];
            second = system->castlePointsWindows[1];
        } else {
            first = system->castlePointsWindows[1];
            second = system->castlePointsWindows[0];
        }
        EraseFrameAndWindow1(first, FALSE);
        WindowArray_Delete(first, 1);
        EraseFrameAndWindow1(second, FALSE);
        WindowArray_Delete(second, 1);
    }
    system->castlePointsWindows[0] = NULL;
    system->castlePointsWindows[1] = NULL;
}

#pragma force_active on
void BattleCastle_PrintPoints(FrontierSystem *system, BattleCastleData *data) {
    FrontierSave *frontierSave;
    int stat;
    u16 points, partnerPoints;
    if (!BattleCastle_IsMultiType(data->type)) {
        GF_ASSERT(system->castlePointsWindows[0]);
        frontierSave = Save_Frontier_GetStatic(data->saveData);
        stat = BattleCastle_GetPointsStat(data->type);
        points = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(data->type)));
        BattleCastle_PrintPlayerPoints(system, system->castlePointsWindows[0], Save_PlayerData_GetProfile(data->saveData), points);
    } else {
        GF_ASSERT(system->castlePointsWindows[0]);
        GF_ASSERT(system->castlePointsWindows[1]);
        if (!Comm_GetLocalNetId()) {
            frontierSave = Save_Frontier_GetStatic(data->saveData);
            stat = BattleCastle_GetPointsStat(data->type);
            points = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(data->type)));
            partnerPoints = data->partnerPoints;
        } else {
            points = data->partnerPoints;
            frontierSave = Save_Frontier_GetStatic(data->saveData);
            stat = BattleCastle_GetPointsStat(data->type);
            partnerPoints = FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetPointsStat(data->type)));
        }
        BattleCastle_PrintPlayerPoints(system, system->castlePointsWindows[0], CommPlayerInfo_GetProfile(0), points);
        BattleCastle_PrintPlayerPoints(system, system->castlePointsWindows[1], CommPlayerInfo_GetProfile(1), partnerPoints);
    }
}
#pragma force_active reset

#pragma force_active on
void BattleCastle_PrintPlayerPoints(FrontierSystem *system, Window *window, PlayerProfile *profile, u16 points) {
    String *tmp = String_New(20, system->heapId);
    String *buf = String_New(20, system->heapId);
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 31, HEAP_ID_FIELD2);

    BufferIntegerAsString(system->msgFormat, 0, points, 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    ReadMsgDataIntoString(msgData, 2, tmp);
    StringExpandPlaceholders(system->msgFormat, buf, tmp);
    AddTextPrinterParameterized(window, 0, buf, 16, 16, TEXT_SPEED_NOTRANSFER, NULL);
    BufferPlayersName(system->msgFormat, 0, profile);
    ReadMsgDataIntoString(msgData, 0, tmp);
    StringExpandPlaceholders(system->msgFormat, buf, tmp);
    AddTextPrinterParameterized(window, 0, buf, 0, 0, TEXT_SPEED_INSTANT, NULL);
    String_Delete(tmp);
    String_Delete(buf);
    DestroyMsgData(msgData);
    CopyWindowToVram(window);
}
#pragma force_active reset

// Value of the current round from ov80_0223BE54 / ov80_0223BE60 (0x14 at 21 / 49 wins of the single challenge).
u32 ov80_02233648(BattleCastleData *data) {
    u32 type = data->type;
    u32 round = data->round;
    u32 result;
    if (type <= 1) {
        result = round >= 8 ? 7 : ov80_0223BE54[round];
    } else {
        result = round >= 8 ? 0x12 : ov80_0223BE60[round];
    }
    if (type == 0) {
        u16 winStreak = data->winStreak;
        if (winStreak == 21 || winStreak == 49) {
            result = 0x14;
        }
    }
    return result;
}
