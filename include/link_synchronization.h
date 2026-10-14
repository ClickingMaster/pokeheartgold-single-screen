#ifndef POKEHEARTGOLD_LINK_SYNCHRONIZATION_H
#define POKEHEARTGOLD_LINK_SYNCHRONIZATION_H

#include "heap.h"

void LinkSync_Init(enum HeapID heapID);
BOOL LinkSync_Exists(void);
void LinkSync_StartSync(u8 state);
BOOL LinkSync_IsSynced(u8 state);
void LinkSync_SendValue(u32 key, u8 value);
s32 LinkSync_GetValue(s32 sender, s32 key);
void LinkSync_ClearValues(void);
void LinkSync_ClearPackets(void);
u32 LinkSync_SendPacket(u32 index, s16 *data);
u16 *LinkSync_GetPacket(s32 index);

#endif // POKEHEARTGOLD_LINK_SYNCHRONIZATION_H
