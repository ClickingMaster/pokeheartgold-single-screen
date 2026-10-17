#ifndef POKEHEARTGOLD_APPLICATION_WII_MESSAGE_WII_NUMBER_H
#define POKEHEARTGOLD_APPLICATION_WII_MESSAGE_WII_NUMBER_H

#include "heap.h"
#include "pm_string.h"

void WiiNumber_ToWiiAddress(String *wiiNumber, char *address, enum HeapID heapID);
BOOL WiiNumber_IsZero(String *wiiNumber, enum HeapID heapID);
void WiiNumber_ParseDigitGroups(char *address, u32 *groups, enum HeapID unused);

#endif // POKEHEARTGOLD_APPLICATION_WII_MESSAGE_WII_NUMBER_H
