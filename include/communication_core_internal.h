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
void sub_02035DD8(void);
void Comm_ResetNetIdBuffers(int slot);
void Comm_ResetDisconnectedNetIds(void);
void sub_02035ED0(int slot);
int sub_02035ED8(int a, int b, int c, int d);
int sub_02035F14(int a, int b, int c);
void sub_02035F4C(void);
void Comm_RequestTransferMode(int connected);
void sub_02035FD8(void);
void sub_02035FE4(void);
int Comm_GetTransferMode(void);
BOOL sub_02036010(void);
void sub_02036024(void);
int sub_020360E4(int value);
void Comm_VBlankSendTask(void);
void sub_0203611C(void);
BOOL Comm_Update(void);
void sub_0203622C(void);
void sub_0203624C(void);
void sub_02036274(void);
void sub_02036298(void);
BOOL Comm_FillParentFrameFromChildren(int slot);
void Comm_SendParentFrame(void);
BOOL sub_02036630(void);
void sub_0203667C(void);
void sub_020367A8(int netId, const u8 *data, int size);
void Comm_OnParentFrameReceived(int netId, const u8 *data, int size);
void sub_02036904(int netId, const u8 *data, int size);
void Comm_OnChildFrameReceived(int slot, const u8 *data, int size);
void sub_020369CC(int slot, const u8 *data);
void Comm_OnChildFrameSent(void *ptr);
void Comm_OnParentFrameSent(void *ptr);
void Comm_SendChildFrame(void);
void sub_02036BE4(void);
int sub_02036D14(const u8 *data, int slot);
int sub_02036DA0(u8 *data);
int Comm_BuildChildFrame(u8 *data);
void Comm_BuildParentFrame(u8 *data);
BOOL sub_02036FA8(void);
int sub_02036FD8(int command, void *data, int size);
int Comm_SendCommand(int command, void *data, int size);
int sub_02037088(int command, void *data, int size);
int sub_02037108(void *command, void *data, int size);
int sub_02037184(int command, void *data);
int sub_02037190(void);
void Comm_FinishCommand(int netId, int command, int size, void *data, PacketCursor *cursor);
void Comm_ReceiveCommands(CommRing *ring, int netId, u8 *buffer, PacketCursor *cursor);
void Comm_ReceiveFromParent(void);
void Comm_ReceiveFromChildren(void);
int Comm_IsNetIdConnected(int slot);
int Comm_CountConnected(void);
int sub_02037474(void);
void sub_02037498(int value);
int sub_020374AC(int slot);
u16 sub_020374C0(int slot);
void sub_020374E4(void);
void sub_02037504(void);
int sub_0203751C(void);
int sub_0203753C(int command, void *data, int size);
void sub_02037594(void);
void sub_02037618(int netId, int size, const u8 *data);
void sub_02037640(int netId, int size, const u8 *data);
void sub_02037668(int netId, int size, const u8 *data);
int Comm_GetLocalNetId(void);
int sub_020376D4(int command, void *data);
int sub_020376E0(int command, void *data);
int sub_020376EC(int command);
int sub_020376F8(void);
int sub_02037700(void);
int sub_0203772C(u16 trainerId);
int sub_0203774C(int value);
int sub_0203775C(int value);
void sub_0203776C(void);
void CommRandom_Init(CommRandom *seed);
int sub_02037824(int value);
int sub_0203783C(void);
int sub_02037854(void);
void sub_0203786C(int value);
int sub_02037880(void);
void sub_02037894(int value, int slot);
int sub_020378AC(int slot);
int sub_020378CC(void);
void sub_020378E4(int value);
void sub_02037930(int value);
BOOL sub_02037958(void);
void sub_02037974(void);
void sub_02037988(void);

#endif // POKEHEARTGOLD_COMMUNICATION_CORE_INTERNAL_H
