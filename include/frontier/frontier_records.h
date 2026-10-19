#ifndef POKEHEARTGOLD_FRONTIER_FRONTIER_RECORDS_H
#define POKEHEARTGOLD_FRONTIER_FRONTIER_RECORDS_H

#include "bg_window.h"

void FrontierRecordsApp_AddWindows(BgConfig *bgConfig, Window *windows, int set);
void FrontierRecordsApp_RemoveWindows(Window *windows, int set);
void FrontierRecordsApp_AddReturnButtonWindow(BgConfig *bgConfig, Window *window);
void FrontierRecordsApp_RemoveReturnButtonWindow(Window *window);

#endif // POKEHEARTGOLD_FRONTIER_FRONTIER_RECORDS_H
