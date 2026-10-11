#ifndef POKEHEARTGOLD_LINK_SYNCHRONIZATION_H
#define POKEHEARTGOLD_LINK_SYNCHRONIZATION_H

#include "heap.h"

void LinkSync_Init(enum HeapID heapID);
BOOL LinkSync_Exists(void);
void LinkSync_StartSync(u8 a0);
BOOL LinkSync_IsSynced(u8 a0);
void LinkSync_SendValue(u32 arg0, u8 arg1);
s32 LinkSync_GetValue(s32 arg0, s32 arg1);
void LinkSync_ClearValues(void);
void LinkSync_ClearPackets(void);
u32 LinkSync_SendPacket(u32 a0, s16 *a1);
u16 *LinkSync_GetPacket(s32 a0);

#endif // POKEHEARTGOLD_LINK_SYNCHRONIZATION_H
