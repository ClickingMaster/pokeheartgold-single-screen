#ifndef POKEHEARTGOLD_COMMUNICATION_CORE_INTERNAL_H
#define POKEHEARTGOLD_COMMUNICATION_CORE_INTERNAL_H

#include "global.h"

#include "communication_packet_pool.h"
#include "sys_task.h"

// Communication core: the work area of the active link, the per-frame update, parent/child frame exchange and the
// command send queues.

// Receive state of one command stream (a command id, its size and the bytes received so far).
typedef struct PacketCursor {
    int received;
    u8 *data;
    u16 size;
    u8 command; // none
    u8 unk_0B;
} PacketCursor;

// 64-bit linear congruential generator.
typedef struct CommRandom {
    s64 state;
    s64 multiplier;
    s64 increment;
} CommRandom;

// The work area of the active link. The child frames go to the parent; the parent frames carry the commands of
// every child (transfer mode 1) or the parent's own commands (transfer mode 0) back to all children.
typedef struct CommWork {
    u8 childFrames[2][64];
    u8 parentFrames[2][192];
    u8 childSendData[264];
    u8 parentSendData[384];
    u8 *childFrameData;
    u8 *recvData;
    u8 *parentRecvData;
    u8 *commandBuffer;
    ByteRing childSendRing;
    ByteRing parentRecvRing;
    ByteRing childFrameRings[8]; // frames received from each child, copied into the next parent frame
    ByteRing parentSendRing;
    ByteRing recvRings[8]; // commands received from each net id
    SysTask *sendTask;
    PacketPool childQueue;
    PacketPool parentQueue;
    PacketCursor recvCursors[8];
    PacketCursor parentCursor;
    CommRandom random;
    u16 directionKeys[8];
    u8 moveSpeeds[8];
    u16 heldKeys; // 0x8000: key sync enabled
    u8 keysLocked;
    u8 localMoveSpeed;
    u8 keyScrambleMode;
    s8 scrambleTimer;
    u16 scrambleDirection;
    BOOL continuousSend;
    int sentFrames;
    int childSentFrames[8];
    int recvDataSize;
    int frameDataSize;
    u16 connectedNetIds;
    u8 childFrameCounts[8];
    u8 waitingFirstFrame[8];
    u8 battleSlots[4];
    u8 frameSequence;
    u8 transferModeHandshake;
    u8 requestedTransferMode;
    u8 transferMode;
    u8 packetSize;
    u8 childFrameIndex;
    u8 parentFrameIndex;
    u8 keySendTimer;
    u8 waitingParentFrame;
    u8 unk_6B3;
    u8 childQueuePending;
    u8 parentQueuePending;
    u8 wifiConnected;
    u8 unk_6B7;
    u8 linkError;
    u8 updateStopped;
    u8 receiveStopped;
    u8 sendSkipInterval;
    u8 frameCount;
} CommWork;

typedef struct CommCore {
    u8 initialized; // the parent frame for the current slot is filled in
    u8 updateLock;  // set by the frame update, cleared by the VBlank send task
    u8 unk_02[2];
    void *allocation;
    CommWork *work; // 32-byte aligned inside allocation
} CommCore;

// Send stage of the parent frame ([0]) and the child frame ([1]): 4 = idle, 0/2 = ready to send, odd = sending.
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
void Comm_ReceiveCommands(ByteRing *ring, int netId, u8 *buffer, PacketCursor *cursor);
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
void Comm_RecvTransferModeRequest(int netId, int size, void *data, void *context);
void Comm_RecvTransferModeChange(int netId, int size, void *data, void *context);
void Comm_RecvTransferModeAck(int netId, int size, void *data, void *context);
int Comm_GetLocalNetId(void);
int Comm_SendFixedCommandNoCopy(int command, void *data);
int Comm_SendFixedCommand(int command, void *data);
int Comm_SendEmptyCommand(int command);
int Comm_GetConnectedChildren(void);
int Comm_HasLinkError(void);
int Comm_GetChildFrameSize(u16 trainerId);
int Comm_GetMaxPlayers(int value);
int Comm_GetMinPlayers(int value);
void Comm_RecvShutdown(int netId, int size, void *data, void *context);
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
