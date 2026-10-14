#ifndef POKEHEARTGOLD_COMMUNICATION_CORE_INTERNAL_H
#define POKEHEARTGOLD_COMMUNICATION_CORE_INTERNAL_H

#include "global.h"

// Communication core (ARM9 static 0x02035900-0x020379A0): the work area of the active link, the per-frame update,
// parent/child frame exchange and the command send queues.

typedef struct CommCore {
    u8 initialized; // the parent frame for the current slot is filled in
    u8 updateLock;  // set by the frame update, cleared by the VBlank send task
    u8 unk_02[2];
    void *allocation;
    u8 *work; // 32-byte aligned inside allocation, 0x6C0 bytes
} CommCore;

// Receive state of one command stream (a command id, its size and the bytes received so far).
typedef struct PacketCursor {
    int received;
    u8 *data;
    u16 size;
    u8 command; // 0xEE: none
    u8 unk_0B;
} PacketCursor;

typedef struct ReceiveBuffer {
    u8 *dest;
    int remaining;
} ReceiveBuffer;

// 64-bit linear congruential generator.
typedef struct CommRandom {
    s64 state;
    s64 multiplier;
    s64 increment;
} CommRandom;

// Byte ring owned by unk_02033AE0 (only the read position is used here).
typedef struct CommRing {
    u8 *buffer;
    s16 read;
} CommRing;

// Send stage of the parent frame ([0]) and the child frame ([1]): 4 = idle, 0/2 = ready to send, odd = sending.
// The object is word-aligned and 4 bytes long in the ROM.
typedef union CommSendStages {
    volatile u8 stage[4];
    u32 align;
} CommSendStages;

extern CommCore gCommCore;
extern CommSendStages gCommSendStages;

int Comm_InitWork(int enabled, int value);
void Comm_ResetBuffers(void);
void Comm_Reset(void);
void Comm_OnTransferModeChanged(void);
void Comm_ResetNetIdBuffers(int slot);
void Comm_ResetDisconnectedNetIds(void);
void Comm_OnChildJoined(int slot);
int Comm_StartParent(int a, int b, int c, int d);
int Comm_StartScan(int a, int b, int c);
void Comm_UpdateTransferMode(void);
void Comm_RequestTransferMode(int connected);
void Comm_RequestTransferMode1(void);
void Comm_RequestTransferMode0(void);
int Comm_GetTransferMode(void);
BOOL Comm_IsTransferMode1(void);
void Comm_Shutdown(void);
int Comm_ConnectToParent(int value);
void Comm_VBlankSendTask(void);
void Comm_ShutdownWhenRequested(void);
BOOL Comm_Update(void);
void Comm_ResetWork(void);
void Comm_ResetInTransferMode1(void);
void Comm_ResetWorkAndParents(void);
void Comm_UpdateChildSend(void);
BOOL Comm_FillParentFrameFromChildren(int slot);
void Comm_SendParentFrame(void);
BOOL Comm_HasNoChildBacklog(void);
void Comm_UpdateParentSend(void);
void Comm_RecvParentFrameCallback(int netId, const u8 *data, int size);
void Comm_OnParentFrameReceived(int netId, const u8 *data, int size);
void Comm_RecvChildFrameCallback(int netId, const u8 *data, int size);
void Comm_OnChildFrameReceived(int slot, const u8 *data, int size);
void Comm_RecvPeerFrameCallback(int slot, const u8 *data);
void Comm_OnChildFrameSent(void *ptr);
void Comm_OnParentFrameSent(void *ptr);
void Comm_SendChildFrame(void);
void Comm_ScrambleDirectionKeys(void);
int Comm_ReadFrameKeys(const u8 *data, int slot);
int Comm_WriteFrameKeys(u8 *data);
int Comm_BuildChildFrame(u8 *data);
void Comm_BuildParentFrame(u8 *data);
BOOL Comm_IsSendSkippedThisFrame(void);
int Comm_SendCommandNoCopy(int command, void *data, int size);
int Comm_SendCommand(int command, void *data, int size);
int Comm_SendParentCommandNoCopy(int command, void *data, int size);
int Comm_SendParentCommand(void *command, void *data, int size);
int Comm_SendFixedParentCommand(int command, void *data);
int Comm_GetSendBufferFreeSpace(void);
void Comm_FinishCommand(int netId, int command, int size, void *data, PacketCursor *cursor);
void Comm_ReceiveCommands(CommRing *ring, int netId, u8 *buffer, PacketCursor *cursor);
void Comm_ReceiveFromParent(void);
void Comm_ReceiveFromChildren(void);
int Comm_IsNetIdConnected(int slot);
int Comm_CountConnected(void);
int Comm_IsActive(void);
void Comm_SetLocalMoveSpeed(int value);
int Comm_GetMoveSpeed(int slot);
u16 Comm_TakeDirectionKeys(int slot);
void Comm_EnableKeySync(void);
void Comm_DisableKeySync(void);
int Comm_IsKeySyncEnabled(void);
int Comm_SendParentCommandNoCopyUnchecked(int command, void *data, int size);
void Comm_UpdateTransferModeHandshake(void);
void Comm_RecvTransferModeRequest(int netId, int size, const u8 *data);
void Comm_RecvTransferModeChange(int netId, int size, const u8 *data);
void Comm_RecvTransferModeAck(int netId, int size, const u8 *data);
int Comm_GetLocalNetId(void);
int Comm_SendFixedCommandNoCopy(int command, void *data);
int Comm_SendFixedCommand(int command, void *data);
int Comm_SendEmptyCommand(int command);
int Comm_GetConnectedChildren(void);
int Comm_HasLinkError(void);
int Comm_GetChildFrameSize(u16 trainerId);
int Comm_GetMaxPlayers(int value);
int Comm_GetMinPlayers(int value);
void Comm_RecvShutdown(void);
void CommRandom_Init(CommRandom *seed);
int Comm_ParentQueueHasCommand(int value);
int Comm_IsParentQueueEmpty(void);
int Comm_IsChildQueueEmpty(void);
void Comm_SetWifiConnected(int value);
int Comm_IsWifiConnected(void);
void Comm_SetBattleSlot(int value, int slot);
int Comm_GetBattleSlot(int slot);
int Comm_IsWifiVoiceChatEnabled(void);
void Comm_SetWifiContinuousSend(int value);
void Comm_SetWifiContinuousSendAndMuteVoice(int value);
BOOL Comm_IsSendingMoveKeys(void);
void Comm_SetLinkError(void);
void Comm_StopFrameUpdate(void);

#endif // POKEHEARTGOLD_COMMUNICATION_CORE_INTERNAL_H
