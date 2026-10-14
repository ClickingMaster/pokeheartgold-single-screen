#include "trainer_card_data.h"

#include "global.h"

#include "constants/game_stats.h"

#include "frontier_save_access.h"
#include "game_stats.h"
#include "heap.h"
#include "launch_application.h"
#include "player_data.h"
#include "pokedex.h"
#include "save_trainer_card.h"
#include "string_util.h"
#include "sys_flags.h"
#include "task.h"
#include "union_room_talk.h"
#include "field_time_events.h"

typedef struct LinkTrainerCardTaskEnv {
    int state;
    TrainerCardData *partnerCard;
    TrainerCardAppArgs args;
} LinkTrainerCardTaskEnv;

TrainerCardData *TrainerCardData_New(enum HeapID heapID);
void TrainerCardData_SetHeader(u8 unk_04_0, u32 version, u32 stars, u32 unk_02, u8 language, TrainerCardData *card);
void TrainerCardData_SetProfile(u32 trainerId, u8 gender, const u16 *name, u32 money, u32 pokedexCount, u32 pokedexObtained, u32 score, TrainerCardData *card);
void TrainerCardData_SetTimes(u8 gameCleared, IGT *igt, const RTCDate *startDate, const RTCDate *clearDate, const RTCTime *clearTime, u8 liveIgt, TrainerCardData *card);
void TrainerCardData_SetRecords(u32 linkCount, u32 linkBattleWins, u32 linkBattleLosses, u32 linkTrades, u8 signatureExists, const void *signature, TrainerCardData *card);
void TrainerCardData_SetBadges(PlayerProfile *profile, FieldSystem *fieldSystem, TrainerCardData *card);
BOOL Task_LinkTrainerCard(TaskManager *taskManager);

static inline u32 GameStats_GetLinkCount(GameStats *stats) {
    return GameStats_GetCapped(stats, GAME_STAT_UNK33)
        + (GameStats_GetCapped(stats, GAME_STAT_WIFI_BATTLES)
            + (GameStats_GetCapped(stats, GAME_STAT_LOCAL_LINK_BATTLES)
                + (GameStats_GetCapped(stats, GAME_STAT_WIFI_TRADES)
                    + (GameStats_GetCapped(stats, GAME_STAT_COMM_CONTEST_ENTRIES) + GameStats_GetCapped(stats, GAME_STAT_LOCAL_LINK_TRADES)))));
}

#pragma force_active on

void TrainerCardData_Build(u8 unk_04_0, u8 liveIgt, u8 unk_02, u8 caseVariant, FieldSystem *fieldSystem, TrainerCardAppArgs *trainerCard) {
    SaveData *save;
    PlayerProfile *profile;
    GameStats *stats;
    IGT *igt;
    RTCDate startDate;
    RTCDate clearDate;
    RTCTime clearTime;
    struct SaveTrainerCard *saveCard;
    u32 i;
    u32 checksum;
    TrainerCardData *card = (TrainerCardData *)trainerCard;

    save = FieldSystem_GetSaveData(fieldSystem);
    profile = Save_PlayerData_GetProfile(save);
    stats = Save_GameStats_Get(save);
    memset(card, 0, sizeof(TrainerCardData));
    card->caseVariant = caseVariant;
    TrainerCardData_SetHeader(unk_04_0, GAME_VERSION, FieldSystem_GetTrainerCardStars(fieldSystem), unk_02, PlayerProfile_GetLanguage(profile), card);
    TrainerCardData_SetProfile(PlayerProfile_GetTrainerID_VisibleHalf(profile), PlayerProfile_GetTrainerGender(profile), PlayerProfile_GetNamePtr(profile), PlayerProfile_GetMoney(profile), Pokedex_CountDexOwned(Save_Pokedex_Get(fieldSystem->saveData)), Pokedex_IsEnabled(Save_Pokedex_Get(fieldSystem->saveData)), GameStats_GetScore(stats), card);
    igt = Save_PlayerData_GetIGTAddr(save);
    FieldSystem_GetAdventureStartTime(fieldSystem, &startDate, &clearTime);
    FieldSystem_GetGameClearTime(fieldSystem, &clearDate, &clearTime);
    TrainerCardData_SetTimes(CheckGameClearFlag(Save_VarsFlags_Get(fieldSystem->saveData)), igt, &startDate, &clearDate, &clearTime, liveIgt, card);
    saveCard = Save_TrainerCard_Get(fieldSystem->saveData);
    TrainerCardData_SetRecords(GameStats_GetLinkCount(stats),
        GameStats_GetCapped(stats, GAME_STAT_LOCAL_LINK_BATTLE_WINS) + GameStats_GetCapped(stats, GAME_STAT_WIFI_BATTLE_WINS),
        GameStats_GetCapped(stats, GAME_STAT_LOCAL_LINK_BATTLE_LOSSES) + GameStats_GetCapped(stats, GAME_STAT_WIFI_BATTLE_LOSSES),
        GameStats_GetCapped(stats, GAME_STAT_LOCAL_LINK_TRADES) + GameStats_GetCapped(stats, GAME_STAT_WIFI_TRADES),
        TrainerCard_SignatureExists(saveCard),
        TrainerCard_GetSignature(saveCard),
        card);
    TrainerCardData_SetBadges(profile, fieldSystem, card);
    checksum = 0;
    for (i = 0; i < sizeof(TrainerCardData); i++) {
        checksum ^= trainerCard->unk0[i];
    }
    card->checksum = checksum;
    card->received = 0;
}

#pragma force_active off

TrainerCardData *TrainerCardData_New(enum HeapID heapID) {
    TrainerCardData *card = Heap_Alloc(heapID, sizeof(TrainerCardData));
    memset(card, 0, sizeof(TrainerCardData));
    return card;
}

TrainerCardAppArgs *TrainerCardAppArgs_New(enum HeapID heapID) {
    TrainerCardAppArgs *args = Heap_Alloc(heapID, sizeof(TrainerCardAppArgs));
    memset(args, 0, sizeof(TrainerCardAppArgs));
    return args;
}

void TrainerCardData_Free(TrainerCardAppArgs *trainerCard) {
    Heap_Free(trainerCard);
}

u8 FieldSystem_GetTrainerCardStars(FieldSystem *fieldSystem) {
    SaveData *saveData = FieldSystem_GetSaveData(fieldSystem);
    SaveVarsFlags *varsFlags;
    u8 stars;
    FrontierSave *frontier;

    Save_GameStats_Get(saveData);
    varsFlags = Save_VarsFlags_Get(saveData);
    frontier = Save_Frontier_GetStatic(saveData);
    stars = 0;

    if (CheckGameClearFlag(varsFlags)) {
        stars++;
    }
    if (Pokedex_NationalDexIsComplete(Save_Pokedex_Get(saveData))) {
        stars++;
    }
    if (FrontierSave_GetStat(frontier, 0, 0xFF) >= 100
        || FrontierSave_GetStat(frontier, 2, 0xFF) >= 100
        || FrontierSave_GetStat(frontier, 4, 0xFF) >= 100
        || FrontierSave_GetStat(frontier, 6, 0xFF) >= 100
        || FrontierSave_GetStat(frontier, 8, 0xFF) >= 100) {
        stars++;
    }
    if (Save_VarsFlags_CheckFlagInArray(varsFlags, 0xF1)) {
        stars++;
    }
    if (Save_VarsFlags_CheckFlagInArray(varsFlags, 0x184)) {
        stars++;
    }
    return stars;
}

void TrainerCardData_SetHeader(u8 unk_04_0, u32 version, u32 stars, u32 unk_02, u8 language, TrainerCardData *card) {
    card->unk_04_0 = unk_04_0;
    card->version = version;
    card->stars = stars;
    card->language = language;
    card->unk_02 = unk_02;
}

void TrainerCardData_SetProfile(u32 trainerId, u8 gender, const u16 *name, u32 money, u32 pokedexCount, u32 pokedexObtained, u32 score, TrainerCardData *card) {
    card->trainerId = trainerId;
    card->gender = gender;
    CopyU16StringArrayN(card->name, name, 8);
    card->money = money;
    card->pokedexCount = pokedexCount;
    card->pokedexObtained = (u8)pokedexObtained;
    card->score = score;
}

void TrainerCardData_SetTimes(u8 gameCleared, IGT *igt, const RTCDate *startDate, const RTCDate *clearDate, const RTCTime *clearTime, u8 liveIgt, TrainerCardData *card) {
    card->playHours = GetIGTHours(igt);
    card->playMinutes = GetIGTMinutes(igt);
    card->startYear = startDate->year;
    card->startMonth = startDate->month;
    card->startDay = startDate->day;
    if (gameCleared) {
        card->clearYear = clearDate->year;
        card->clearMonth = clearDate->month;
        card->clearDay = clearDate->day;
        card->clearHours = clearTime->hour;
        card->clearMinutes = clearTime->minute;
    } else {
        card->clearYear = 0;
        card->clearMonth = 0;
        card->clearDay = 0;
        card->clearHours = 0;
        card->clearMinutes = 0;
    }
    card->liveIgt = liveIgt;
    if (liveIgt) {
        card->igt = igt;
    } else {
        card->igt = NULL;
    }
}

void TrainerCardData_SetRecords(u32 linkCount, u32 linkBattleWins, u32 linkBattleLosses, u32 linkTrades, u8 signatureExists, const void *signature, TrainerCardData *card) {
    card->linkCount = linkCount;
    if (linkCount > 999999) {
        card->linkCount = 999999;
    }
    card->linkBattleWins = linkBattleWins;
    card->linkBattleLosses = linkBattleLosses;
    if (card->linkBattleWins > 9999) {
        card->linkBattleWins = 9999;
    }
    if (card->linkBattleLosses > 9999) {
        card->linkBattleLosses = 9999;
    }
    card->linkTrades = linkTrades;
    if (linkTrades > 99999) {
        card->linkTrades = 99999;
    }
    card->signatureExists = signatureExists;
    MI_CpuCopy8(signature, card->signature, sizeof(card->signature));
}

void TrainerCardData_SetBadges(PlayerProfile *profile, FieldSystem *fieldSystem, TrainerCardData *card) {
    u32 *words;
    u8 i;
    u16 mask;
    u8 badge;

    TrainerCard_GetBadgeShininessArr(Save_TrainerCard_Get(fieldSystem->saveData));
    i = 0;
    words = card->badgeShininess;
    for (; i < 8; i++) {
        words[i] &= ~1;
        words[i] &= 1;
    }
    mask = 1;
    for (badge = 0; badge < 16; badge++) {
        if (PlayerProfile_TestBadgeFlag(profile, badge)) {
            card->badges |= mask;
        }
        mask <<= 1;
    }
}

void LinkTrainerCard_StartTask(FieldSystem *fieldSystem) {
    LinkTrainerCardTaskEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(LinkTrainerCardTaskEnv));
    env->state = 0;
    env->partnerCard = UnionRoom_AllocTrainerCards(fieldSystem->unionRoom);
    TaskManager_Call(fieldSystem->taskman, Task_LinkTrainerCard, env);
}

BOOL Task_LinkTrainerCard(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    LinkTrainerCardTaskEnv *env = TaskManager_GetEnvironment(taskManager);

    switch (env->state) {
    case 0:
        UnionRoom_SendTrainerCard(fieldSystem->unionRoom);
        env->state = 1;
    case 1:
        if (env->partnerCard->received != 0) {
            env->state = 10;
        }
        break;
    case 10:
        MI_CpuCopy8(env->partnerCard, &env->args, sizeof(TrainerCardData));
        TrainerCard_LaunchApp(fieldSystem, &env->args);
        env->state = 11;
        break;
    case 11:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            UnionRoom_FreeTrainerCards(fieldSystem->unionRoom);
            Heap_Free(env);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
