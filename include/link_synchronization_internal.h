#ifndef POKEHEARTGOLD_LINK_SYNCHRONIZATION_INTERNAL_H
#define POKEHEARTGOLD_LINK_SYNCHRONIZATION_INTERNAL_H
#include "global.h"

#include "heap.h"
#include "communication_core.h"
#include "link_synchronization.h"
typedef struct SyncPacket {
    u8 data[0x48];
} SyncPacket;
typedef struct LinkSync {
    u8 values[8][2];
    u8 states[8];
    SyncPacket packets[8];
    u8 received[8];
    u8 resend, completed, requested, pending;
} LinkSync;
extern LinkSync *gLinkSync;
BOOL Comm_SendFixedParentCommand(int, void *);
BOOL Comm_SendFixedCommand(int, void *);
void LinkSync_Init(enum HeapID heap);
void LinkSync_Free(void);
BOOL LinkSync_Exists(void);
void LinkSync_RecvRequest(int sender, int unused, u8 *data);
void LinkSync_RecvState(int sender, int unused, u8 *data);
void LinkSync_RecvDone(int sender, int unused, u8 *data);
void LinkSync_StartSync(u8 state);
void LinkSync_Update(void);
BOOL LinkSync_IsSynced(u8 state);
u8 LinkSync_GetPlayerState(int index);
void LinkSync_RecvValue(int sender, int unused, u8 *data);
u32 LinkSync_GetValueSize(void);
void LinkSync_SendValue(u32 key, u8 value);
s32 LinkSync_GetValue(s32 sender, s32 key);
void LinkSync_ClearValues(void);
void LinkSync_ClearPackets(void);
u32 LinkSync_SendPacket(u32 index, s16 *data);
u16 *LinkSync_GetPacket(s32 index);
void LinkSync_RecvPacket(int sender, int unused, void *data);

#endif
