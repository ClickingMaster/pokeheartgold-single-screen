#ifndef POKEHEARTGOLD_VIEW_RANKINGS_TASK_H
#define POKEHEARTGOLD_VIEW_RANKINGS_TASK_H

#include "field_types_def.h"
#include "save.h"

typedef struct ViewRankingsArgs {
    u16 page_scroll;
    u16 cursorPos;
    SaveData *saveData;
} ViewRankingsArgs;

typedef struct ViewRankingsTaskData {
    u32 state;
    ViewRankingsArgs *args;
} ViewRankingsTaskData;

// pageScrollParam = 3 * scroll + page
void FieldSystem_LaunchTask_ViewRankingsApp(FieldSystem *fieldSystem, u8 pageScrollParam, u8 cursorPos);

#endif // POKEHEARTGOLD_VIEW_RANKINGS_TASK_H
