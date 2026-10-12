#ifndef POKEHEARTGOLD_WIFI_CLUB_H
#define POKEHEARTGOLD_WIFI_CLUB_H

#include "task.h"

BOOL WifiClub_IsMatchingActivity(s32 netId);
void CallTask_WifiClub(TaskManager *taskManager);
void CallTask_WifiClubWithResult(TaskManager *taskManager, u16 *result);

#endif // POKEHEARTGOLD_WIFI_CLUB_H
