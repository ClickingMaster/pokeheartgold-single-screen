#ifndef POKEHEARTGOLD_FRONTIER_FRONTIER_RECORDS_H
#define POKEHEARTGOLD_FRONTIER_FRONTIER_RECORDS_H

#include "bg_window.h"

extern const u8 gFrontierRecordsLetterPositions[];

void FrontierRecordsApp_AddWindows(BgConfig *bgConfig, Window *windows, int set);
void FrontierRecordsApp_RemoveWindows(Window *windows, int set);
void FrontierRecordsApp_AddReturnButtonWindow(BgConfig *bgConfig, Window *window);
void FrontierRecordsApp_RemoveReturnButtonWindow(Window *window);

#endif // POKEHEARTGOLD_FRONTIER_FRONTIER_RECORDS_H
