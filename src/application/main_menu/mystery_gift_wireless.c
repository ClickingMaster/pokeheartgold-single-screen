#include "application/main_menu/mystery_gift_wireless.h"

#include "global.h"

#include "heap.h"
#include "mystery_gift.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "sys_task_api.h"
#include "unk_02033AE0.h"
#include "communication_player_state.h"
#include "unk_02034B0C.h"
#include "link_synchronization.h"

// DS-wireless link used by Mystery Gift to receive a Wonder Card from a distribution system. The link
// is driven by a small state callback run from a task every frame.

typedef struct MysteryGiftCommWork {
    void *owner;                        // 0x000
    u8 unk_004[0x18];                   // 0x004, initialised by CommRandom_Init
    SysTask *task;                      // 0x01C
    MysteryGiftCommStateFunc stateFunc; // 0x020
    u16 stateArg;                       // 0x024
    u8 paused;                          // 0x026
    u8 channel;                         // 0x027
    PlayerProfile *profiles[16];        // 0x028
    u8 recvBuffer[sizeof(WonderCard)];  // 0x068
    u8 received;                        // 0x3C0
    u8 unk_3C1[16];                     // 0x3C1
} MysteryGiftCommWork;                  // size: 0x3D4

extern MysteryGiftCommWork *sMysteryGiftCommWork;
extern const u8 sMysteryGiftCommCommands[]; // command table

extern u16 Comm_GetLocalNetId(void);
extern int Comm_IsNetIdConnected(u16 a0);
extern BOOL sub_02037474(void);
extern void CommRandom_Init(void *a0);
extern void sub_02037E1C(int a0);
extern int sub_02037EC8(void);
extern void sub_02037E9C(void);
extern int sub_02037088(int a0, void *a1, void *a2);
extern void sub_02038114(void *a0, void *a1);
extern void sub_02038148(void *a0, int a1);
extern void *sub_020358D0(int a0);
extern OverlayManager *MysteryGiftApp_GetOverlayManager(void);

// The pieces of this module that poll the work pointer read it as volatile.
#define VOLATILE_WORK (*(MysteryGiftCommWork *volatile *)&sMysteryGiftCommWork)

void MysteryGiftComm_Start(void *owner) {
    if (sMysteryGiftCommWork == NULL) {
        MysteryGiftComm_RegisterCommands(owner);
        sMysteryGiftCommWork = Heap_Alloc(HEAP_ID_15, sizeof(MysteryGiftCommWork));
        MI_CpuFill8(sMysteryGiftCommWork, 0, sizeof(MysteryGiftCommWork));
        sMysteryGiftCommWork->stateArg = 50;
        sMysteryGiftCommWork->task = SysTask_CreateOnMainQueue(MysteryGiftComm_Task, NULL, 10);
        sMysteryGiftCommWork->owner = owner;
        CommRandom_Init(sMysteryGiftCommWork->unk_004);
    }
}

void MysteryGiftComm_SetState(MysteryGiftCommStateFunc func, u16 arg) {
    sMysteryGiftCommWork->stateFunc = func;
    sMysteryGiftCommWork->stateArg = arg;
}

#pragma force_active on

void ov74_02229BD0(void) {
    int i;
    if (Comm_IsNetIdConnected(Comm_GetLocalNetId())) {
        for (i = 0; i < 16; i++) {
            VOLATILE_WORK->unk_3C1[i] = 0;
        }
        CommPlayerInfo_SendLocal();
        MysteryGiftComm_SetState(ov74_02229C0C, 0);
    }
}

void ov74_02229C0C(void) {
    int i;
    for (i = 0; i < 16; i++) {
        if (VOLATILE_WORK->profiles[i] == NULL && Comm_IsNetIdConnected((u16)i) == 1) {
            VOLATILE_WORK->profiles[i] = CommPlayerInfo_GetProfile(i);
        }
    }
}

#pragma force_active reset

void ov74_02229C44(void) {
    if (sub_02034DB8()) {
        MysteryGiftComm_SetState(ov74_02229C5C, 0);
    }
}

void ov74_02229C5C(void) {
}

void ov74_02229C60(void) {
    sub_02037E1C(sMysteryGiftCommWork->channel);
    MysteryGiftComm_SetState(ov74_02229C80, 0);
}

void ov74_02229C80(void) {
    if (sub_02037EC8()) {
        sMysteryGiftCommWork->received = FALSE;
        CommPlayerInfo_SendLocal();
        LinkSync_StartSync(0xAB);
        MysteryGiftComm_SetState(ov74_02229CB4, 0);
    }
}

void ov74_02229CB4(void) {
}

void MysteryGiftComm_Task(SysTask *task, void *data) {
    MysteryGiftCommStateFunc func;
    if (sMysteryGiftCommWork == NULL) {
        SysTask_Destroy(task);
        return;
    }
    func = sMysteryGiftCommWork->stateFunc;
    if (func != NULL && sMysteryGiftCommWork->paused == FALSE) {
        func();
    }
}

void ov74_02229CE0(void *owner, void *a1, void *a2) {
    if (!sub_02037474()) {
        sub_02038114(a1, a2);
        MysteryGiftComm_Start(owner);
        MysteryGiftComm_SetState(ov74_02229BD0, 0);
    }
}

int ov74_02229D0C(void *a0, void *a1) {
    return sub_02037088(0x16, a0, a1);
}

#pragma force_active on

void ov74_02229D1C(void *owner, int a1) {
    if (!sub_02037474()) {
        void **args = OverlayManager_GetArgs(MysteryGiftApp_GetOverlayManager());
        sub_02038148(args[2], a1);
        MysteryGiftComm_Start(owner);
        MysteryGiftComm_SetState(ov74_02229C44, 0);
    }
}

#pragma force_active reset

void ov74_02229D50(int channel) {
    sMysteryGiftCommWork->channel = channel;
    MysteryGiftComm_SetState(ov74_02229C60, 0);
}

// Copies the first partner's 0x50-byte info block (non-zero at +0x4C) to dst + 0x8C; returns its index.
int ov74_02229D6C(void *dst) {
    int i;
    for (i = 0; i < 16; i++) {
        u8 *src = sub_020358D0(i);
        if (src != NULL && *(u16 *)(src + 0x4C) != 0) {
            memcpy((u8 *)dst + 0x8C, src, 0x50);
            return i;
        }
    }
    return -1;
}

void MysteryGiftComm_HandleReceived(u8 channel) {
    if (sMysteryGiftCommWork->channel == channel) {
        sMysteryGiftCommWork->received = TRUE;
    }
}

u8 MysteryGiftComm_IsReceived(void) {
    return sMysteryGiftCommWork->received;
}

int MysteryGiftComm_GetRecvBufferSize(void) {
    return sizeof(WonderCard);
}

void *MysteryGiftComm_GetRecvBuffer(int netId, void *arg, int size) {
    return sMysteryGiftCommWork->recvBuffer;
}

#pragma force_active on

void ov74_02229DE0(int i) {
    sMysteryGiftCommWork->unk_3C1[i] = TRUE;
}

#pragma force_active reset

void MysteryGiftComm_Free(void) {
    Heap_FreeExplicit(HEAP_ID_15, sMysteryGiftCommWork);
    sMysteryGiftCommWork = NULL;
    sub_02037E9C();
}

void MysteryGiftComm_RegisterCommands(void *owner) {
    Comm_RegisterCommandTable(sMysteryGiftCommCommands, 2, owner);
}
