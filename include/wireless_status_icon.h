#ifndef POKEHEARTGOLD_WIRELESS_STATUS_ICON_H
#define POKEHEARTGOLD_WIRELESS_STATUS_ICON_H

#include "heap.h"

void WirelessStatusIcon_Create(void);
void WirelessStatusIcon_Destroy(void);
void WirelessStatusIcon_CreateIfConnected(void);
void WirelessStatusIcon_CreateOnScreenIfConnected(int screen);
void WirelessStatusIcon_ShowOnBothScreens(int topFirst, enum HeapID heapID);
void *WirelessStatusIcon_LoadPaletteFile(enum HeapID heapId);

#endif // POKEHEARTGOLD_WIRELESS_STATUS_ICON_H
