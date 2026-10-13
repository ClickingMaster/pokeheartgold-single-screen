#include "unk_02055418.h"

#include "global.h"

#include "constants/sprites.h"

#include "pokeathlon/pokeathlon_save.h"

#include "apricorn_tree.h"
#include "friend_group.h"
#include "gf_3d_render.h"
#include "map_object.h"
#include "overlay_01.h"
#include "field/field_effect_manager.h"
#include "roamer.h"
#include "sav_system_info.h"
#include "save_misc_data.h"
#include "save_wifi_history.h"
#include "script.h"
#include "script_manager.h"
#include "sys_vars.h"
#include "friend_group_access.h"
#include "unk_02031B0C.h"
#include "frontier_field_support.h"
#include "unk_0206D494.h"
#include "gear_phone_ring_manager.h"
#include "safari_zone_encounters.h"

void FieldSystem_StartBugContestTimer(FieldSystem *fieldSystem) {
    RTCDate date;
    RTCTime time;
    SysInfo_RTC *sysinfo = Save_SysInfo_RTC_Get(fieldSystem->saveData);
    if (!sysinfo->initialized) {
        return;
    }
    GF_RTC_CopyDateTime(&date, &time);
    FieldSystem_CheckDayChange(fieldSystem, sysinfo, &date);
    FieldSystem_CheckTimeAdvance(fieldSystem, sysinfo, &date, &time);
}

void FieldSystem_CheckDayChange(FieldSystem *fieldSystem, SysInfo_RTC *sysinfo, RTCDate *date) {
    u32 day = RTC_ConvertDateToDay(date);
    if (day < (u32)(sysinfo->days_since_nitro_epoch)) {
        sysinfo->days_since_nitro_epoch = day;
        return;
    }
    if (day <= sysinfo->days_since_nitro_epoch) {
        return;
    }
    FieldSystem_HandleDaysPassed(fieldSystem, day - sysinfo->days_since_nitro_epoch);
    sysinfo->days_since_nitro_epoch = day;
}

void FieldSystem_CheckTimeAdvance(FieldSystem *fieldSystem, SysInfo_RTC *sysinfo, RTCDate *date, RTCTime *time) {
    s64 seconds = RTC_ConvertDateTimeToSecond(date, time);
    s64 sys_seconds = RTC_ConvertDateTimeToSecond(&sysinfo->date, &sysinfo->time);
    if (seconds < sys_seconds) {
        sysinfo->date = *date;
        sysinfo->time = *time;
        return;
    }
    s32 delta_seconds = seconds - sys_seconds;
    if (delta_seconds > 0) {
        GearPhoneRingManager_HandleTimeAdvance(FieldSystem_GetGearPhoneRingManager(fieldSystem), seconds);
    }
    s32 minutes = delta_seconds / 60;
    if (minutes <= 0) {
        return;
    }
    Save_SysInfo_RTC_DecrementPenalty(sysinfo, minutes);
    FieldSystem_HandleMinutesPassed(fieldSystem, minutes, time);
    sysinfo->date = *date;
    sysinfo->time = *time;
}

void FieldSystem_HandleDaysPassed(FieldSystem *fieldSystem, int days) {
    BOOL hasPenalty = FieldSystem_HasPenalty(fieldSystem);
    ClearDailyFlags(fieldSystem);
    Save_DullBadges(fieldSystem->saveData, days); // reset badge shininess..?
    Save_FriendGroup_AdvanceSeeds(Save_FriendGroup_Get(fieldSystem->saveData), days);
    Roamers_SetRand(Save_Roamers_Get(fieldSystem->saveData), Save_FriendGroup_GetCurrentSeed(Save_FriendGroup_Get(fieldSystem->saveData)));
    Party_UpdatePokerus(SaveArray_Party_Get(fieldSystem->saveData), days);
    Save_LCRNGAdvanceLotoID(fieldSystem->saveData, (u16)days);
    Save_VarsFlags_RandomizeVar4041(fieldSystem->saveData);
    BattleTower_InitSeed(fieldSystem->saveData);
    WiFiHistory_UpgradeAllLocationsState(Save_WiFiHistory_Get(fieldSystem->saveData));
    FieldSystem_RegrowApricornTrees(fieldSystem);
    PhoneCallPersistentState_BlackBeltKenji_UpdateWaitDays(SaveData_GetPhoneCallPersistentState(fieldSystem->saveData), days);
    if (!hasPenalty) {
        sub_02031CCC(Save_ApricornBox_Get(fieldSystem->saveData), days);
        SaveData_SafariZone_CheckAreasWithUpdatedEncounters(fieldSystem->saveData, days);
    }
    PokeathlonSave_ResetDailyItemsBought(Save_Pokeathlon_Get(fieldSystem->saveData));
    Save_VarsFlags_UpdateBuenasPasswordSet(Save_VarsFlags_Get(fieldSystem->saveData));
}

void FieldSystem_HandleMinutesPassed(FieldSystem *fieldSystem, u32 unkA, RTCTime *time) {
    GearPhoneRingManager_AddMinutes(FieldSystem_GetGearPhoneRingManager(fieldSystem), unkA, 0);
    FieldSystem_IncrementBugContestTimer(fieldSystem, unkA);
    Party_TryResetShaymin(SaveArray_Party_Get(fieldSystem->saveData), unkA, time);
}

TIMEOFDAY Field_GetTimeOfDay(FieldSystem *fieldSystem) {
    return GF_RTC_GetTimeOfDayByHour(Save_SysInfo_RTC_Get(fieldSystem->saveData)->time.hour);
}

TimeOfDayWildParam Field_GetTimeOfDayWildParam(FieldSystem *fieldSystem) {
    return GF_RTC_GetTimeOfDayWildParamByHour(Save_SysInfo_RTC_Get(fieldSystem->saveData)->time.hour);
}

u32 Field_GetMonth(FieldSystem *fieldSystem) {
    return Save_SysInfo_RTC_Get(fieldSystem->saveData)->date.month;
}

u32 Field_GetDay(FieldSystem *fieldSystem) {
    return Save_SysInfo_RTC_Get(fieldSystem->saveData)->date.day;
}

u32 Field_GetHour(FieldSystem *fieldSystem) {
    return Save_SysInfo_RTC_Get(fieldSystem->saveData)->time.hour;
}

void FieldSystem_GetAdventureStartTime(FieldSystem *fieldSystem, RTCDate *date, RTCTime *time) {
    SysInfo_RTC *sysinfo = Save_SysInfo_RTC_Get(fieldSystem->saveData);
    RTC_ConvertSecondToDateTime(date, time, sysinfo->seconds_since_nitro_epoch);
}

void FieldSystem_GetGameClearTime(FieldSystem *fieldSystem, RTCDate *date, RTCTime *time) {
    SysInfo_RTC *sysinfo = Save_SysInfo_RTC_Get(fieldSystem->saveData);
    RTC_ConvertSecondToDateTime(date, time, sysinfo->seconds_at_game_clear);
}

void FieldSystem_SetGameClearTime(FieldSystem *fieldSystem) {
    SysInfo_RTC *sysinfo = Save_SysInfo_RTC_Get(fieldSystem->saveData);
    sysinfo->seconds_at_game_clear = GF_RTC_DateTimeToSec();
}

BOOL FieldSystem_HasPenalty(FieldSystem *fieldSystem) {
    SysInfo_RTC *sysinfo = Save_SysInfo_RTC_Get(fieldSystem->saveData);
    return Save_SysInfo_RTC_HasPenalty(sysinfo);
}

UnkStruct_020556FC *sub_02055680(FieldSystem *fieldSystem, enum HeapID heapID) {
    UnkStruct_020556FC *unkPtr = Heap_Alloc(heapID, sizeof(UnkStruct_020556FC));
    MI_CpuClear8(unkPtr, sizeof(UnkStruct_020556FC));
    unkPtr->heapID = heapID;
    sub_020556C8(fieldSystem, unkPtr);
    return unkPtr;
}

void sub_020556A8(UnkStruct_020556FC *unkPtr) {
    sub_020556FC(unkPtr);
    Heap_Free(unkPtr);
}

void FieldSystem_RegrowApricornTrees(FieldSystem *fieldSystem) {
    APRICORN_TREE *trees = Save_FieldApricornTrees_Get(fieldSystem->saveData);
    ApricornTrees_RegrowEmpty(trees);
}

void sub_020556C8(FieldSystem *fieldSystem, UnkStruct_020556FC *unkPtrB) {
    FieldEffectManager *fieldEffectManager = fieldSystem->fieldEffectManager;
    unkPtrB->modelFile = Heap_Alloc(unkPtrB->heapID, FieldEffectManager_GetNarcMemberSize(fieldEffectManager, 31));
    FieldEffectManager_ReadNarcMember(fieldEffectManager, 31, unkPtrB->modelFile);
    GF3dRender_InitObjFromHeader(&unkPtrB->renderObj, &unkPtrB->model, &unkPtrB->modelFile);
}

void sub_020556FC(UnkStruct_020556FC *unkPtr) {
    FieldEffectManager_HeapFree(unkPtr->modelFile); // function frees modelFile to heap
}

BOOL FieldSystem_ApricornTreeObject_HasApricorn(FieldSystem *fieldSystem, LocalMapObject *mapObject) {
    if (mapObject == NULL) {
        return FALSE;
    }
    u32 id = MapObject_GetSpriteID(mapObject);
    if (id != SPRITE_BONGURI_R && id != SPRITE_BONGURI_Y && id != SPRITE_BONGURI_B && id != SPRITE_BONGURI_G && id != SPRITE_BONGURI_P && id != SPRITE_BONGURI_W && id != SPRITE_BONGURI_BK) {
        return FALSE;
    }
    if (FieldSystem_ApricornTree_HasApricorn(fieldSystem, mapObject) != TRUE) {
        return FALSE;
    }
    return TRUE;
}

BOOL FieldSystem_ApricornTree_PickApricorn(FieldSystem *fieldSystem, LocalMapObject *mapObj) {
    ApricornTrees_PickApricorn(Save_FieldApricornTrees_Get(fieldSystem->saveData), MapObject_GetParam(mapObj, 0));
    return TRUE;
}

BOOL FieldSystem_ApricornTree_HasApricorn(FieldSystem *fieldSystem, LocalMapObject *mapObj) {
    APRICORN_TREE *trees = Save_FieldApricornTrees_Get(fieldSystem->saveData);
    int idx = MapObject_GetParam(mapObj, 0);
    return ApricornTrees_HasApricorn(trees, idx);
}

u32 FieldSystem_ApricornTree_TryGetApricorn(FieldSystem *fieldSystem, LocalMapObject *mapObject) {
    return ApricornTrees_TryGetApricorn(Save_FieldApricornTrees_Get(fieldSystem->saveData), MapObject_GetParam(mapObject, 0));
}

int FieldSystem_ApricornTree_GetApricorn(FieldSystem *fieldSystem, LocalMapObject *mapObject) {
    return ApricornTrees_GetApricorn(Save_FieldApricornTrees_Get(fieldSystem->saveData), MapObject_GetParam(mapObject, 0));
}
