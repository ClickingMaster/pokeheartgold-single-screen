#ifndef POKEHEARTGOLD_FIELD_TIME_EVENTS_H
#define POKEHEARTGOLD_FIELD_TIME_EVENTS_H

#include "heap.h"
#include "sav_system_info.h"
#include "script.h"

// Berry Patch Manager in pokeplatinum. Definitely used for something else here.
typedef struct UnusedFieldModel {
    enum HeapID heapID;
    NNSG3dRenderObj renderObj;
    NNSG3dResMdl *model;
    NNSG3dResFileHeader *modelFile;
} UnusedFieldModel; // size 0x60

void FieldSystem_StartBugContestTimer(FieldSystem *fieldSystem);
void FieldSystem_CheckDayChange(FieldSystem *fieldSystem, SysInfo_RTC *sysinfo, RTCDate *date);
void FieldSystem_CheckTimeAdvance(FieldSystem *fieldSystem, SysInfo_RTC *sysinfo, RTCDate *date, RTCTime *time);
void FieldSystem_HandleDaysPassed(FieldSystem *fieldSystem, int unkA);
void FieldSystem_HandleMinutesPassed(FieldSystem *fieldSystem, u32 unkA, RTCTime *time);
TIMEOFDAY Field_GetTimeOfDay(FieldSystem *fieldSystem);
TimeOfDayWildParam Field_GetTimeOfDayWildParam(FieldSystem *fieldSystem);
u32 Field_GetMonth(FieldSystem *fieldSystem);
u32 Field_GetDay(FieldSystem *fieldSystem);
u32 Field_GetHour(FieldSystem *fieldSystem);
void FieldSystem_GetAdventureStartTime(FieldSystem *fieldSystem, RTCDate *date, RTCTime *time);
void FieldSystem_GetGameClearTime(FieldSystem *fieldSystem, RTCDate *date, RTCTime *time);
void FieldSystem_SetGameClearTime(FieldSystem *fieldSystem);
BOOL FieldSystem_HasPenalty(FieldSystem *fieldSystem);
UnusedFieldModel *UnusedFieldModel_New(FieldSystem *fieldSystem, enum HeapID heapID);
void UnusedFieldModel_Delete(UnusedFieldModel *unkPtr);
void FieldSystem_RegrowApricornTrees(FieldSystem *fieldSystem);
void UnusedFieldModel_Load(FieldSystem *fieldSystem, UnusedFieldModel *unkPtrB);
void UnusedFieldModel_Unload(struct UnusedFieldModel *unkPtr);
BOOL FieldSystem_ApricornTreeObject_HasApricorn(FieldSystem *fieldSystem, LocalMapObject *mapObject);
BOOL FieldSystem_ApricornTree_PickApricorn(FieldSystem *fieldSystem, LocalMapObject *mapObj);
BOOL FieldSystem_ApricornTree_HasApricorn(FieldSystem *fieldSystem, LocalMapObject *mapObj);
u32 FieldSystem_ApricornTree_TryGetApricorn(FieldSystem *fieldSystem, LocalMapObject *mapObject);
int FieldSystem_ApricornTree_GetApricorn(FieldSystem *fieldSystem, LocalMapObject *mapObject);

#endif // POKEHEARTGOLD_FIELD_TIME_EVENTS_H
