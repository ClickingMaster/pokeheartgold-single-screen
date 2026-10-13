#include "global.h"

#include "communication_core_internal.h"
#include "error_handling.h"
#include "gf_rtc.h"
#include "heap.h"
#include "system.h"
#include "link_synchronization.h"

// Called with the argument types their call sites were matched against.
extern void *SysTask_CreateOnVBlankQueue(void *func, void *data, int priority);
extern void SysTask_Destroy(void *task);
extern void LinkSync_Free(void);
extern int CommState_GetLinkMode(void);
extern void CommState_SetErrorWatch(int, int);
extern void CommState_CheckLinkError(int);
extern void LinkSync_Update(void);
extern void CommState_Main(void);
extern u16 Wireless_GetConnectedBitmap(void);
extern int Wireless_GetState(void);
extern BOOL Wireless_SendData(void *data, u16 size, int, void (*callback)(void *));
extern int Wireless_GetAid(void);
extern void Wireless_SetChildJoinedCallback(void (*callback)(int));
extern void ByteRing_Init(void *ring, void *buffer, int size);
extern void ByteRing_Write(void *ring, const void *data, int size, int line);
extern int ByteRing_Read(void *ring, void *dest, int size);
extern u8 ByteRing_ReadByte(void *ring);
extern int ByteRing_GetReadableSize(void *ring);
extern int ByteRing_GetFreeSize(void *ring);
extern void ByteRing_Commit(void *ring);
extern int PacketPool_AllNodesFree(void *queue);
extern int PacketPool_Enqueue(void *queue, int command, void *data, int size, int, int);
extern int PacketPool_WriteFrame(void *queue, ReceiveBuffer *receive, int);
extern void PacketPool_Init(void *queue, int count, void *ring);
extern void PacketPool_Reset(void *queue);
extern void PacketPool_Free(void *queue);
extern int PacketPool_HasCommand(void *queue, int);
extern int Comm_GetMaxChildren(u16);
extern int Comm_GetMinChildren(u16);
extern int Comm_IsWifiLinkMode(int);
extern int Comm_IsWifiPeerLinkMode(int);
extern void Comm_CallCommandRecv(int netId, int command, int size, void *data);
extern u32 Comm_GetCommandSize(int command);
extern int Comm_CommandHasBuffer(int command);
extern void *Comm_GetCommandBuffer(int command, int netId, int size);
extern void CommPlayerInfo_Free(void);
extern void CommWireless_ClearParents(void);
extern int CommWireless_StartParent(int, int, int);
extern int CommWireless_StartScan(int, int);
extern int CommWireless_RequestStopAndFree(void);
extern int CommWireless_ConnectToParent(int);
extern BOOL CommWireless_Main(int);
extern int CommWireless_Exists(void);
extern int CommWireless_GetConnectedChildren(void);
extern int CommWireless_IsLinkLost(void);
extern int CommWireless_IsShutdownRequested(void);
extern void CommWireless_RequestShutdown(void);
extern BOOL WifiConnection_SendToHost(void *frame, int size);
extern BOOL WifiConnection_SendFromHost(void *frame, int size);
extern BOOL WifiConnection_SendToAll(void *frame, int size);
extern int WifiConnection_GetAid(void);
extern void WifiConnection_Shutdown(void);
extern int ov00_021E7080(void);
extern void ov00_021E70CC(int);
extern u32 ov00_021EE4FC(void);

// The bytes around the key state in the work area (Comm_ScrambleDirectionKeys).
typedef struct CommKeyWork {
    u8 filler_000[0x62C];
    CommRandom random; // 0x62C
    u8 filler_644[0x18];
    u16 buttons; // 0x65C
    u16 unk_65E;
    u8 mode;       // 0x660
    s8 timer;      // 0x661
    u16 direction; // 0x662
} CommKeyWork;

CommCore gCommCore;
CommSendStages gCommSendStages = {
    { 4, 4 }
};

int Comm_InitWork(int create, int packetSize) {
    int existing = 0;
    int count;
    int i;

    gCommCore.updateLock = 0;
    if (create != 0) {
        count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
        if (gCommCore.work != NULL) {
            return 1;
        }

        LinkSync_Init(HEAP_ID_15);
        gCommCore.allocation = Heap_Alloc(HEAP_ID_15, 0x6e0);
        gCommCore.work = (u8 *)(32 - ((u32)gCommCore.allocation & 31)) + (u32)gCommCore.allocation;
        MI_CpuFill8(gCommCore.work, 0, 0x6c0);

        if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
            *(u32 *)(gCommCore.work + 0x690) = packetSize * 2 + 0x40;
        } else {
            *(u32 *)(gCommCore.work + 0x690) = packetSize + 0x40;
        }
        *(u32 *)(gCommCore.work + 0x68c) = *(u32 *)(gCommCore.work + 0x690) * count;
        gCommCore.work[0x6ad] = 0;
        packetSize = 42;
        gCommCore.work[0x6ae] = packetSize;
        *(void **)(gCommCore.work + 0x490) = Heap_Alloc(HEAP_ID_15, *(u32 *)(gCommCore.work + 0x690) * 2);
        *(void **)(gCommCore.work + 0x494) = Heap_Alloc(HEAP_ID_15, *(u32 *)(gCommCore.work + 0x690));
        *(void **)(gCommCore.work + 0x48c) = Heap_Alloc(HEAP_ID_15, *(u32 *)(gCommCore.work + 0x68c));
        *(void **)(gCommCore.work + 0x488) = Heap_Alloc(HEAP_ID_15, *(u32 *)(gCommCore.work + 0x68c));

        if (CommState_GetLinkMode() == 10) {
            PacketPool_Init(gCommCore.work + 0x580, 100, gCommCore.work + 0x498);
            PacketPool_Init(gCommCore.work + 0x5a0, 800, gCommCore.work + 0x510);
        } else {
            PacketPool_Init(gCommCore.work + 0x580, 20, gCommCore.work + 0x498);
            PacketPool_Init(gCommCore.work + 0x5a0, packetSize + 238, gCommCore.work + 0x510);
        }
    } else {
        existing = 1;
        if (gCommCore.work == NULL) {
            GF_AssertFail();
        }
    }

    *(u16 *)(gCommCore.work + 0x694) = 0;
    for (i = 0; i < 4; i++) {
        gCommCore.work[0x6a6 + i] = 0xff;
    }
    if (existing == 0) {
        Comm_Reset();
    }
    CommRandom_Init((CommRandom *)(gCommCore.work + 0x62c));
    if (existing == 0) {
        *(void **)(gCommCore.work + 0x57c) = SysTask_CreateOnVBlankQueue(Comm_VBlankSendTask, NULL, 0);
    }
    gCommCore.work[0x6b6] = 0;
    return 1;
}

#pragma force_active on

static inline void ResetPacketCursors(void) {
    int i = 0;
    int offset;
    int half = 0;

    gCommCore.work[0x6b4] = i;
    gCommCore.work[0x6b5] = i;
    offset = 0;
    for (; i < 8; i++, offset += 12, half += 2) {
        gCommCore.work[0x696 + i] = 0;
        gCommCore.work[0x69e + i] = 1;
        *(u16 *)(gCommCore.work + 0x644 + half) = 0;
        ((PacketCursor *)(gCommCore.work + 0x5c0 + offset))->command = 0xee;
        ((PacketCursor *)(gCommCore.work + 0x5c0 + offset))->size = 0xffff;
        ((PacketCursor *)(gCommCore.work + 0x5c0 + offset))->data = NULL;
        ((PacketCursor *)(gCommCore.work + 0x5c0 + offset))->received = 0;
        *(u32 *)(gCommCore.work + 0x66c + i * 4) = 0;
    }
}

void Comm_ResetBuffers(void) {
    int count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
    int length;
    int i;

    gCommCore.work[0x660] = 0;
    gCommCore.work[0x661] = 0;
    MI_CpuFill8(*(void **)(gCommCore.work + 0x48c), 0, *(int *)(gCommCore.work + 0x68c));
    MI_CpuFill8(gCommCore.work + 0x51c, 0, 0x60);
    length = *(int *)(gCommCore.work + 0x68c) / count;
    for (i = 0; i < count; i++) {
        ByteRing_Init(gCommCore.work + 0x51c + i * 12, (u8 *)*(void **)(gCommCore.work + 0x48c) + i * length, length);
    }
    MI_CpuFill8(*(void **)(gCommCore.work + 0x488), 0, *(int *)(gCommCore.work + 0x68c));
    MI_CpuFill8(gCommCore.work + 0x4b0, 0, 0x60);
    for (i = 0; i < count; i++) {
        ByteRing_Init(gCommCore.work + 0x4b0 + i * 12, (u8 *)*(void **)(gCommCore.work + 0x488) + i * length, length);
    }
    MI_CpuFill8(gCommCore.work + 0x308, 0, 0x180);
    ByteRing_Init(gCommCore.work + 0x510, gCommCore.work + 0x308, 0x180);
    MI_CpuFill8(gCommCore.work + 0x80, 0xee, 0x180);
    MI_CpuFill8(gCommCore.work + 0x140, 0xee, 0x180);
    MI_CpuFill8(gCommCore.work + 0x200, 0, 0x108);
    ByteRing_Init(gCommCore.work + 0x498, gCommCore.work + 0x200, 0x108);
    MI_CpuFill8(gCommCore.work, 0xee, 0x26);
    MI_CpuFill8(gCommCore.work + 0x40, 0xee, 0x26);
    gCommCore.work[0] = 0xff;
    gCommCore.work[0x40] = 0xff;
    MI_CpuFill8(*(void **)(gCommCore.work + 0x490), 0, *(int *)(gCommCore.work + 0x690) * 2);
    ByteRing_Init(gCommCore.work + 0x4a4, *(void **)(gCommCore.work + 0x490), *(int *)(gCommCore.work + 0x690) * 2);
    ResetPacketCursors();
    *(int *)(gCommCore.work + 0x668) = 0;
    gCommCore.work[0x62a] = 0xee;
    *(u16 *)(gCommCore.work + 0x628) = 0xffff;
    *(int *)(gCommCore.work + 0x624) = 0;
    *(int *)(gCommCore.work + 0x620) = 0;
    gCommCore.work[0x6b2] = 1;
    gCommCore.work[0x6b3] = 1;
    gCommCore.initialized = 0;
    PacketPool_Reset(gCommCore.work + 0x580);
    PacketPool_Reset(gCommCore.work + 0x5a0);
    gCommCore.work[0x6b7] = 0;
}

#pragma force_active off

void Comm_Reset(void) {
    gCommCore.work[0x6AF] = 0;
    gCommCore.work[0x6B0] = 0;
    *(u32 *)(gCommCore.work + 0x664) = 1;
    Comm_ResetBuffers();
    gCommSendStages.stage[0] = 4;
    gCommSendStages.stage[1] = 4;
}

void sub_02035DD8(void) {
    Comm_ResetBuffers();
}

void Comm_ResetNetIdBuffers(int slot) {
    int count;
    u8 *work;
    int chunkSize;
    int offset = slot;
    int ringOffset = slot;

    (gCommCore.work + slot)[0x696] = 0;
    (gCommCore.work + slot)[0x69E] = 1;
    *(int *)(gCommCore.work + slot * 4 + 0x66C) = 0;
    count = Comm_GetMaxChildren((u16)CommState_GetLinkMode());
    work = gCommCore.work;
    work = (u8 *)work; // needed to match
    chunkSize = *(int *)(work + 0x68C) / (count + 1);
    offset *= chunkSize;
    ringOffset *= 12;
    ByteRing_Init(work + 0x4B0 + ringOffset, *(u8 **)(work + 0x488) + offset, chunkSize);
    work = gCommCore.work;
    ByteRing_Init(work + 0x51C + ringOffset, *(u8 **)(work + 0x48C) + offset, chunkSize);
    (gCommCore.work + ringOffset)[0x5CA] = 0xEE;
    *(u16 *)(gCommCore.work + ringOffset + 0x5C8) = 0xFFFF;
    *(int *)(gCommCore.work + ringOffset + 0x5C4) = 0;
    *(int *)(gCommCore.work + ringOffset + 0x5C0) = 0;
}

void Comm_ResetDisconnectedNetIds(void) {
    int i;
    for (i = 1; i < 8; i++) {
        if (!Comm_IsNetIdConnected((u16)i) && gCommCore.work[0x69E + i] == 0) {
            Comm_ResetNetIdBuffers(i);
        }
    }
}

void Comm_OnChildJoined(int slot) {
    Comm_ResetNetIdBuffers(slot);
}

int Comm_StartParent(int a, int b, int c, int d) {
    int result = 1;
    if (!Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        result = CommWireless_StartParent(a, b, d);
        Wireless_SetChildJoinedCallback(Comm_OnChildJoined);
    }
    Comm_InitWork(a, c);
    return result;
}

int Comm_StartScan(int a, int b, int c) {
    int result = 1;
    if (!Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        result = CommWireless_StartScan(a, b);
    }
    Comm_InitWork(a, c);
    gCommSendStages.stage[1] = 4;
    return result;
}

void Comm_UpdateTransferMode(void) {
    int changed = 0;
    if (Comm_GetLocalNetId() == 0) {
        if (gCommSendStages.stage[0] != 4) {
            return;
        }
    } else if (gCommSendStages.stage[1] != 4) {
        return;
    }
    if (gCommCore.work[0x6AD] == 2) {
        gCommCore.work[0x6AD] = 0;
        changed = 1;
    }
    if (gCommCore.work[0x6AD] == 3) {
        changed = 1;
        gCommCore.work[0x6AD] = 1;
    }
    if (changed) {
        sub_02035DD8();
    }
    Comm_UpdateTransferModeHandshake();
}

void Comm_RequestTransferMode(int connected) {
    int state = gCommCore.work[0x6AD];
    if (state == 0 && connected == 1) {
        gCommCore.work[0x6AD] = 3;
        return;
    }
    if (state == 1 && connected == 0) {
        gCommCore.work[0x6AD] = 2;
    }
}

void sub_02035FD8(void) {
    Comm_RequestTransferMode(1);
}

void sub_02035FE4(void) {
    Comm_RequestTransferMode(0);
}

int Comm_GetTransferMode(void) {
    int state = gCommCore.work[0x6AD];
    if (state == 2) {
        return 1;
    }
    if (state == 3) {
        return 0;
    }
    return state;
}

BOOL sub_02036010(void) {
    if (Comm_GetTransferMode() == 1) {
        return TRUE;
    }
    return FALSE;
}

void Comm_Shutdown(void) {
    int destroy = 0;
    if (gCommCore.work != NULL) {
        if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
            WifiConnection_Shutdown();
            destroy = 1;
        } else if (CommWireless_RequestStopAndFree()) {
            destroy = 1;
        }
    }
    if (destroy) {
        LinkSync_Free();
        CommPlayerInfo_Free();
        gCommCore.updateLock = 0;
        SysTask_Destroy(*(void **)(gCommCore.work + 0x57C));
        *(void **)(gCommCore.work + 0x57C) = NULL;
        Heap_Free(*(void **)(gCommCore.work + 0x490));
        Heap_Free(*(void **)(gCommCore.work + 0x494));
        Heap_Free(*(void **)(gCommCore.work + 0x48C));
        Heap_Free(*(void **)(gCommCore.work + 0x488));
        PacketPool_Free(gCommCore.work + 0x5A0);
        PacketPool_Free(gCommCore.work + 0x580);
        Heap_Free(gCommCore.allocation);
        gCommCore.work = NULL;
        gCommCore.allocation = NULL;
    }
}

int Comm_ConnectToParent(int value) {
    return CommWireless_ConnectToParent(value);
}

void Comm_VBlankSendTask(void) {
    if (gCommCore.updateLock != 0) {
        Comm_SendChildFrame();
        if (Comm_GetLocalNetId() == 0 && Comm_IsNetIdConnected(0)) {
            Comm_SendParentFrame();
        }
        gCommCore.updateLock = 0;
    }
}

void Comm_ShutdownWhenRequested(void) {
    if (CommWireless_IsShutdownRequested()) {
        if (Comm_GetLocalNetId() == 0) {
            if (CommWireless_GetConnectedChildren() == 0) {
                Comm_Shutdown();
            }
        } else {
            Comm_Shutdown();
        }
    }
}

BOOL Comm_Update(void) {
    u8 *work;

    CommState_Main();
    work = gCommCore.work;
    if (work != NULL) {
        if (work[0x6B9] == 0) {
            work[0x6BC]++;
            gCommCore.updateLock = 0;
            Comm_UpdateTransferMode();
            work = gCommCore.work;
            *(u16 *)(work + 0x65C) |= gSystem.heldKeys & 0x7FFF;
            Comm_ScrambleDirectionKeys();
            Comm_UpdateChildSend();
            work = gCommCore.work;
            *(u16 *)(work + 0x65C) &= 0x8000;
            if (Comm_GetTransferMode() == 0) {
                Comm_ReceiveFromParent();
            }
            if (Comm_GetLocalNetId() == 0 && Comm_IsNetIdConnected(0) != 0) {
                if (Comm_IsWifiPeerLinkMode(CommState_GetLinkMode()) == 0) {
                    Comm_UpdateParentSend();
                }
            }
            if (Comm_GetLocalNetId() == 0 || Comm_GetTransferMode() == 1) {
                Comm_ReceiveFromChildren();
            }
            gCommCore.updateLock = 1;
        }
        CommWireless_Main(*(u16 *)(gCommCore.work + 0x694));
        if (Comm_GetLocalNetId() == 0) {
            Comm_ResetDisconnectedNetIds();
        }
        Comm_ShutdownWhenRequested();
    } else {
        CommWireless_Main(0);
    }
    CommState_CheckLinkError(0);
    LinkSync_Update();
    return TRUE;
}

void Comm_ResetWork(void) {
    u8 lock = gCommCore.updateLock;
    gCommCore.updateLock = 0;
    if (gCommCore.work != NULL) {
        Comm_Reset();
    }
    gCommCore.updateLock = lock;
}

void sub_0203624C(void) {
    u8 lock = gCommCore.updateLock;
    gCommCore.updateLock = 0;
    if (gCommCore.work != NULL) {
        gCommCore.work[0x6AD] = 1;
        Comm_Reset();
    }
    gCommCore.updateLock = lock;
}

void Comm_ResetWorkAndParents(void) {
    u8 lock = gCommCore.updateLock;
    gCommCore.updateLock = 0;
    if (gCommCore.work != NULL) {
        Comm_Reset();
        CommWireless_ClearParents();
    }
    gCommCore.updateLock = lock;
}

#pragma force_active on

void Comm_UpdateChildSend(void) {
    int i;
    int count;

    if (Comm_IsWifiPeerLinkMode(CommState_GetLinkMode())) {
        if (gCommCore.work[0x6B6]) {
            if (*(int *)(gCommCore.work + 0x664)) {
                if (!Comm_HasNoChildBacklog()) {
                    return;
                }
                if (gCommSendStages.stage[1] == 4) {
                    Comm_BuildChildFrame(gCommCore.work);
                    gCommSendStages.stage[1] = 2;
                }
            } else if (gCommSendStages.stage[1] == 4) {
                if (!Comm_BuildChildFrame(gCommCore.work)) {
                    return;
                }
                gCommSendStages.stage[1] = 2;
            }
            if (Comm_IsSendSkippedThisFrame()) {
                return;
            }
            if (WifiConnection_SendToAll(gCommCore.work, 38)) {
                count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
                for (i = 0; i < count; i++) {
                    if (Comm_IsNetIdConnected((u16)i)) {
                        (*(int *)(gCommCore.work + i * 4 + 0x66C))++;
                    }
                }
                gCommSendStages.stage[1] = 4;
            }
        }
    } else if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        if (!gCommCore.work[0x6B6]) {
            return;
        }
        if (*(int *)(gCommCore.work + 0x664)) {
            if (*(int *)(gCommCore.work + 0x668) > 3) {
                return;
            }
            if (gCommSendStages.stage[1] == 4) {
                Comm_BuildChildFrame(gCommCore.work);
                gCommSendStages.stage[1] = 2;
            }
        } else if (gCommSendStages.stage[1] == 4) {
            if (!Comm_BuildChildFrame(gCommCore.work)) {
                return;
            }
            gCommSendStages.stage[1] = 2;
        }
        if (Comm_IsSendSkippedThisFrame()) {
            return;
        }
        if (!WifiConnection_SendToHost(gCommCore.work, 38)) {
            return;
        }
        gCommSendStages.stage[1] = 4;
        (*(int *)(gCommCore.work + 0x668))++;
    } else if (Wireless_GetState() == 4) {
        if (!Comm_IsNetIdConnected(Comm_GetLocalNetId())) {
            return;
        }
        if (gCommSendStages.stage[1] == 4 && *(int *)(gCommCore.work + 0x668) <= 3) {
            Comm_BuildChildFrame(gCommCore.work + gCommCore.work[0x6AF] * 64);
            Comm_BuildChildFrame(gCommCore.work + (1 - gCommCore.work[0x6AF]) * 64);
            gCommSendStages.stage[1] = 0;
        }
        Comm_SendChildFrame();
    }
}

#pragma force_active off

BOOL Comm_FillParentFrameFromChildren(int slot) {
    int bad;
    int stride;
    int count;
    int i;
    int entryOffset;
    int rowOffset;
    int slotOffset;
    bad = 0;
    stride = Comm_GetChildFrameSize((u16)CommState_GetLinkMode());
    count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
    i = bad;
    if (count > 0) {
        entryOffset = i;
        rowOffset = entryOffset;
        slotOffset = slot * 0xC0;
        do {
            u8 *ctx = gCommCore.work;
            ByteRing_Commit(ctx + 0x4B0 + entryOffset);
            if (Comm_IsNetIdConnected((u16)i)) {
                ctx = gCommCore.work;
                ctx[0x80 + slotOffset + rowOffset] = 14;
            } else {
                ctx = gCommCore.work;
                bad++;
                ctx[0x80 + slotOffset + rowOffset] = 0xFF;
                goto update;
            }
            ctx = gCommCore.work;
            ByteRing_Read(ctx + 0x4B0 + entryOffset, ctx + 0x80 + slotOffset + rowOffset, stride);
            ctx = gCommCore.work;
            if (ctx[0x80 + slotOffset + rowOffset] == 14) {
                bad++;
            }
update:
            entryOffset += 12;
            rowOffset += stride;
            i++;
        } while (i < count);
    }
    if (bad == count) {
        return FALSE;
    }
    return TRUE;
}

#pragma force_active on

void Comm_SendParentFrame(void) {
    int count;
    int i;

    if (gCommCore.work == NULL) {
        return;
    }
    if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        return;
    }
    Comm_GetChildFrameSize(CommState_GetLinkMode());
    count = Comm_GetMaxChildren(CommState_GetLinkMode()) + 1;
    if (gCommSendStages.stage[0] != 2 && gCommSendStages.stage[0] != 0) {
        return;
    }
    gCommSendStages.stage[0]++;
    if (Comm_GetTransferMode() == 1) {
        if (!gCommCore.initialized) {
            Comm_FillParentFrameFromChildren(gCommCore.work[0x6B0]);
            gCommCore.initialized = 1;
        }
    }
    if (Wireless_GetState() == 4) {
        if (!Wireless_SendData(gCommCore.work + 0x80 + gCommCore.work[0x6B0] * 0xC0, 0xC0, 14, Comm_OnParentFrameSent)) {
            gCommSendStages.stage[0]--;
        }
    }
    if (gCommSendStages.stage[0] == 1 || gCommSendStages.stage[0] == 3) {
        gCommCore.initialized = 0;
        for (i = 0; i < count; i++) {
            if (Comm_IsNetIdConnected((u16)i)) {
                (*(int *)(gCommCore.work + i * 4 + 0x66C))++;
            }
        }
        Comm_OnParentFrameReceived(0, gCommCore.work + 0x80 + gCommCore.work[0x6B0] * 0xC0, 0xC0);
        gCommCore.work[0x6B0] = 1 - gCommCore.work[0x6B0];
    }
    if (Wireless_GetState() != 4) {
        gCommSendStages.stage[0]++;
    }
}

#pragma force_active off

BOOL Comm_HasNoChildBacklog(void) {
    int count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
    int i;
    for (i = 1; i < count; i++) {
        if (Comm_IsNetIdConnected((u16)i) && *(int *)(gCommCore.work + 0x66C + i * 4) > 3) {
            return FALSE;
        }
    }
    return TRUE;
}

void Comm_UpdateParentSend(void) {
    int count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
    int i;

    if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        if (!Comm_IsNetIdConnected(0)) {
            return;
        }
        if (*(int *)(gCommCore.work + 0x664) != 0) {
            if (!Comm_HasNoChildBacklog()) {
                return;
            }
            if (gCommSendStages.stage[0] == 4) {
                if (Comm_GetTransferMode() == 1) {
                    Comm_FillParentFrameFromChildren(0);
                }
                gCommSendStages.stage[0] = 2;
            }
        } else {
            if (gCommSendStages.stage[0] == 4) {
                if (Comm_GetTransferMode() == 1) {
                    if (!Comm_FillParentFrameFromChildren(0)) {
                        return;
                    }
                }
            }
            gCommSendStages.stage[0] = 2;
        }
        if (!WifiConnection_SendFromHost(gCommCore.work + 0x80, 0xC0)) {
            return;
        }
        gCommSendStages.stage[0] = 4;
        for (i = 0; i < count; i++) {
            if (Comm_IsNetIdConnected((u16)i)) {
                (*(int *)(gCommCore.work + i * 4 + 0x66C))++;
            }
        }
    } else {
        if (Wireless_GetState() != 4 || gCommSendStages.stage[0] != 4 || !Comm_HasNoChildBacklog()) {
            return;
        }
        if (Comm_GetTransferMode() == 0) {
            Comm_BuildParentFrame(gCommCore.work + 0x80 + gCommCore.work[0x6B0] * 0xC0);
            Comm_BuildParentFrame(gCommCore.work + 0x80 + (1 - gCommCore.work[0x6B0]) * 0xC0);
        }
        gCommSendStages.stage[0] = 0;
        Comm_SendParentFrame();
    }
}

void Comm_RecvParentFrameCallback(int netId, const u8 *data, int size) {
    Comm_OnParentFrameReceived(netId, data, size);
}

void Comm_OnParentFrameReceived(int netId, const u8 *data, int size) {
    u8 *work;
    int stride;
    int count;
    int i;
    int offset;

    work = gCommCore.work;
    (*(int *)(work + 0x668))--;
    if (data == NULL) {
        return;
    }
    if (data[0] == 0xB) {
        if (Comm_GetTransferMode() == 1) {
            return;
        }
        data++;
    } else if (Comm_GetTransferMode() == 0) {
        return;
    }
    work = gCommCore.work;
    if (work[0x6B2] != 0 && (data[0] & 1)) {
        return;
    }
    work[0x6B2] = 0;
    if (Comm_GetTransferMode() == 1) {
        stride = Comm_GetChildFrameSize((u16)CommState_GetLinkMode());
        count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
        i = 0;
        if (count <= 0) {
            return;
        }
        offset = i;
        do {
            if (data[0] == 0xFF) {
                work = gCommCore.work;
                *(u16 *)(work + 0x694) &= ~(1 << i);
            } else {
                work = gCommCore.work;
                *(u16 *)(work + 0x694) |= 1 << i;
            }
            if (data[0] == 0xFF) {
                data += stride;
                goto next;
            }
            if (data[0] == 0xE) {
                data += stride;
                goto next;
            }
            work = gCommCore.work;
            if (work[0x69E + i] != 0 && (data[0] & 1)) {
                data += stride;
                goto next;
            }
            data++;
            ByteRing_Write(work + 0x51C + offset, data, stride - 1, i + 0x54C);
            data += stride - 1;
            work = gCommCore.work;
            work[0x69E + i] = 0;
next:
            i++;
            offset += 12;
        } while (i < count);
    } else {
        *(u16 *)(gCommCore.work + 0x694) = data[1];
        *(u16 *)(gCommCore.work + 0x694) <<= 8;
        *(u16 *)(gCommCore.work + 0x694) += data[2];
        ByteRing_Write(gCommCore.work + 0x4A4, data + 4, data[3], 0x560);
    }
}

void Comm_RecvChildFrameCallback(int netId, const u8 *data, int size) {
    Comm_OnChildFrameReceived(netId, data, size);
}

void Comm_OnChildFrameReceived(int slot, const u8 *data, int size) {
    int remaining;
    u8 *received;

    remaining = --*(int *)(gCommCore.work + 0x66C + slot * 4);
    if (data == NULL) {
        return;
    }
    received = gCommCore.work + 0x69E;
    if (received[slot] != 0 && (data[0] & 1) != 0) {
        return;
    }
    received[slot] = 0;
    if (Comm_GetTransferMode() == 1) {
        remaining = Comm_GetChildFrameSize((u16)CommState_GetLinkMode());
        Comm_GetMaxChildren((u16)CommState_GetLinkMode());
        if ((data[0] & 2) == 0) {
            ByteRing_Write(gCommCore.work + 0x4B0 + slot * 12, data, remaining, 0x5A5);
        }
        received = gCommCore.work + 0x696;
        received[slot]++;
    } else {
        Comm_ReadFrameKeys(data, slot);
        if ((data[0] & 2) == 0) {
            ByteRing_Write(gCommCore.work + 0x51C + slot * 12, data + 1, 11, 0x5AE);
        }
    }
}

void Comm_RecvPeerFrameCallback(int slot, const u8 *data) {
    int remaining;

    remaining = --*(int *)(gCommCore.work + 0x66C + slot * 4);
    if (data == NULL) {
        return;
    }
    {
        u8 *received = gCommCore.work + 0x69E;
        if (received[slot] != 0 && (data[0] & 1) != 0) {
            return;
        }
        received[slot] = 0;
    }
    if (Comm_GetTransferMode() != 1) {
        return;
    }
    remaining = Comm_GetChildFrameSize((u16)CommState_GetLinkMode());
    Comm_GetMaxChildren((u16)CommState_GetLinkMode());
    if (data[0] == 0xFF) {
        *(u16 *)(gCommCore.work + 0x694) &= ~(1 << slot);
    } else {
        *(u16 *)(gCommCore.work + 0x694) |= 1 << slot;
    }
    if (data[0] == 0xFF || data[0] == 2 || data[0] == 0xE) {
        return;
    }
    {
        u8 *received = gCommCore.work + 0x69E;
        if (received[slot] != 0 && (data[0] & 1) != 0) {
            return;
        }
    }
    ByteRing_Write(gCommCore.work + 0x51C + slot * 12, data + 1, remaining - 1, 0x5E7);
    {
        u8 *received = gCommCore.work + 0x69E;
        received[slot] = 0;
    }
}

void Comm_OnChildFrameSent(void *ptr) {
    if (ptr != NULL) {
        gCommSendStages.stage[1]++;
    } else {
        GF_AssertFail();
    }
}

void Comm_OnParentFrameSent(void *ptr) {
    if (ptr != NULL) {
        gCommSendStages.stage[0]++;
    } else {
        GF_AssertFail();
    }
}

void Comm_SendChildFrame(void) {
    int frameSize;
    u8 *work;

    if (gCommCore.work == NULL) {
        return;
    }
    if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        return;
    }
    frameSize = Comm_GetChildFrameSize((u16)CommState_GetLinkMode());
    Comm_GetMaxChildren((u16)CommState_GetLinkMode());
    if (Wireless_GetState() != 4) {
        return;
    }
    if (!Comm_IsNetIdConnected(Comm_GetLocalNetId())) {
        Comm_GetLocalNetId();
        return;
    }
    if (gCommSendStages.stage[1] == 2 || gCommSendStages.stage[1] == 0) {
        if (Comm_GetLocalNetId()) {
            gCommSendStages.stage[1]++;
            work = gCommCore.work;
            if (!Wireless_SendData(work + work[0x6AF] * 64, (u16)frameSize, 14, Comm_OnChildFrameSent)) {
                gCommSendStages.stage[1]--;
                return;
            }
            gCommCore.work[0x6AF] = 1 - gCommCore.work[0x6AF];
            (*(u32 *)(gCommCore.work + 0x668))++;
        } else {
            if ((Wireless_GetConnectedBitmap() & 0xFFFE) == 0) {
                return;
            }
            gCommSendStages.stage[1]++;
            Comm_OnChildFrameSent((void *)1);
            work = gCommCore.work;
            Comm_OnChildFrameReceived(0, work + work[0x6AF] * 64, (u16)frameSize);
            gCommCore.work[0x6AF] = 1 - gCommCore.work[0x6AF];
            (*(u32 *)(gCommCore.work + 0x668))++;
        }
    }
}

#pragma force_active on

static inline u32 CommKeyWork_Random(CommKeyWork *w, u32 max) {
    CommRandom *rng = &w->random;
    rng->state = rng->multiplier * rng->state + rng->increment;
    return (u32)((((u64)rng->state >> 32) * max) >> 32);
}

void Comm_ScrambleDirectionKeys(void) {
    CommKeyWork *w = (CommKeyWork *)gCommCore.work;
    u16 result = 0;
    if (w->mode == 0 || !(w->buttons & 0xf0)) {
        return;
    }
    if (w->mode == 2) {
        if (w->buttons & 0x20) {
            result |= 0x10;
        }
        if (w->buttons & 0x10) {
            result |= 0x20;
        }
        if (w->buttons & 0x40) {
            result |= 0x80;
        }
        if (w->buttons & 0x80) {
            result |= 0x40;
        }
    } else if (w->direction != 0) {
        result = w->direction;
        w->timer--;
        if (((CommKeyWork *)gCommCore.work)->timer < 0) {
            ((CommKeyWork *)gCommCore.work)->direction = 0;
        }
    } else {
        switch (CommKeyWork_Random(w, 4)) {
        case 0:
            result = 0x20;
            break;
        case 1:
            result = 0x10;
            break;
        case 2:
            result = 0x40;
            break;
        case 3:
            result = 0x80;
            break;
        }
        w = (CommKeyWork *)gCommCore.work;
        w->timer = CommKeyWork_Random(w, 16);
        ((CommKeyWork *)gCommCore.work)->direction = result;
    }
    ((CommKeyWork *)gCommCore.work)->buttons &= ~0xf0;
    ((CommKeyWork *)gCommCore.work)->buttons += result;
}

#pragma force_active off

int Comm_ReadFrameKeys(const u8 *data, int slot) {
    u8 value;
    u8 direction;
    int decoded;

    ((u16 *)(gCommCore.work + 0x644))[slot] = 0;
    value = *data;
    if ((value & 0x10) == 0x10) {
        direction = value & 0x0c;
        if (direction == 0) {
            ((u16 *)(gCommCore.work + 0x644))[slot] |= 0x40;
        } else if (direction == 4) {
            ((u16 *)(gCommCore.work + 0x644))[slot] |= 0x80;
        } else if (direction == 8) {
            ((u16 *)(gCommCore.work + 0x644))[slot] |= 0x20;
        } else if (direction == 0x0c) {
            ((u16 *)(gCommCore.work + 0x644))[slot] |= 0x10;
        }
        decoded = *data >> 5;
        decoded &= 7;
        {
            u8 *work = gCommCore.work + slot;
            work[0x654] = decoded;
        }
    }
    return 1;
}

int Comm_WriteFrameKeys(u8 *data) {
    u16 flags;
    if (gCommCore.work[0x65E] != 0) {
        return 0;
    }
    if (!Comm_IsKeySyncEnabled()) {
        return 0;
    }
    if (gCommCore.work[0x6B1] != 0) {
        gCommCore.work[0x6B1]--;
    }
    flags = *(u16 *)(gCommCore.work + 0x65C);
    if (flags & 0x40) {
        *data |= 0x10;
        gCommCore.work[0x6B1] = 8;
    } else if (flags & 0x80) {
        *data |= 0x14;
        gCommCore.work[0x6B1] = 8;
    } else if (flags & 0x20) {
        *data |= 0x18;
        gCommCore.work[0x6B1] = 8;
    } else if (flags & 0x10) {
        *data |= 0x1C;
        gCommCore.work[0x6B1] = 8;
    }
    *data |= gCommCore.work[0x65F] << 5;
    return 0;
}

int Comm_BuildChildFrame(u8 *data) {
    ReceiveBuffer receive;
    int count = Comm_GetChildFrameSize((u16)CommState_GetLinkMode());

    Comm_GetMaxChildren((u16)CommState_GetLinkMode());
    if (gCommCore.work[0x6B4] == 0) {
        *data = 0;
    } else {
        *data = 1;
    }
    if (Comm_GetTransferMode() == 0) {
        Comm_WriteFrameKeys(data);
    }
    gCommCore.work[0x6B4] = 0;
    if (PacketPool_AllNodesFree(gCommCore.work + 0x580)) {
        *data |= 2;
        if (*data == 2) {
            return 0;
        }
    } else {
        receive.remaining = count - 1;
        receive.dest = data + 1;
        if (PacketPool_WriteFrame(gCommCore.work + 0x580, &receive, 1) == 0) {
            gCommCore.work[0x6B4] = 1;
        }
        if (Comm_GetTransferMode() == 1) {
            gCommCore.work[0x6AA]++;
            *data |= (gCommCore.work[0x6AA] << 4) & 0xF0;
        }
    }
    return 1;
}

void Comm_BuildParentFrame(u8 *data) {
    ReceiveBuffer receive;
    u16 value;

    data[0] = 0xB;
    if (gCommCore.work[0x6B5] == 0) {
        data[1] = 0;
    } else {
        data[1] = 1;
    }
    value = Wireless_GetConnectedBitmap();
    data[2] = value >> 8;
    data[3] = value;
    receive.remaining = 0xBB;
    receive.dest = data + 5;
    if (PacketPool_WriteFrame(gCommCore.work + 0x5A0, &receive, 0)) {
        gCommCore.work[0x6B5] = 0;
        data[4] = 0xBB - receive.remaining;
    } else {
        gCommCore.work[0x6B5] = 1;
        data[4] = 0xBB;
    }
}

BOOL Comm_IsSendSkippedThisFrame(void) {
    int divisor = gCommCore.work[0x6BB];
    if (divisor == 0) {
        return FALSE;
    }
    if (gCommCore.work[0x6BC] % divisor == 0) {
        return TRUE;
    }
    return FALSE;
}

int Comm_SendCommandNoCopy(int command, void *data, int size) {
    if (!Comm_IsNetIdConnected(Comm_GetLocalNetId())) {
        return 0;
    }
    if (PacketPool_Enqueue(gCommCore.work + 0x580, command, data, size, 1, 0)) {
        return 1;
    }
    if (CommState_GetLinkMode() == 10) {
        Comm_SetLinkError();
    }
    return 0;
}

int Comm_SendCommand(int command, void *data, int size) {
    if (!Comm_IsNetIdConnected(Comm_GetLocalNetId())) {
        return 0;
    }
    if (PacketPool_Enqueue(gCommCore.work + 0x580, command, data, size, 1, 1)) {
        return 1;
    }
    if (CommState_GetLinkMode() == 10) {
        Comm_SetLinkError();
    }
    return 0;
}

int Comm_SendParentCommandNoCopy(int command, void *data, int size) {
    if (Comm_GetLocalNetId() != 0) {
        GF_AssertFail();
        return 0;
    }
    if (!Comm_IsNetIdConnected(0)) {
        return 0;
    }
    if (Comm_GetTransferMode() == 1) {
        return Comm_SendCommandNoCopy(command, data, size);
    }
    if (PacketPool_Enqueue(gCommCore.work + 0x5A0, command, data, size, 1, 0)) {
        return 1;
    }
    if (CommState_GetLinkMode() == 10) {
        Comm_SetLinkError();
    }
    return 0;
}

int Comm_SendParentCommand(void *command, void *data, int size) {
    if (Comm_GetLocalNetId() != 0) {
        Comm_SetLinkError();
        return 0;
    }
    if (!Comm_IsNetIdConnected(0)) {
        return 0;
    }
    if (Comm_GetTransferMode() == 1) {
        return Comm_SendCommand((int)command, data, size);
    }
    if (PacketPool_Enqueue(gCommCore.work + 0x5A0, (int)command, data, size, 1, 1)) {
        return 1;
    }
    if (CommState_GetLinkMode() == 10) {
        Comm_SetLinkError();
    }
    return 0;
}

int Comm_SendFixedParentCommand(int command, void *data) {
    return Comm_SendParentCommand((void *)command, data, 0);
}

int Comm_GetSendBufferFreeSpace(void) {
    return ByteRing_GetFreeSize(gCommCore.work + 0x498);
}

void Comm_FinishCommand(int netId, int command, int size, void *data, PacketCursor *cursor) {
    Comm_CallCommandRecv(netId, command, size, data);
    cursor->command = 0xEE;
    cursor->size = 0xFFFF;
    cursor->data = NULL;
    cursor->received = 0;
}

#pragma force_active on

void Comm_ReceiveCommands(CommRing *ring, int netId, u8 *buffer, PacketCursor *cursor) {
    while (ByteRing_GetReadableSize(ring)) {
        int command = cursor->command;
        if (command == 0xEE) {
            command = ByteRing_ReadByte(ring);
            if (command == 0xEE) {
                continue;
            }
        }
        int start = ring->read;
        cursor->command = command;
        int size = cursor->size;
        if (size == 0xFFFF) {
            size = Comm_GetCommandSize(command);
            if (gCommCore.work[0x6B8]) {
                return;
            }
            if (size == 0xFFFF) {
                if (ByteRing_GetReadableSize(ring) < 1) {
                    ring->read = start;
                    return;
                }
                size = ByteRing_ReadByte(ring) << 8;
                size += ByteRing_ReadByte(ring);
                start = ring->read;
            }
            cursor->size = size;
        }
        if (Comm_CommandHasBuffer(command)) {
            if (!cursor->data) {
                cursor->data = Comm_GetCommandBuffer(command, netId, cursor->size);
            }
            int copied = ByteRing_Read(ring, buffer, size - cursor->received);
            if (cursor->data) {
                MI_CpuCopy8(buffer, cursor->data + cursor->received, copied);
            }
            cursor->received = (s32)((u32)cursor->received + (u32)copied);
            if (cursor->received >= size) {
                Comm_FinishCommand(netId, command, size, cursor->data, cursor);
                if (command == 17) {
                    break;
                }
            }
        } else {
            if (ByteRing_GetReadableSize(ring) >= size) {
                ByteRing_Read(ring, buffer, size);
                Comm_FinishCommand(netId, command, size, buffer, cursor);
                if (command == 17) {
                    break;
                }
            } else {
                ring->read = start;
                return;
            }
        }
    }
}

#pragma force_active off

void Comm_ReceiveFromParent(void) {
    u8 *work = gCommCore.work;
    if (work != NULL && work[0x6BA] == 0) {
        ByteRing_Commit(work + 0x4A4);
        if (ByteRing_GetReadableSize(gCommCore.work + 0x4A4) > 0) {
            work = gCommCore.work;
            Comm_ReceiveCommands((CommRing *)(work + 0x4A4), 0, *(u8 **)(work + 0x494), (PacketCursor *)(work + 0x620));
        }
    }
}

void Comm_ReceiveFromChildren(void) {
    int count;
    int i;
    int offset;
    if (gCommCore.work == NULL || gCommCore.work[0x6BA] != 0) {
        return;
    }
    count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
    i = 0;
    if (count > 0) {
        offset = i;
        do {
            ByteRing_Commit(gCommCore.work + 0x51C + offset);
            if (ByteRing_GetReadableSize(gCommCore.work + 0x51C + offset) > 0) {
                Comm_ReceiveCommands((CommRing *)(gCommCore.work + 0x51C + offset),
                    i,
                    *(u8 **)(gCommCore.work + 0x494),
                    (PacketCursor *)(gCommCore.work + 0x5C0 + offset));
            }
            i++;
            offset += 0xC;
        } while (i < count);
    }
}

int Comm_IsNetIdConnected(int slot) {
    if (gCommCore.work == NULL) {
        return 0;
    }
    if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        if (gCommCore.work[0x6B6] != 0) {
            u16 mask = ov00_021EE4FC();
            if ((mask & (1 << slot)) != 0) {
                return 1;
            }
        }
        return 0;
    }
    if (!Comm_IsActive()) {
        return 0;
    }
    if (Wireless_GetState() != 4) {
        return 0;
    }
    if (slot == Comm_GetLocalNetId()) {
        return 1;
    }
    if (Comm_GetLocalNetId() == 0) {
        if ((Wireless_GetConnectedBitmap() & (1 << slot)) != 0) {
            return 1;
        }
    } else if ((*(u16 *)(gCommCore.work + 0x694) & (1 << slot)) != 0) {
        return 1;
    }
    return 0;
}

int Comm_CountConnected(void) {
    int count = 0;
    int i;
    for (i = 0; i < 8; i++) {
        if (Comm_IsNetIdConnected((u16)i)) {
            count++;
        }
    }
    return count;
}

int Comm_IsActive(void) {
    if (gCommCore.work != NULL && Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        return 1;
    }
    return CommWireless_Exists();
}

void Comm_SetLocalMoveSpeed(int value) {
    gCommCore.work[0x65F] = value;
}

int Comm_GetMoveSpeed(int slot) {
    u8 *entry = gCommCore.work + slot;
    return entry[0x654];
}

u16 Comm_TakeDirectionKeys(int slot) {
    u16 value;
    if (gCommCore.work == NULL) {
        return 0;
    }
    value = ((u16 *)(gCommCore.work + 0x644))[slot];
    ((u16 *)(gCommCore.work + 0x644))[slot] = 0;
    return value;
}

void Comm_EnableKeySync(void) {
    if (gCommCore.work != NULL) {
        *(u16 *)(gCommCore.work + 0x65C) |= 0x8000;
    }
}

void Comm_DisableKeySync(void) {
    if (gCommCore.work != NULL) {
        *(u16 *)(gCommCore.work + 0x65C) = 0;
    }
}

int Comm_IsKeySyncEnabled(void) {
    if (gCommCore.work != NULL) {
        return *(u16 *)(gCommCore.work + 0x65C) & 0x8000;
    }
    return 1;
}

int sub_0203753C(int command, void *data, int size) {
    if (Comm_GetTransferMode() == 1) {
        return PacketPool_Enqueue(gCommCore.work + 0x580, command, data, size, 1, 0);
    }
    return PacketPool_Enqueue(gCommCore.work + 0x5A0, command, data, size, 1, 0);
}

void Comm_UpdateTransferModeHandshake(void) {
    s32 result;

    if (gCommCore.work == NULL) {
        return;
    }
    switch (gCommCore.work[0x6AB]) {
    case 1:
        if (Comm_GetTransferMode() == 1) {
            result = Comm_SendFixedCommand(11, gCommCore.work + 0x6AC);
        } else {
            result = Comm_SendParentCommand((void *)11, gCommCore.work + 0x6AC, 1);
        }
        if (result != 0) {
            gCommCore.work[0x6AB] = 2;
        }
        break;
    case 3:
        if (Comm_SendFixedCommand(12, gCommCore.work + 0x6AC) != 0) {
            Comm_RequestTransferMode(gCommCore.work[0x6AC]);
            gCommCore.work[0x6AB] = 0;
        }
        break;
    }
}

void Comm_RecvTransferModeRequest(int netId, int size, const u8 *data) {
    if (Comm_GetLocalNetId() == 0) {
        gCommCore.work[0x6AB] = 1;
        gCommCore.work[0x6AC] = *data;
    }
}

void Comm_RecvTransferModeChange(int netId, int size, const u8 *data) {
    if (Comm_GetLocalNetId() != 0) {
        gCommCore.work[0x6AC] = *data;
        gCommCore.work[0x6AB] = 3;
    }
}

void Comm_RecvTransferModeAck(int netId, int size, const u8 *data) {
    if (Comm_GetLocalNetId() == 0 && gCommCore.work[0x6AB] == 2) {
        Comm_RequestTransferMode(*data);
        gCommCore.work[0x6AB] = 0;
    }
}

int Comm_GetLocalNetId(void) {
    if (gCommCore.work != NULL) {
        if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
            int id = WifiConnection_GetAid();
            if (id != -1) {
                return (u16)id;
            }
        } else {
            return Wireless_GetAid();
        }
    }
    return 0;
}

int Comm_SendFixedCommandNoCopy(int command, void *data) {
    return Comm_SendCommandNoCopy(command, data, 0);
}

int Comm_SendFixedCommand(int command, void *data) {
    return Comm_SendCommand(command, data, 0);
}

int Comm_SendEmptyCommand(int command) {
    return Comm_SendCommand(command, NULL, 0);
}

int Comm_GetConnectedChildren(void) {
    return CommWireless_GetConnectedChildren();
}

int Comm_HasLinkError(void) {
    if (gCommCore.work != NULL && gCommCore.work[0x6B8] != 0) {
        CommState_SetErrorWatch(1, 1);
        return 1;
    }
    return CommWireless_IsLinkLost();
}

int Comm_GetChildFrameSize(u16 linkMode) {
    if ((u32)Comm_GetMaxChildren(linkMode) >= 5) {
        return 12;
    }
    if (Comm_GetTransferMode() == 0) {
        return 12;
    }
    return 0x26;
}

int Comm_GetMaxPlayers(int value) {
    return Comm_GetMaxChildren((u16)value) + 1;
}

int Comm_GetMinPlayers(int value) {
    return Comm_GetMinChildren((u16)value) + 1;
}

void Comm_RecvShutdown(void) {
    u8 value;
    if (!CommWireless_IsShutdownRequested() && !Comm_GetLocalNetId()) {
        Comm_SendFixedParentCommand(2, &value);
    }
    CommWireless_RequestShutdown();
}

#pragma force_active on

void CommRandom_Init(CommRandom *seed) {
    RTCDate date;
    RTCTime time;

    GF_RTC_CopyDateTime(&date, &time);
    seed->state = (time.second + gSystem.vblankCounter) + ((time.minute + ((time.hour + ((date.day + ((date.month + ((u64)date.year << 4)) << 5)) << 5)) << 6)) << 6);
    seed->multiplier = 0x5D588B656C078965ULL;
    seed->increment = 0x269EC3;
}

#pragma force_active off

int Comm_ParentQueueHasCommand(int value) {
    return PacketPool_HasCommand(gCommCore.work + 0x5A0, value);
}

int Comm_IsParentQueueEmpty(void) {
    return PacketPool_AllNodesFree(gCommCore.work + 0x5A0);
}

int Comm_IsChildQueueEmpty(void) {
    return PacketPool_AllNodesFree(gCommCore.work + 0x580);
}

void Comm_SetWifiConnected(int value) {
    gCommCore.work[0x6B6] = value;
}

int Comm_IsWifiConnected(void) {
    return gCommCore.work[0x6B6];
}

void Comm_SetBattleSlot(int value, int slot) {
    if (gCommCore.work != NULL) {
        u8 *entry = gCommCore.work + slot;
        entry[0x6A6] = value;
    }
}

int Comm_GetBattleSlot(int slot) {
    if (gCommCore.work != NULL) {
        u8 *entry = gCommCore.work + slot;
        if (entry[0x6A6] != 0xFF) {
            return entry[0x6A6];
        }
    }
    return slot;
}

int sub_020378CC(void) {
    if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        return ov00_021E7080();
    }
    return 0;
}

void sub_020378E4(int value) {
    int i;
    if (Comm_IsWifiLinkMode(CommState_GetLinkMode()) && *(int *)(gCommCore.work + 0x664) != value) {
        *(int *)(gCommCore.work + 0x664) = value;
        if (value != 0) {
            *(int *)(gCommCore.work + 0x668) = 0;
            for (i = 0; i < 8; i++) {
                *(int *)(gCommCore.work + 0x66C + i * 4) = 0;
            }
        }
    }
}

void sub_02037930(int value) {
    sub_020378E4(value);
    if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        if (value != 0) {
            ov00_021E70CC(0);
        } else {
            ov00_021E70CC(1);
        }
    }
}

BOOL Comm_IsSendingMoveKeys(void) {
    return gCommCore.work[0x6B1] != 0;
}

void Comm_SetLinkError(void) {
    gCommCore.work[0x6B8] = 1;
}

void Comm_StopFrameUpdate(void) {
    if (gCommCore.work != NULL) {
        gCommCore.work[0x6B9] = 1;
    }
}
