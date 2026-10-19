#include "global.h"

#include "constants/comm_type.h"

#include "dwc/dwc_main_internal.h"

#include "comm_wireless.h"
#include "communication_command.h"
#include "communication_core_internal.h"
#include "communication_player_state.h"
#include "error_handling.h"
#include "gf_rtc.h"
#include "heap.h"
#include "link_synchronization.h"
#include "system.h"
#include "wifi_connection.h"
#include "wireless_manager.h"

extern void *SysTask_CreateOnVBlankQueue(void *func, void *data, int priority);
extern void SysTask_Destroy(void *task);
extern void LinkSync_Free(void);
extern int CommState_GetLinkMode(void);
extern void CommState_SetErrorWatch(int, int);
extern void CommState_CheckLinkError(int);
extern void LinkSync_Update(void);
extern void CommState_Main(void);
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
extern int PacketPool_WriteFrame(void *queue, ByteWriter *writer, int);
extern void PacketPool_Init(void *queue, int count, void *ring);
extern void PacketPool_Reset(void *queue);
extern void PacketPool_Free(void *queue);
extern int PacketPool_HasCommand(void *queue, int);
extern int Comm_GetMaxChildren(u16);
extern int Comm_IsWifiLinkMode(int);

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
        gCommCore.allocation = Heap_Alloc(HEAP_ID_15, sizeof(CommWork) + 32);
        gCommCore.work = (CommWork *)(32 - ((u32)gCommCore.allocation & 31) + (u32)gCommCore.allocation);
        MI_CpuFill8(gCommCore.work, 0, sizeof(CommWork));

        if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
            gCommCore.work->frameDataSize = packetSize * 2 + 0x40;
        } else {
            gCommCore.work->frameDataSize = packetSize + 0x40;
        }
        gCommCore.work->recvDataSize = gCommCore.work->frameDataSize * count;
        gCommCore.work->transferMode = 0;
        packetSize = 42;
        gCommCore.work->packetSize = packetSize;
        gCommCore.work->parentRecvData = Heap_Alloc(HEAP_ID_15, gCommCore.work->frameDataSize * 2);
        gCommCore.work->commandBuffer = Heap_Alloc(HEAP_ID_15, gCommCore.work->frameDataSize);
        gCommCore.work->recvData = Heap_Alloc(HEAP_ID_15, gCommCore.work->recvDataSize);
        gCommCore.work->childFrameData = Heap_Alloc(HEAP_ID_15, gCommCore.work->recvDataSize);

        if (CommState_GetLinkMode() == COMM_TYPE_10) {
            PacketPool_Init(&gCommCore.work->childQueue, 100, &gCommCore.work->childSendRing);
            PacketPool_Init(&gCommCore.work->parentQueue, 800, &gCommCore.work->parentSendRing);
        } else {
            PacketPool_Init(&gCommCore.work->childQueue, 20, &gCommCore.work->childSendRing);
            PacketPool_Init(&gCommCore.work->parentQueue, packetSize + 238, &gCommCore.work->parentSendRing);
        }
    } else {
        existing = 1;
        if (gCommCore.work == NULL) {
            GF_AssertFail();
        }
    }

    gCommCore.work->connectedNetIds = 0;
    for (i = 0; i < 4; i++) {
        gCommCore.work->battleSlots[i] = 0xFF;
    }
    if (existing == 0) {
        Comm_Reset();
    }
    CommRandom_Init(&gCommCore.work->random);
    if (existing == 0) {
        gCommCore.work->sendTask = SysTask_CreateOnVBlankQueue(Comm_VBlankSendTask, NULL, 0);
    }
    gCommCore.work->wifiConnected = 0;
    return 1;
}

void Comm_ResetBuffers(void) {
    int count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
    int length;
    int i;

    gCommCore.work->keyScrambleMode = 0;
    gCommCore.work->scrambleTimer = 0;
    MI_CpuFill8(gCommCore.work->recvData, 0, gCommCore.work->recvDataSize);
    MI_CpuFill8(gCommCore.work->recvRings, 0, sizeof(gCommCore.work->recvRings));
    length = gCommCore.work->recvDataSize / count;
    for (i = 0; i < count; i++) {
        ByteRing_Init(&gCommCore.work->recvRings[i], gCommCore.work->recvData + i * length, length);
    }
    MI_CpuFill8(gCommCore.work->childFrameData, 0, gCommCore.work->recvDataSize);
    MI_CpuFill8(gCommCore.work->childFrameRings, 0, sizeof(gCommCore.work->childFrameRings));
    for (i = 0; i < count; i++) {
        ByteRing_Init(&gCommCore.work->childFrameRings[i], gCommCore.work->childFrameData + i * length, length);
    }
    MI_CpuFill8(gCommCore.work->parentSendData, 0, sizeof(gCommCore.work->parentSendData));
    ByteRing_Init(&gCommCore.work->parentSendRing, gCommCore.work->parentSendData, sizeof(gCommCore.work->parentSendData));
    MI_CpuFill8(gCommCore.work->parentFrames[0], 0xEE, sizeof(gCommCore.work->parentFrames));
    MI_CpuFill8(gCommCore.work->parentFrames[1], 0xEE, sizeof(gCommCore.work->parentFrames));
    MI_CpuFill8(gCommCore.work->childSendData, 0, sizeof(gCommCore.work->childSendData));
    ByteRing_Init(&gCommCore.work->childSendRing, gCommCore.work->childSendData, sizeof(gCommCore.work->childSendData));
    MI_CpuFill8(gCommCore.work->childFrames[0], 0xEE, 38);
    MI_CpuFill8(gCommCore.work->childFrames[1], 0xEE, 38);
    gCommCore.work->childFrames[0][0] = 0xFF;
    gCommCore.work->childFrames[1][0] = 0xFF;
    MI_CpuFill8(gCommCore.work->parentRecvData, 0, gCommCore.work->frameDataSize * 2);
    ByteRing_Init(&gCommCore.work->parentRecvRing, gCommCore.work->parentRecvData, gCommCore.work->frameDataSize * 2);
    gCommCore.work->childQueuePending = 0;
    gCommCore.work->parentQueuePending = 0;
    for (i = 0; i < 8; i++) {
        gCommCore.work->childFrameCounts[i] = 0;
        gCommCore.work->waitingFirstFrame[i] = 1;
        gCommCore.work->directionKeys[i] = 0;
        gCommCore.work->recvCursors[i].command = 0xEE;
        gCommCore.work->recvCursors[i].size = 0xFFFF;
        gCommCore.work->recvCursors[i].data = NULL;
        gCommCore.work->recvCursors[i].received = 0;
        gCommCore.work->childSentFrames[i] = 0;
    }
    gCommCore.work->sentFrames = 0;
    gCommCore.work->parentCursor.command = 0xEE;
    gCommCore.work->parentCursor.size = 0xFFFF;
    gCommCore.work->parentCursor.data = NULL;
    gCommCore.work->parentCursor.received = 0;
    gCommCore.work->waitingParentFrame = 1;
    gCommCore.work->unk_6B3 = 1;
    gCommCore.initialized = 0;
    PacketPool_Reset(&gCommCore.work->childQueue);
    PacketPool_Reset(&gCommCore.work->parentQueue);
    gCommCore.work->unk_6B7 = 0;
}

void Comm_Reset(void) {
    gCommCore.work->childFrameIndex = 0;
    gCommCore.work->parentFrameIndex = 0;
    gCommCore.work->continuousSend = 1;
    Comm_ResetBuffers();
    gCommSendStages.stage[0] = 4;
    gCommSendStages.stage[1] = 4;
}

void Comm_OnTransferModeChanged(void) {
    Comm_ResetBuffers();
}

void Comm_ResetNetIdBuffers(int slot) {
    int count;
    CommWork *work;
    int length;

    gCommCore.work->childFrameCounts[slot] = 0;
    gCommCore.work->waitingFirstFrame[slot] = 1;
    gCommCore.work->childSentFrames[slot] = 0;
    count = Comm_GetMaxChildren((u16)CommState_GetLinkMode());
    work = gCommCore.work;
    work = (CommWork *)work;
    length = work->recvDataSize / (count + 1);
    ByteRing_Init(&work->childFrameRings[slot], work->childFrameData + slot * length, length);
    work = gCommCore.work;
    ByteRing_Init(&work->recvRings[slot], work->recvData + slot * length, length);
    gCommCore.work->recvCursors[slot].command = 0xEE;
    gCommCore.work->recvCursors[slot].size = 0xFFFF;
    gCommCore.work->recvCursors[slot].data = NULL;
    gCommCore.work->recvCursors[slot].received = 0;
}

void Comm_ResetDisconnectedNetIds(void) {
    int i;
    for (i = 1; i < 8; i++) {
        if (!Comm_IsNetIdConnected((u16)i) && gCommCore.work->waitingFirstFrame[i] == 0) {
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
    if (gCommCore.work->transferMode == 2) {
        gCommCore.work->transferMode = 0;
        changed = 1;
    }
    if (gCommCore.work->transferMode == 3) {
        changed = 1;
        gCommCore.work->transferMode = 1;
    }
    if (changed) {
        Comm_OnTransferModeChanged();
    }
    Comm_UpdateTransferModeHandshake();
}

void Comm_RequestTransferMode(int connected) {
    int state = gCommCore.work->transferMode;
    if (state == 0 && connected == 1) {
        gCommCore.work->transferMode = 3;
        return;
    }
    if (state == 1 && connected == 0) {
        gCommCore.work->transferMode = 2;
    }
}

void Comm_RequestTransferMode1(void) {
    Comm_RequestTransferMode(1);
}

void Comm_RequestTransferMode0(void) {
    Comm_RequestTransferMode(0);
}

int Comm_GetTransferMode(void) {
    int state = gCommCore.work->transferMode;
    if (state == 2) {
        return 1;
    }
    if (state == 3) {
        return 0;
    }
    return state;
}

BOOL Comm_IsTransferMode1(void) {
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
        SysTask_Destroy(gCommCore.work->sendTask);
        gCommCore.work->sendTask = NULL;
        Heap_Free(gCommCore.work->parentRecvData);
        Heap_Free(gCommCore.work->commandBuffer);
        Heap_Free(gCommCore.work->recvData);
        Heap_Free(gCommCore.work->childFrameData);
        PacketPool_Free(&gCommCore.work->parentQueue);
        PacketPool_Free(&gCommCore.work->childQueue);
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
    CommWork *work;

    CommState_Main();
    work = gCommCore.work;
    if (work != NULL) {
        if (work->updateStopped == 0) {
            work->frameCount++;
            gCommCore.updateLock = 0;
            Comm_UpdateTransferMode();
            work = gCommCore.work;
            work->heldKeys |= gSystem.heldKeys & 0x7FFF;
            Comm_ScrambleDirectionKeys();
            Comm_UpdateChildSend();
            work = gCommCore.work;
            work->heldKeys &= 0x8000;
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
        CommWireless_Main(gCommCore.work->connectedNetIds);
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

void Comm_ResetInTransferMode1(void) {
    u8 lock = gCommCore.updateLock;
    gCommCore.updateLock = 0;
    if (gCommCore.work != NULL) {
        gCommCore.work->transferMode = 1;
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

void Comm_UpdateChildSend(void) {
    int i;
    int count;

    if (Comm_IsWifiPeerLinkMode(CommState_GetLinkMode())) {
        if (gCommCore.work->wifiConnected) {
            if (gCommCore.work->continuousSend) {
                if (!Comm_HasNoChildBacklog()) {
                    return;
                }
                if (gCommSendStages.stage[1] == 4) {
                    Comm_BuildChildFrame(gCommCore.work->childFrames[0]);
                    gCommSendStages.stage[1] = 2;
                }
            } else if (gCommSendStages.stage[1] == 4) {
                if (!Comm_BuildChildFrame(gCommCore.work->childFrames[0])) {
                    return;
                }
                gCommSendStages.stage[1] = 2;
            }
            if (Comm_IsSendSkippedThisFrame()) {
                return;
            }
            if (WifiConnection_SendToAll(gCommCore.work->childFrames[0], 38)) {
                count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
                for (i = 0; i < count; i++) {
                    if (Comm_IsNetIdConnected((u16)i)) {
                        gCommCore.work->childSentFrames[i]++;
                    }
                }
                gCommSendStages.stage[1] = 4;
            }
        }
    } else if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        if (!gCommCore.work->wifiConnected) {
            return;
        }
        if (gCommCore.work->continuousSend) {
            if (gCommCore.work->sentFrames > 3) {
                return;
            }
            if (gCommSendStages.stage[1] == 4) {
                Comm_BuildChildFrame(gCommCore.work->childFrames[0]);
                gCommSendStages.stage[1] = 2;
            }
        } else if (gCommSendStages.stage[1] == 4) {
            if (!Comm_BuildChildFrame(gCommCore.work->childFrames[0])) {
                return;
            }
            gCommSendStages.stage[1] = 2;
        }
        if (Comm_IsSendSkippedThisFrame()) {
            return;
        }
        if (!WifiConnection_SendToHost(gCommCore.work->childFrames[0], 38)) {
            return;
        }
        gCommSendStages.stage[1] = 4;
        gCommCore.work->sentFrames++;
    } else if (Wireless_GetState() == 4) {
        if (!Comm_IsNetIdConnected(Comm_GetLocalNetId())) {
            return;
        }
        if (gCommSendStages.stage[1] == 4 && gCommCore.work->sentFrames <= 3) {
            Comm_BuildChildFrame(gCommCore.work->childFrames[gCommCore.work->childFrameIndex]);
            Comm_BuildChildFrame(gCommCore.work->childFrames[1 - gCommCore.work->childFrameIndex]);
            gCommSendStages.stage[1] = 0;
        }
        Comm_SendChildFrame();
    }
}

BOOL Comm_FillParentFrameFromChildren(int slot) {
    int bad = 0;
    int stride = Comm_GetChildFrameSize((u16)CommState_GetLinkMode());
    int count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
    int i;

    for (i = 0; i < count; i++) {
        ByteRing_Commit(&gCommCore.work->childFrameRings[i]);
        if (Comm_IsNetIdConnected((u16)i)) {
            gCommCore.work->parentFrames[slot][i * stride] = 14;
        } else {
            bad++;
            gCommCore.work->parentFrames[slot][i * stride] = 0xFF;
            continue;
        }
        ByteRing_Read(&gCommCore.work->childFrameRings[i], &gCommCore.work->parentFrames[slot][i * stride], stride);
        if (gCommCore.work->parentFrames[slot][i * stride] == 14) {
            bad++;
        }
    }
    if (bad == count) {
        return FALSE;
    }
    return TRUE;
}

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
            Comm_FillParentFrameFromChildren(gCommCore.work->parentFrameIndex);
            gCommCore.initialized = 1;
        }
    }
    if (Wireless_GetState() == 4) {
        if (!Wireless_SendData(gCommCore.work->parentFrames[gCommCore.work->parentFrameIndex], 0xC0, 14, Comm_OnParentFrameSent)) {
            gCommSendStages.stage[0]--;
        }
    }
    if (gCommSendStages.stage[0] == 1 || gCommSendStages.stage[0] == 3) {
        gCommCore.initialized = 0;
        for (i = 0; i < count; i++) {
            if (Comm_IsNetIdConnected((u16)i)) {
                gCommCore.work->childSentFrames[i]++;
            }
        }
        Comm_OnParentFrameReceived(0, gCommCore.work->parentFrames[gCommCore.work->parentFrameIndex], 0xC0);
        gCommCore.work->parentFrameIndex = 1 - gCommCore.work->parentFrameIndex;
    }
    if (Wireless_GetState() != 4) {
        gCommSendStages.stage[0]++;
    }
}

BOOL Comm_HasNoChildBacklog(void) {
    int count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
    int i;
    for (i = 1; i < count; i++) {
        if (Comm_IsNetIdConnected((u16)i) && gCommCore.work->childSentFrames[i] > 3) {
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
        if (gCommCore.work->continuousSend != 0) {
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
        if (!WifiConnection_SendFromHost(gCommCore.work->parentFrames[0], sizeof(gCommCore.work->parentFrames[0]))) {
            return;
        }
        gCommSendStages.stage[0] = 4;
        for (i = 0; i < count; i++) {
            if (Comm_IsNetIdConnected((u16)i)) {
                gCommCore.work->childSentFrames[i]++;
            }
        }
    } else {
        if (Wireless_GetState() != 4 || gCommSendStages.stage[0] != 4 || !Comm_HasNoChildBacklog()) {
            return;
        }
        if (Comm_GetTransferMode() == 0) {
            Comm_BuildParentFrame(gCommCore.work->parentFrames[gCommCore.work->parentFrameIndex]);
            Comm_BuildParentFrame(gCommCore.work->parentFrames[1 - gCommCore.work->parentFrameIndex]);
        }
        gCommSendStages.stage[0] = 0;
        Comm_SendParentFrame();
    }
}

void Comm_RecvParentFrameCallback(int netId, const u8 *data, int size) {
    Comm_OnParentFrameReceived(netId, data, size);
}

void Comm_OnParentFrameReceived(int netId, const u8 *data, int size) {
    int stride;
    int count;
    int i;

    gCommCore.work->sentFrames--;
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
    if (gCommCore.work->waitingParentFrame != 0 && (data[0] & 1)) {
        return;
    }
    gCommCore.work->waitingParentFrame = 0;
    if (Comm_GetTransferMode() == 1) {
        stride = Comm_GetChildFrameSize((u16)CommState_GetLinkMode());
        count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
        for (i = 0; i < count; i++) {
            if (data[0] == 0xFF) {
                gCommCore.work->connectedNetIds &= ~(1 << i);
            } else {
                gCommCore.work->connectedNetIds |= 1 << i;
            }
            if (data[0] == 0xFF) {
                data += stride;
                continue;
            }
            if (data[0] == 0xE) {
                data += stride;
                continue;
            }
            if (gCommCore.work->waitingFirstFrame[i] != 0 && (data[0] & 1)) {
                data += stride;
                continue;
            }
            data++;
            ByteRing_Write(&gCommCore.work->recvRings[i], data, stride - 1, i + 0x54C);
            data += stride - 1;
            gCommCore.work->waitingFirstFrame[i] = 0;
        }
    } else {
        gCommCore.work->connectedNetIds = data[1];
        gCommCore.work->connectedNetIds <<= 8;
        gCommCore.work->connectedNetIds += data[2];
        ByteRing_Write(&gCommCore.work->parentRecvRing, data + 4, data[3], 0x560);
    }
}

void Comm_RecvChildFrameCallback(int netId, const u8 *data, int size) {
    Comm_OnChildFrameReceived(netId, data, size);
}

void Comm_OnChildFrameReceived(int slot, const u8 *data, int size) {
    int frameSize;

    gCommCore.work->childSentFrames[slot]--;
    if (data == NULL) {
        return;
    }
    if (gCommCore.work->waitingFirstFrame[slot] != 0 && (data[0] & 1) != 0) {
        return;
    }
    gCommCore.work->waitingFirstFrame[slot] = 0;
    if (Comm_GetTransferMode() == 1) {
        frameSize = Comm_GetChildFrameSize((u16)CommState_GetLinkMode());
        Comm_GetMaxChildren((u16)CommState_GetLinkMode());
        if ((data[0] & 2) == 0) {
            ByteRing_Write(&gCommCore.work->childFrameRings[slot], data, frameSize, 0x5A5);
        }
        gCommCore.work->childFrameCounts[slot]++;
    } else {
        Comm_ReadFrameKeys(data, slot);
        if ((data[0] & 2) == 0) {
            ByteRing_Write(&gCommCore.work->recvRings[slot], data + 1, 11, 0x5AE);
        }
    }
}

void Comm_RecvPeerFrameCallback(int slot, const u8 *data) {
    int frameSize;

    gCommCore.work->childSentFrames[slot]--;
    if (data == NULL) {
        return;
    }
    if (gCommCore.work->waitingFirstFrame[slot] != 0 && (data[0] & 1) != 0) {
        return;
    }
    gCommCore.work->waitingFirstFrame[slot] = 0;
    if (Comm_GetTransferMode() != 1) {
        return;
    }
    frameSize = Comm_GetChildFrameSize((u16)CommState_GetLinkMode());
    Comm_GetMaxChildren((u16)CommState_GetLinkMode());
    if (data[0] == 0xFF) {
        gCommCore.work->connectedNetIds &= ~(1 << slot);
    } else {
        gCommCore.work->connectedNetIds |= 1 << slot;
    }
    if (data[0] == 0xFF || data[0] == 2 || data[0] == 0xE) {
        return;
    }
    if (gCommCore.work->waitingFirstFrame[slot] != 0 && (data[0] & 1) != 0) {
        return;
    }
    ByteRing_Write(&gCommCore.work->recvRings[slot], data + 1, frameSize - 1, 0x5E7);
    gCommCore.work->waitingFirstFrame[slot] = 0;
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
            if (!Wireless_SendData(gCommCore.work->childFrames[gCommCore.work->childFrameIndex], (u16)frameSize, 14, Comm_OnChildFrameSent)) {
                gCommSendStages.stage[1]--;
                return;
            }
            gCommCore.work->childFrameIndex = 1 - gCommCore.work->childFrameIndex;
            gCommCore.work->sentFrames++;
        } else {
            if ((Wireless_GetConnectedBitmap() & 0xFFFE) == 0) {
                return;
            }
            gCommSendStages.stage[1]++;
            Comm_OnChildFrameSent((void *)1);
            Comm_OnChildFrameReceived(0, gCommCore.work->childFrames[gCommCore.work->childFrameIndex], (u16)frameSize);
            gCommCore.work->childFrameIndex = 1 - gCommCore.work->childFrameIndex;
            gCommCore.work->sentFrames++;
        }
    }
}

static inline u32 CommWork_Random(CommWork *w, u32 max) {
    CommRandom *rng = &w->random;
    rng->state = rng->multiplier * rng->state + rng->increment;
    return (u32)((((u64)rng->state >> 32) * max) >> 32);
}

void Comm_ScrambleDirectionKeys(void) {
    CommWork *w = gCommCore.work;
    u16 result = 0;
    if (w->keyScrambleMode == 0 || !(w->heldKeys & PAD_PLUS_KEY_MASK)) {
        return;
    }
    if (w->keyScrambleMode == 2) {
        if (w->heldKeys & PAD_KEY_LEFT) {
            result |= PAD_KEY_RIGHT;
        }
        if (w->heldKeys & PAD_KEY_RIGHT) {
            result |= PAD_KEY_LEFT;
        }
        if (w->heldKeys & PAD_KEY_UP) {
            result |= PAD_KEY_DOWN;
        }
        if (w->heldKeys & PAD_KEY_DOWN) {
            result |= PAD_KEY_UP;
        }
    } else if (w->scrambleDirection != 0) {
        result = w->scrambleDirection;
        w->scrambleTimer--;
        if (gCommCore.work->scrambleTimer < 0) {
            gCommCore.work->scrambleDirection = 0;
        }
    } else {
        switch (CommWork_Random(w, 4)) {
        case 0:
            result = PAD_KEY_LEFT;
            break;
        case 1:
            result = PAD_KEY_RIGHT;
            break;
        case 2:
            result = PAD_KEY_UP;
            break;
        case 3:
            result = PAD_KEY_DOWN;
            break;
        }
        w = gCommCore.work;
        w->scrambleTimer = CommWork_Random(w, 16);
        gCommCore.work->scrambleDirection = result;
    }
    gCommCore.work->heldKeys &= ~PAD_PLUS_KEY_MASK;
    gCommCore.work->heldKeys += result;
}

int Comm_ReadFrameKeys(const u8 *data, int slot) {
    u8 value;
    u8 direction;
    int decoded;

    gCommCore.work->directionKeys[slot] = 0;
    value = *data;
    if ((value & 0x10) == 0x10) {
        direction = value & 0x0c;
        if (direction == 0) {
            gCommCore.work->directionKeys[slot] |= 0x40;
        } else if (direction == 4) {
            gCommCore.work->directionKeys[slot] |= 0x80;
        } else if (direction == 8) {
            gCommCore.work->directionKeys[slot] |= 0x20;
        } else if (direction == 0x0c) {
            gCommCore.work->directionKeys[slot] |= 0x10;
        }
        decoded = *data >> 5;
        decoded &= 7;
        gCommCore.work->moveSpeeds[slot] = decoded;
    }
    return 1;
}

int Comm_WriteFrameKeys(u8 *data) {
    u16 flags;
    if (gCommCore.work->keysLocked != 0) {
        return 0;
    }
    if (!Comm_IsKeySyncEnabled()) {
        return 0;
    }
    if (gCommCore.work->keySendTimer != 0) {
        gCommCore.work->keySendTimer--;
    }
    flags = gCommCore.work->heldKeys;
    if (flags & 0x40) {
        *data |= 0x10;
        gCommCore.work->keySendTimer = 8;
    } else if (flags & 0x80) {
        *data |= 0x14;
        gCommCore.work->keySendTimer = 8;
    } else if (flags & 0x20) {
        *data |= 0x18;
        gCommCore.work->keySendTimer = 8;
    } else if (flags & 0x10) {
        *data |= 0x1C;
        gCommCore.work->keySendTimer = 8;
    }
    *data |= gCommCore.work->localMoveSpeed << 5;
    return 0;
}

int Comm_BuildChildFrame(u8 *data) {
    ByteWriter receive;
    int count = Comm_GetChildFrameSize((u16)CommState_GetLinkMode());

    Comm_GetMaxChildren((u16)CommState_GetLinkMode());
    if (gCommCore.work->childQueuePending == 0) {
        *data = 0;
    } else {
        *data = 1;
    }
    if (Comm_GetTransferMode() == 0) {
        Comm_WriteFrameKeys(data);
    }
    gCommCore.work->childQueuePending = 0;
    if (PacketPool_AllNodesFree(&gCommCore.work->childQueue)) {
        *data |= 2;
        if (*data == 2) {
            return 0;
        }
    } else {
        receive.remaining = count - 1;
        receive.dest = data + 1;
        if (PacketPool_WriteFrame(&gCommCore.work->childQueue, &receive, 1) == 0) {
            gCommCore.work->childQueuePending = 1;
        }
        if (Comm_GetTransferMode() == 1) {
            gCommCore.work->frameSequence++;
            *data |= (gCommCore.work->frameSequence << 4) & 0xF0;
        }
    }
    return 1;
}

void Comm_BuildParentFrame(u8 *data) {
    ByteWriter receive;
    u16 value;

    data[0] = 0xB;
    if (gCommCore.work->parentQueuePending == 0) {
        data[1] = 0;
    } else {
        data[1] = 1;
    }
    value = Wireless_GetConnectedBitmap();
    data[2] = value >> 8;
    data[3] = value;
    receive.remaining = 0xBB;
    receive.dest = data + 5;
    if (PacketPool_WriteFrame(&gCommCore.work->parentQueue, &receive, 0)) {
        gCommCore.work->parentQueuePending = 0;
        data[4] = 0xBB - receive.remaining;
    } else {
        gCommCore.work->parentQueuePending = 1;
        data[4] = 0xBB;
    }
}

BOOL Comm_IsSendSkippedThisFrame(void) {
    int divisor = gCommCore.work->sendSkipInterval;
    if (divisor == 0) {
        return FALSE;
    }
    if (gCommCore.work->frameCount % divisor == 0) {
        return TRUE;
    }
    return FALSE;
}

int Comm_SendCommandNoCopy(int command, void *data, int size) {
    if (!Comm_IsNetIdConnected(Comm_GetLocalNetId())) {
        return 0;
    }
    if (PacketPool_Enqueue(&gCommCore.work->childQueue, command, data, size, 1, 0)) {
        return 1;
    }
    if (CommState_GetLinkMode() == COMM_TYPE_10) {
        Comm_SetLinkError();
    }
    return 0;
}

int Comm_SendCommand(int command, void *data, int size) {
    if (!Comm_IsNetIdConnected(Comm_GetLocalNetId())) {
        return 0;
    }
    if (PacketPool_Enqueue(&gCommCore.work->childQueue, command, data, size, 1, 1)) {
        return 1;
    }
    if (CommState_GetLinkMode() == COMM_TYPE_10) {
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
    if (PacketPool_Enqueue(&gCommCore.work->parentQueue, command, data, size, 1, 0)) {
        return 1;
    }
    if (CommState_GetLinkMode() == COMM_TYPE_10) {
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
    if (PacketPool_Enqueue(&gCommCore.work->parentQueue, (int)command, data, size, 1, 1)) {
        return 1;
    }
    if (CommState_GetLinkMode() == COMM_TYPE_10) {
        Comm_SetLinkError();
    }
    return 0;
}

int Comm_SendFixedParentCommand(int command, void *data) {
    return Comm_SendParentCommand((void *)command, data, 0);
}

int Comm_GetSendBufferFreeSpace(void) {
    return ByteRing_GetFreeSize(&gCommCore.work->childSendRing);
}

void Comm_FinishCommand(int netId, int command, int size, void *data, PacketCursor *cursor) {
    Comm_CallCommandRecv(netId, command, size, data);
    cursor->command = 0xEE;
    cursor->size = 0xFFFF;
    cursor->data = NULL;
    cursor->received = 0;
}

void Comm_ReceiveCommands(ByteRing *ring, int netId, u8 *buffer, PacketCursor *cursor) {
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
            if (gCommCore.work->linkError) {
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

void Comm_ReceiveFromParent(void) {
    if (gCommCore.work != NULL && gCommCore.work->receiveStopped == 0) {
        ByteRing_Commit(&gCommCore.work->parentRecvRing);
        if (ByteRing_GetReadableSize(&gCommCore.work->parentRecvRing) > 0) {
            Comm_ReceiveCommands(&gCommCore.work->parentRecvRing, 0, gCommCore.work->commandBuffer, &gCommCore.work->parentCursor);
        }
    }
}

void Comm_ReceiveFromChildren(void) {
    int count;
    int i;

    if (gCommCore.work == NULL || gCommCore.work->receiveStopped != 0) {
        return;
    }
    count = Comm_GetMaxChildren((u16)CommState_GetLinkMode()) + 1;
    for (i = 0; i < count; i++) {
        ByteRing_Commit(&gCommCore.work->recvRings[i]);
        if (ByteRing_GetReadableSize(&gCommCore.work->recvRings[i]) > 0) {
            Comm_ReceiveCommands(&gCommCore.work->recvRings[i], i, gCommCore.work->commandBuffer, &gCommCore.work->recvCursors[i]);
        }
    }
}

int Comm_IsNetIdConnected(int slot) {
    if (gCommCore.work == NULL) {
        return 0;
    }
    if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        if (gCommCore.work->wifiConnected != 0) {
            u16 mask = DWC_GetAIDBitmap();
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
    } else if ((gCommCore.work->connectedNetIds & (1 << slot)) != 0) {
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
    gCommCore.work->localMoveSpeed = value;
}

int Comm_GetMoveSpeed(int slot) {
    return gCommCore.work->moveSpeeds[slot];
}

u16 Comm_TakeDirectionKeys(int slot) {
    u16 value;
    if (gCommCore.work == NULL) {
        return 0;
    }
    value = gCommCore.work->directionKeys[slot];
    gCommCore.work->directionKeys[slot] = 0;
    return value;
}

void Comm_EnableKeySync(void) {
    if (gCommCore.work != NULL) {
        gCommCore.work->heldKeys |= 0x8000;
    }
}

void Comm_DisableKeySync(void) {
    if (gCommCore.work != NULL) {
        gCommCore.work->heldKeys = 0;
    }
}

int Comm_IsKeySyncEnabled(void) {
    if (gCommCore.work != NULL) {
        return gCommCore.work->heldKeys & 0x8000;
    }
    return 1;
}

int Comm_SendParentCommandNoCopyUnchecked(int command, void *data, int size) {
    if (Comm_GetTransferMode() == 1) {
        return PacketPool_Enqueue(&gCommCore.work->childQueue, command, data, size, 1, 0);
    }
    return PacketPool_Enqueue(&gCommCore.work->parentQueue, command, data, size, 1, 0);
}

void Comm_UpdateTransferModeHandshake(void) {
    s32 result;

    if (gCommCore.work == NULL) {
        return;
    }
    switch (gCommCore.work->transferModeHandshake) {
    case 1:
        if (Comm_GetTransferMode() == 1) {
            result = Comm_SendFixedCommand(11, &gCommCore.work->requestedTransferMode);
        } else {
            result = Comm_SendParentCommand((void *)11, &gCommCore.work->requestedTransferMode, 1);
        }
        if (result != 0) {
            gCommCore.work->transferModeHandshake = 2;
        }
        break;
    case 3:
        if (Comm_SendFixedCommand(12, &gCommCore.work->requestedTransferMode) != 0) {
            Comm_RequestTransferMode(gCommCore.work->requestedTransferMode);
            gCommCore.work->transferModeHandshake = 0;
        }
        break;
    }
}

void Comm_RecvTransferModeRequest(int netId, int size, void *data, void *context) {
    const u8 *mode = data;
    if (Comm_GetLocalNetId() == 0) {
        gCommCore.work->transferModeHandshake = 1;
        gCommCore.work->requestedTransferMode = *mode;
    }
}

void Comm_RecvTransferModeChange(int netId, int size, void *data, void *context) {
    const u8 *mode = data;
    if (Comm_GetLocalNetId() != 0) {
        gCommCore.work->requestedTransferMode = *mode;
        gCommCore.work->transferModeHandshake = 3;
    }
}

void Comm_RecvTransferModeAck(int netId, int size, void *data, void *context) {
    const u8 *mode = data;
    if (Comm_GetLocalNetId() == 0 && gCommCore.work->transferModeHandshake == 2) {
        Comm_RequestTransferMode(*mode);
        gCommCore.work->transferModeHandshake = 0;
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
    if (gCommCore.work != NULL && gCommCore.work->linkError != 0) {
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

void Comm_RecvShutdown(int netId, int size, void *data, void *context) {
    u8 value;
    if (!CommWireless_IsShutdownRequested() && !Comm_GetLocalNetId()) {
        Comm_SendFixedParentCommand(2, &value);
    }
    CommWireless_RequestShutdown();
}

void CommRandom_Init(CommRandom *seed) {
    RTCDate date;
    RTCTime time;

    GF_RTC_CopyDateTime(&date, &time);
    seed->state = (time.second + gSystem.vblankCounter) + ((time.minute + ((time.hour + ((date.day + ((date.month + ((u64)date.year << 4)) << 5)) << 5)) << 6)) << 6);
    seed->multiplier = 0x5D588B656C078965ULL;
    seed->increment = 0x269EC3;
}

int Comm_ParentQueueHasCommand(int value) {
    return PacketPool_HasCommand(&gCommCore.work->parentQueue, value);
}

int Comm_IsParentQueueEmpty(void) {
    return PacketPool_AllNodesFree(&gCommCore.work->parentQueue);
}

int Comm_IsChildQueueEmpty(void) {
    return PacketPool_AllNodesFree(&gCommCore.work->childQueue);
}

void Comm_SetWifiConnected(int value) {
    gCommCore.work->wifiConnected = value;
}

int Comm_IsWifiConnected(void) {
    return gCommCore.work->wifiConnected;
}

void Comm_SetBattleSlot(int value, int slot) {
    if (gCommCore.work != NULL) {
        gCommCore.work->battleSlots[slot] = value;
    }
}

int Comm_GetBattleSlot(int slot) {
    if (gCommCore.work != NULL) {
        if (gCommCore.work->battleSlots[slot] != 0xFF) {
            return gCommCore.work->battleSlots[slot];
        }
    }
    return slot;
}

int Comm_IsWifiVoiceChatEnabled(void) {
    if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        return WifiConnection_IsVoiceChatEnabled();
    }
    return 0;
}

void Comm_SetWifiContinuousSend(int value) {
    int i;
    if (Comm_IsWifiLinkMode(CommState_GetLinkMode()) && gCommCore.work->continuousSend != value) {
        gCommCore.work->continuousSend = value;
        if (value != 0) {
            gCommCore.work->sentFrames = 0;
            for (i = 0; i < 8; i++) {
                gCommCore.work->childSentFrames[i] = 0;
            }
        }
    }
}

void Comm_SetWifiContinuousSendAndMuteVoice(int value) {
    Comm_SetWifiContinuousSend(value);
    if (Comm_IsWifiLinkMode(CommState_GetLinkMode())) {
        if (value != 0) {
            WifiConnection_SetVoiceUnmuted(0);
        } else {
            WifiConnection_SetVoiceUnmuted(1);
        }
    }
}

BOOL Comm_IsSendingMoveKeys(void) {
    return gCommCore.work->keySendTimer != 0;
}

void Comm_SetLinkError(void) {
    gCommCore.work->linkError = 1;
}

void Comm_StopFrameUpdate(void) {
    if (gCommCore.work != NULL) {
        gCommCore.work->updateStopped = 1;
    }
}
