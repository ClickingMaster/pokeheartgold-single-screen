#include "wifi_connection.h"

#include "global.h"

#include "heap.h"
#include "voice_chat.h"

typedef struct MatchParams {
    u32 a;
    u32 timeout;
} MatchParams;

extern const MatchParams _0221602C;
extern char gWifiGameName[];
extern char gWifiSecretKey[];
extern char gWifiLobbyKeyName[];
extern char gWifiMatchFilterFormat[];

extern int sprintf(char *dest, const char *format, ...);

extern void *FriendRoster_GetUserData(void *);
extern void *FriendRoster_GetDwcFriendData(void *, int);
extern void FriendRoster_MergeFriend(void *, int, int);
extern void *Save_FriendRoster_Get(void *);
extern void FrontierSave_ClearRow(void *, int, int);
extern void *Save_Frontier_GetStatic(void *);
extern int sub_02034084(void);
extern int sub_02034098(void);
extern int Comm_CountConnected(void);
extern void CommState_SetWifiError(int, u32, int);
extern int CommState_GetLinkMode(void);
extern void CommState_SetError(int);
extern int DWC_CheckHasProfile(void *);
extern int DWC_CheckValidConsole(void *);
extern int DWC_CheckDirtyFlag(void *);
extern void DWC_ClearDirtyFlag(void *);
extern void DWC_CreateExchangeToken(void *, void *);
extern int DWC_GetFriendDataType(void *);
extern int DWC_IsBuddyFriendData(void *);
extern u16 NNS_FndSetGroupIDForExpHeap(NNSFndHeapHandle heap, u16 groupID);
extern int NNS_FndGetGroupIDForMBlockExpHeap(void *);

extern int ov00_021EC11C(int *, u32 *);
extern void ov00_021EC210(void);
extern void ov00_021EC294(void *(*alloc)(int, u32, int), void (*free)(int, void *, u32));
extern void ov00_021EC3F0(void *, int, int, int);
extern void ov00_021EC454(int);
extern void ov00_021EC4A4(void);
extern int ov00_021EC5B4(void);
extern void ov00_021EC60C(void);
extern int ov00_021EC724(void);
extern void ov00_021EC8D8(void);
extern void ov00_021ED730(void *, void *, int, char *, char *, int, int, void *, int);
extern void ov00_021ED9B4(void);
extern void ov00_021EDB1C(void);
extern void ov00_021EDD6C(u16 *, int, void (*)(int, int, int), void *);
extern int ov00_021EDE74(int, void (*)(u32), void *, void (*)(void), int, void (*)(int, int), int);
extern void ov00_021EDF1C(u8, void *, void (*)(int, int), int, BOOL (*)(void), int);
extern int ov00_021EDFBC(u8, void (*)(int, int, int, int, int), void *, void (*)(int), void *);
extern int ov00_021EE054(int, void (*)(u32, u32, u32, u32, u32), void *, void (*)(int), void *);
extern void ov00_021EE24C(void (*)(int, int, int, int), void *);
extern void ov00_021EE278(void);
extern void ov00_021EE384(u32 *);
extern int ov00_021EE448(void);
extern int ov00_021EE490(void);
extern u32 ov00_021EE4FC(void);
extern int ov00_021EE530(u8);
extern void ov00_021EFE78(void);
extern int ov00_021EFDCC(void *, void *, int *);
extern void ov00_021EFF04(void (*)(void), int);
extern void ov00_021F1284(void);
extern void ov00_021F12F0(void (*)(void), int);
extern int ov00_021F13A4(int, const char *, const char *);
extern int ov00_021F14D4(int, const void *, u32);
extern int ov00_021F9580(u8);
extern u32 ov00_021F9750(u32, void *, u32);
extern void ov00_021F9850(u8, void *, int);
extern int ov00_021F9988(u8, int);
extern void ov00_021F989C(void (*)(u32, s32));
extern void ov00_021F98BC(void (*)(u16, const u8 *, int));
extern void ov00_021F98DC(void (*)(void));

#pragma force_active on

int WifiConnection_Init(void *saveData, int heapId, u32 heapSize, int maxPlayers) {
    u8 *raw;
    int i;

    GF_ASSERT(gWifiConnection == NULL);
    raw = Heap_Alloc((enum HeapID)heapId, sizeof(WifiConnection));
    MI_CpuFill8(raw, 0, sizeof(WifiConnection));
    gWifiConnection = (WifiConnection *)(((u32)raw + 31) & ~31);
    gWifiConnection->rawAlloc = raw;
    gWifiConnection->saveData = saveData;
    gWifiConnection->recvLocalCallback = NULL;
    gWifiConnection->recvCallback = NULL;
    gWifiConnection->errorCallback = NULL;
    gWifiConnection->state = WIFI_CONNECTION_STATE_INIT;
    gWifiConnection->heapId = heapId;
    gWifiConnection->heapId2 = heapId;
    gWifiConnection->heapSize = heapSize;
    gWifiConnection->heapMemory = Heap_Alloc((enum HeapID)heapId, heapSize + 0x3020);
    gWifiConnection->extraHeapMemory = NULL;
    gWifiConnection->heap = NNS_FndCreateExpHeapEx((void *)(((u32)gWifiConnection->heapMemory + 31) & ~31), heapSize, 0);
    gWifiConnection->extraHeap = NULL;
    gWifiConnection->voiceChatMode = 0;
    gWifiConnection->targetFriendIndex = -1;
    gWifiConnection->friendStatusIndex = 0;
    gWifiConnection->maxPlayers = maxPlayers;
    gWifiConnection->conferenceAidMask = 0;
    gWifiConnection->unk_10D8 = 0;
    gWifiConnection->unk_1098 = -1;
    gWifiConnection->unk_109C = 1;
    gWifiConnection->forceError = 0;
    gWifiConnection->retryCount = 0;
    gWifiConnection->sendSequence = 0;
    gWifiConnection->recvSequence = 0;
    gWifiConnection->unk_10D4 = 1;
    gWifiConnection->unk_10D0 = 1;
    gWifiConnection->unk_10D6 = 1;
    if (saveData != NULL) {
        gWifiConnection->userData = FriendRoster_GetUserData(Save_FriendRoster_Get(gWifiConnection->saveData));
        gWifiConnection->friendList = FriendRoster_GetDwcFriendData(Save_FriendRoster_Get(gWifiConnection->saveData), 0);
    }
    gWifiConnection->unk_10E5 = 1;
    gWifiConnection->friendStatusData = NULL;
    for (i = 0; i < 32; i++) {
        gWifiConnection->friendStatus[i] = 0;
    }
    ov00_021E700C();
    if (!DWC_CheckHasProfile(gWifiConnection->userData)) {
        return 1;
    }
    if (DWC_CheckValidConsole(gWifiConnection->userData)) {
        return 0;
    }
    return 2;
}

#pragma force_active reset

void WifiConnection_Destroy(void) {
    if (gWifiConnection != NULL) {
        WifiConnection_FreeRecvBuffers();
        if (gWifiConnection->extraHeapMemory != NULL) {
            NNS_FndDestroyExpHeap(gWifiConnection->extraHeap);
            Heap_Free(gWifiConnection->extraHeapMemory);
            gWifiConnection->extraHeapMemory = NULL;
        }
        NNS_FndDestroyExpHeap(gWifiConnection->heap);
        Heap_FreeExplicit((enum HeapID)gWifiConnection->heapId, gWifiConnection->heapMemory);
        Heap_FreeExplicit((enum HeapID)gWifiConnection->heapId, gWifiConnection->rawAlloc);
        gWifiConnection = NULL;
    }
    ov00_021EC210();
}

#pragma force_active on

int WifiConnection_UpdateLogin(void) {
    OSOwnerInfo owner;
    int err;

    switch ((u32)gWifiConnection->state) {
    case WIFI_CONNECTION_STATE_INIT:
        ov00_021EC294(WifiConnection_Alloc, WifiConnection_Free);
        ov00_021EC3F0(gWifiConnection->libConfig, 2, 1, 20);
        ov00_021EC454(2);
        ov00_021EC4A4();
        gWifiConnection->state = WIFI_CONNECTION_STATE_WAIT_LINK;
        gWifiConnection->voiceChatActive = 0;
    case WIFI_CONNECTION_STATE_WAIT_LINK:
        if (ov00_021EC5B4()) {
            if (ov00_021EC724() == 4) {
                gWifiConnection->state = WIFI_CONNECTION_STATE_LOGIN;
            } else {
                gWifiConnection->state = WIFI_CONNECTION_STATE_ERROR;
            }
        } else {
            ov00_021EC60C();
            break;
        }
    case WIFI_CONNECTION_STATE_LOGIN:
        err = WifiConnection_CheckError();
        if (err) {
            return err;
        }
        ov00_021ED730(gWifiConnection->loginControl, gWifiConnection->userData, 0x29E7, gWifiGameName, gWifiSecretKey, 0, 0, gWifiConnection->friendList, 32);
        OS_GetOwnerInfo(&owner);
        ov00_021EDD6C(owner.nickName, 0, WifiConnection_LoginCallback, NULL);
        gWifiConnection->state = WIFI_CONNECTION_STATE_LOGGING_IN;
    case WIFI_CONNECTION_STATE_LOGGING_IN:
        ov00_021EDB1C();
        break;
    case WIFI_CONNECTION_STATE_LOGGED_IN:
        ov00_021EDB1C();
        return 27;
    }
    return WifiConnection_CheckError();
}

#pragma force_active reset

void WifiConnection_SetReceiveCallbacks(WifiReceiveCallback recvLocal, WifiReceiveCallback recv) {
    gWifiConnection->recvLocalCallback = recvLocal;
    gWifiConnection->recvCallback = recv;
}

void WifiConnection_SetDisconnectCallback(void (*callback)(int, void *), void *arg) {
    gWifiConnection->disconnectCallback = callback;
    gWifiConnection->disconnectCallbackArg = arg;
}

void WifiConnection_SetConnectFilterCallback(BOOL (*callback)(int)) {
    gWifiConnection->connectFilterCallback = callback;
}

void ov00_021E5CD0(void (*callback)(int, void *), void *arg) {
    gWifiConnection->clientCallback = callback;
    gWifiConnection->clientCallbackArg = arg;
}

#pragma force_active on

int ov00_021E5CEC(const char *name, int maxPlayers, int filterLocal, int timeoutSeconds) {
    MatchParams params;

    GF_ASSERT(gWifiConnection);
    if (gWifiConnection->state != WIFI_CONNECTION_STATE_LOGGED_IN) {
        return 0;
    }
    WifiConnection_FreeRecvBuffers();
    params = _0221602C;
    params.timeout = 1000 * timeoutSeconds;
    GF_ASSERT(ov00_021F14D4(0, &params, 8) == 0);
    GF_ASSERT(ov00_021F13A4(0, gWifiLobbyKeyName, name) != 0);
    MI_CpuFill8(gWifiConnection->matchFilter, 0, 128);
    sprintf(gWifiConnection->matchFilter, gWifiMatchFilterFormat, gWifiLobbyKeyName, name);
    GF_ASSERT(strlen(gWifiConnection->matchFilter) < 128);
    if (filterLocal) {
        ov00_021F13A4(1, gWifiConnection->matchFilter, gWifiConnection->matchFilter);
    }
    gWifiConnection->state = WIFI_CONNECTION_STATE_MATCHING;
    gWifiConnection->maxPlayers = maxPlayers;
    ov00_021EDF1C((u8)maxPlayers, gWifiConnection->matchFilter, ov00_021E63CC, 0, ov00_021E6424, 0);
    gWifiConnection->matchType = 0;
    ov00_021F989C(WifiConnection_SendDoneCallback);
    ov00_021F98BC(WifiConnection_ReceiveCallback);
    ov00_021EE24C(WifiConnection_ConnectionClosedCallback, 0);
    ov00_021F98DC(WifiConnection_TimeoutCallback);
    gWifiConnection->sendBusy = 0;
    gWifiConnection->unk_10E5 = 1;
    return 1;
}

#pragma force_active reset

void ov00_021E5E34(void) {
    if (gWifiConnection->state == WIFI_CONNECTION_STATE_CANCEL_REQUESTED) {
        gWifiConnection->state = WIFI_CONNECTION_STATE_CANCELLED_BY_PEER;
    } else {
        gWifiConnection->state = WIFI_CONNECTION_STATE_CANCELLED;
    }
}

#pragma force_active on

int WifiConnection_Update(int success) {
    switch (gWifiConnection->state) {
    case WIFI_CONNECTION_STATE_MATCHING:
        if (success) {
            gWifiConnection->state = WIFI_CONNECTION_STATE_MATCHED;
        }
        if (gWifiConnection->matchType == 2 && gWifiConnection->targetFriendIndex >= 0 && gWifiConnection->friendStatus[gWifiConnection->targetFriendIndex] != 6) {
            gWifiConnection->state = WIFI_CONNECTION_STATE_CANCEL_REQUESTED;
        }
        break;
    case WIFI_CONNECTION_STATE_MATCHED:
    case WIFI_CONNECTION_STATE_CANCEL_REQUESTED:
        if (!gWifiConnection->matchType) {
            ov00_021F1284();
        } else {
            ov00_021EE278();
            ov00_021E5E34();
        }
        break;
    case WIFI_CONNECTION_STATE_CONNECTED:
        CommState_GetLinkMode();
        if (!sub_02034084()) {
            WifiConnection_StartVoiceChat(gWifiConnection->heapId);
        }
        {
            WifiConnection *connection = gWifiConnection;
            if (connection->unk_109C) {
                connection->unk_10D4 = 1;
            } else {
                connection->unk_10D4 = 0;
            }
        }
        gWifiConnection->state = WIFI_CONNECTION_STATE_CONNECTED_VOICE;
        return 20;
    case WIFI_CONNECTION_STATE_CANCELLED:
        gWifiConnection->state = WIFI_CONNECTION_STATE_LOGGED_IN;
        gWifiConnection->sendBusy = 0;
        gWifiConnection->unk_1098 = -1;
        return 21;
    case WIFI_CONNECTION_STATE_CANCELLED_BY_PEER:
        gWifiConnection->state = WIFI_CONNECTION_STATE_LOGGED_IN;
        gWifiConnection->sendBusy = 0;
        gWifiConnection->unk_1098 = -1;
        return 22;
    case WIFI_CONNECTION_STATE_ERROR:
        return WifiConnection_CheckError();
    case WIFI_CONNECTION_STATE_DISCONNECTING:
        if (!gWifiConnection->voiceChatActive) {
            ov00_021EE278();
            gWifiConnection->state = WIFI_CONNECTION_STATE_DISCONNECTED;
        }
        break;
    }
    return WifiConnection_UpdateConnected();
}

#pragma force_active reset

int WifiConnection_SendToHost(void *data, int size) {
    if (size >= 0x100) {
        return 0;
    }
    if (ov00_021EE490() == 0) {
        if (gWifiConnection->recvLocalCallback != NULL) {
            gWifiConnection->recvLocalCallback(0, data, (u16)size);
        }
        return 1;
    }
    if (gWifiConnection->sendBusy != 0 || WifiConnection_CanSendToAll() == 0) {
        return 0;
    }
    *(u32 *)gWifiConnection->sendBuffer = 1 | ((u32)gWifiConnection->unk_10D4 << 8);
    gWifiConnection->sendSequence++;
    gWifiConnection->sendBuffer[2] = gWifiConnection->sendSequence;
    MI_CpuCopy8(data, gWifiConnection->sendBuffer + 4, size);
    gWifiConnection->sendBusy = 1;
    ov00_021F9750(1, gWifiConnection, size + 4);
    return 1;
}

int WifiConnection_SendFromHost(void *data, int size) {
    if (size >= 0x100) {
        return 0;
    }
    if (ov00_021EE490() != 0) {
        return 0;
    }
    if (gWifiConnection->sendBusy != 0 || WifiConnection_CanSendToAll() == 0) {
        return 0;
    }
    *(u32 *)gWifiConnection->sendBuffer = 1 | ((u32)gWifiConnection->unk_10D4 << 8);
    gWifiConnection->sendSequence++;
    gWifiConnection->sendBuffer[2] = gWifiConnection->sendSequence;
    MI_CpuCopy8(data, gWifiConnection->sendBuffer + 4, size);
    gWifiConnection->sendBusy = 1;
    if (ov00_021F9750(ov00_021EE4FC(), gWifiConnection, size + 4) == 0) {
        gWifiConnection->sendBusy = 0;
        return 0;
    }
    if (gWifiConnection->recvCallback != NULL) {
        gWifiConnection->recvCallback(0, data, (u16)size);
    }
    return 1;
}

int WifiConnection_SendToAll(void *data, int size) {
    u16 aidMask;
    if (size >= 0x100) {
        return 0;
    }
    if (gWifiConnection->sendBusy != 0 || WifiConnection_CanSendToAll() == 0) {
        return 0;
    }
    *(u32 *)gWifiConnection->sendBuffer = 1 | ((u32)gWifiConnection->unk_10D4 << 8);
    gWifiConnection->sendSequence++;
    gWifiConnection->sendBuffer[2] = gWifiConnection->sendSequence;
    MI_CpuCopy8(data, gWifiConnection->sendBuffer + 4, size);
    gWifiConnection->sendBusy = 1;
    aidMask = ov00_021EE4FC();
    if (ov00_021F9750(aidMask, gWifiConnection, size + 4) != aidMask) {
        gWifiConnection->sendBusy = 0;
        return 0;
    }
    if (gWifiConnection->recvCallback != NULL) {
        gWifiConnection->recvCallback(ov00_021EE490(), data, (u16)size);
    }
    return 1;
}

void WifiConnection_LoginCallback(int result, int unused, int arg) {
    if (DWC_CheckDirtyFlag(gWifiConnection->userData)) {
        DWC_ClearDirtyFlag(gWifiConnection->userData);
        gWifiConnection->friendDataChanged = 1;
    }
    if (result == 0) {
        if (ov00_021EDE74(0, ov00_021E6274, gWifiConnection->userData, ov00_021E6298, arg, ov00_021E629C, arg) == 0) {
            CommState_SetError(1);
            return;
        }
        ov00_021EFF04(ov00_021E62D4, 0);
        return;
    }
    gWifiConnection->state = WIFI_CONNECTION_STATE_ERROR;
}

void WifiConnection_TimeoutCallback(void) {
    if (gWifiConnection->unk_10A8 != 0) {
        ov00_021EE278();
        gWifiConnection->unk_1098 = -1;
        gWifiConnection->state = WIFI_CONNECTION_STATE_TIMED_OUT;
    }
}

void ov00_021E6274(u32 result) {
    if (result == 0) {
        gWifiConnection->state = WIFI_CONNECTION_STATE_LOGGED_IN;
    } else {
        gWifiConnection->state = WIFI_CONNECTION_STATE_ERROR;
    }
}

void ov00_021E6298(void) {
}

#pragma force_active on

void ov00_021E629C(int index, int srcIndex) {
    FriendRoster_MergeFriend(Save_FriendRoster_Get(gWifiConnection->saveData), index, srcIndex);
    FrontierSave_ClearRow(Save_Frontier_GetStatic(gWifiConnection->saveData), index, srcIndex);
}

#pragma force_active reset

void ov00_021E62D4(void) {
}

void WifiConnection_ResetKeepAliveTimers(void) {
    MI_CpuFill8(gWifiConnection->keepAliveTimers, 0, sizeof(gWifiConnection->keepAliveTimers));
}

#pragma force_active on

void ov00_021E62F4(void) {
    int i;
    for (i = 0; i < 4; i++) {
        ov00_021F9988((u8)i, 0);
    }
    if (ov00_021EE490() == 0) {
        for (i = 0; i < gWifiConnection->maxPlayers; i++) {
            if (i != ov00_021EE490() && (ov00_021EE4FC() & (1 << i))) {
                if (!ov00_021F9988((u8)i, 10000)) {
                    GF_AssertFail();
                }
            }
        }
    } else {
        if (!ov00_021F9988(0, 10000)) {
            GF_AssertFail();
        }
    }
    gWifiConnection->unk_10A8 = 1;
    WifiConnection_ResetKeepAliveTimers();
}

void ov00_021E6388(int aid) {
    int i;
    gWifiConnection->state = WIFI_CONNECTION_STATE_CONNECTED;
    for (i = 0; i < gWifiConnection->maxPlayers; i++) {
        if (i != ov00_021EE490()) {
            WifiConnection_AllocRecvBuffer(i);
        }
    }
    ov00_021E62F4();
}

void ov00_021E63CC(int result, int cancelled) {
    WifiConnection *connection;
    if (result == 0) {
        if (cancelled == 0) {
            ov00_021E6388(1 - ov00_021EE490());
        } else {
            ov00_021E5E34();
        }
    } else {
        gWifiConnection->state = WIFI_CONNECTION_STATE_ERROR;
    }
    connection = gWifiConnection;
    if (connection->clientCallback) {
        int aid = ov00_021EE490();
        gWifiConnection->clientCallback(aid, connection->clientCallbackArg);
    }
}

#pragma force_active reset

BOOL ov00_021E6424(void) {
    return TRUE;
}

void WifiConnection_SendDoneCallback(u32 result, s32 aid) {
    gWifiConnection->sendBusy = 0;
    gWifiConnection->keepAliveTimers[aid] = 0;
}

void ov00_021E644C(u32 header) {
    CommState_GetLinkMode();
    if (sub_02034084() == 0) {
        if (header & 0x100) {
            gWifiConnection->unk_10D0 = 1;
        } else {
            gWifiConnection->unk_10D0 = 0;
        }
    }
}

#pragma force_active on

void WifiConnection_ReceiveCallback(u16 aid, const u8 *data, int size) {
    void *buf;
    u32 header;

    header = data[0] | ((data[1] << 8) | ((data[3] << 24) | (data[2] << 16)));
    gWifiConnection->unk_10A8 = 1;
    if ((u8)header == 1) {
        ov00_021E644C(header);
        gWifiConnection->recvSequence = data[2];
    } else {
        if (VoiceChat_ReceiveData(aid, (void *)data, (void *)size) == 0) {
            ov00_021E644C(header);
        }
        return;
    }
    size -= 4;
    buf = WifiConnection_Alloc(0, size, 4);
    if (buf != NULL) {
        MI_CpuCopy8(data + 4, buf, size);
        if (ov00_021EE490() == 0) {
            if (gWifiConnection->recvLocalCallback != NULL) {
                gWifiConnection->recvLocalCallback(aid, buf, size);
            }
        } else {
            if (gWifiConnection->recvCallback != NULL) {
                gWifiConnection->recvCallback(aid, buf, size);
            }
        }
        WifiConnection_Free(0, buf, size);
    }
}

#pragma force_active reset

void WifiConnection_ConnectionClosedCallback(int result, int unused1, int unused2, int arg) {
    gWifiConnection->sendBusy = 0;
    gWifiConnection->retryCount = 0;
    gWifiConnection->unk_10A0 = 0;
    if (result == 0 && gWifiConnection->unk_10E5 == 1 && ov00_021EE448() == 1) {
        if (gWifiConnection->state != WIFI_CONNECTION_STATE_MATCHED) {
            gWifiConnection->state = WIFI_CONNECTION_STATE_DISCONNECTING;
        }
        if (gWifiConnection->voiceChatActive != 0) {
            WifiConnection_StopVoiceChat();
        }
    }
    if (gWifiConnection->disconnectCallback != NULL) {
        gWifiConnection->disconnectCallback(arg, gWifiConnection->disconnectCallbackArg);
    }
}

void *WifiConnection_Alloc(int unused, u32 size, int alignment) {
    OSIntrMode intr = OS_DisableInterrupts();
    void *result = NNS_FndAllocFromExpHeapEx(gWifiConnection->heap, size, alignment);
    if (result == NULL && gWifiConnection->extraHeapMemory != NULL) {
        result = NNS_FndAllocFromExpHeapEx(gWifiConnection->extraHeap, size, alignment);
    }
    if (result == NULL) {
        CommState_SetError(1);
        OS_RestoreInterrupts(intr);
        return NULL;
    }
    OS_RestoreInterrupts(intr);
    return result;
}

void WifiConnection_Free(int unused, void *ptr, u32 size) {
    if (ptr != NULL) {
        OSIntrMode intr = OS_DisableInterrupts();
        if (NNS_FndGetGroupIDForMBlockExpHeap(ptr) == 0x10) {
            if (gWifiConnection->extraHeapMemory == NULL) {
                CommState_SetError(1);
                return;
            }
            NNS_FndFreeToExpHeap(gWifiConnection->extraHeap, ptr);
        } else {
            NNS_FndFreeToExpHeap(gWifiConnection->heap, ptr);
        }
        OS_RestoreInterrupts(intr);
    }
}

#pragma force_active on

int WifiConnection_CheckError(void) {
    int error;
    u32 type;
    int status;
    int result = 0;

    status = ov00_021EC11C(&error, &type);
    if (status) {
        result = error;
        if (error == 0 || type == 1) {
            result = status;
        }
        switch (type) {
        case 1:
            ov00_021EC210();
            break;
        case 2:
            ov00_021EC210();
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            if (gWifiConnection) {
                switch ((u32)gWifiConnection->state) {
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 12:
                case 16:
                case 17:
                case 18:
                    ov00_021ED9B4();
                case 0:
                case 1:
                case 2:
                    if (CommState_GetLinkMode() != 33) {
                        ov00_021EC8D8();
                    }
                    break;
                }
                ov00_021EC210();
            }
            if (gWifiConnection) {
                gWifiConnection->state = WIFI_CONNECTION_STATE_FATAL_ERROR;
            }
            break;
        case 7:
            if (gWifiConnection) {
                gWifiConnection->state = WIFI_CONNECTION_STATE_ERROR_CALLBACK;
                if (gWifiConnection->errorCallback) {
                    gWifiConnection->errorCallback(-error);
                }
            }
            break;
        }
    }
    if (gWifiConnection->forceError) {
        result = 26;
    }
    if (status) {
        CommState_SetWifiError(error, type, status);
    }
    return result;
}

#pragma force_active reset

int WifiConnection_CanSendToAll(void) {
    int i;
    int found = 0;
    i = found;
    if (gWifiConnection->maxPlayers > 0) {
        do {
            if (i != ov00_021EE490() && ov00_021EE530((u8)i) != 0) {
                found = 1;
                if (ov00_021F9580((u8)i) == 0) {
                    return 0;
                }
            }
            i++;
        } while (i < gWifiConnection->maxPlayers);
    }
    return found;
}

int WifiConnection_SendKeepAlive(int aid) {
    if (gWifiConnection->sendBusy == 0 && WifiConnection_CanSendToAll() != 0) {
        u32 mask = (u32)ov00_021EE4FC();
        if ((mask & 0xFFFE) != 0) {
            gWifiConnection->sendBusy = 1;
            *(u32 *)gWifiConnection->sendBuffer = 2 | ((u32)gWifiConnection->unk_10D4 << 8);
            ov00_021F9750(ov00_021EE4FC(), gWifiConnection, 4);
            gWifiConnection->keepAliveTimers[aid] = 0;
            return 1;
        }
    }
    return 0;
}

#pragma force_active on

int WifiConnection_UpdateConnected(void) {
    s32 state;

    ov00_021EDB1C();
    WifiConnection_UpdateFriendStatus();
    if (gWifiConnection->voiceChatActive) {
        if (gWifiConnection->unk_10D4 == 1 && gWifiConnection->unk_10D0 == 1 && gWifiConnection->unk_10D6 == 1) {
            VoiceChat_Unmute();
        } else {
            VoiceChat_Mute();
        }
        VoiceChat_Update();
        {
            WifiConnection *connection = gWifiConnection;
            if (connection->conferenceAidMask != ov00_021EE4FC() && !connection->unk_10DF && connection->unk_109C) {
                s32 aidMask = ov00_021EE4FC();
                s32 ownAid = ov00_021EE490();
                if (VoiceChat_AddConferenceClients(aidMask, ownAid)) {
                    gWifiConnection->conferenceAidMask = ov00_021EE4FC();
                }
            }
        }
    }
    state = gWifiConnection->state;
    if (state == WIFI_CONNECTION_STATE_TIMED_OUT) {
        s32 result = WifiConnection_CheckError();
        if (!result) {
            return 23;
        }
        return result;
    }
    if (state == WIFI_CONNECTION_STATE_DISCONNECTED) {
        return 24;
    }
    if ((u32)state - WIFI_CONNECTION_STATE_CONNECTED <= 1) {
        s32 i;
        for (i = 0; i < gWifiConnection->maxPlayers; i++) {
            // byte-offset form kept: indexing keepAliveTimers[i] directly compiles differently
            u8 *timers = (u8 *)gWifiConnection + i * 4;
            u32 next = *(u32 *)(timers + offsetof(WifiConnection, keepAliveTimers));
            s32 old = *(volatile s32 *)(timers + offsetof(WifiConnection, keepAliveTimers));
            *(u32 *)(timers + offsetof(WifiConnection, keepAliveTimers)) = next + 1;
            if (old >= 120 && !gWifiConnection->sendBusy && WifiConnection_SendKeepAlive(i)) {
                WifiConnection_ResetKeepAliveTimers();
                break;
            }
        }
    }
    return WifiConnection_CheckError();
}

#pragma force_active reset

int WifiConnection_GetAid(void) {
    if (gWifiConnection != NULL && (gWifiConnection->state == WIFI_CONNECTION_STATE_CONNECTED || gWifiConnection->state == WIFI_CONNECTION_STATE_CONNECTED_VOICE || gWifiConnection->state == WIFI_CONNECTION_STATE_DISCONNECTING)) {
        return ov00_021EE490();
    }
    return -1;
}

void WifiConnection_ClearVoiceChatActive(void) {
    gWifiConnection->voiceChatActive = 0;
}

#pragma force_active on

void WifiConnection_StartVoiceChat(int heapId) {
    int count = 1;
    int online;
    int codec;

    CommState_GetLinkMode();
    online = sub_02034084();
    if (online) {
        count = Comm_CountConnected() - 1;
        if (count < 1) {
            count = 1;
        }
        gWifiConnection->unk_10D4 = 1;
        gWifiConnection->unk_10D0 = 1;
        gWifiConnection->unk_10D6 = 1;
    }
    if (gWifiConnection->voiceChatActive == 0) {
        switch (gWifiConnection->voiceChatMode) {
        case 2:
            codec = 1;
            break;
        case 3:
            codec = 2;
            break;
        case 4:
            codec = 3;
            break;
        case 5:
            codec = 4;
            break;
        case 0:
        case 1:
        default:
            if (online == 0) {
                codec = 4;
            } else {
                codec = 3;
            }
            break;
        }
        VoiceChat_Init(heapId, codec, count);
        VoiceChat_SetEndCallback(WifiConnection_ClearVoiceChatActive);
        gWifiConnection->voiceChatActive = 1;
    }
}

#pragma force_active reset

void WifiConnection_StopVoiceChat(void) {
    VoiceChat_Shutdown();
    if (gWifiConnection != NULL) {
        gWifiConnection->voiceChatActive = 0;
        gWifiConnection->conferenceAidMask = 0;
    }
}

int WifiConnection_ClassifyErrorCode(int errorCode, u32 context) {
    s32 hundreds = errorCode / 100;
    s32 thousands = errorCode / 1000;

    if (errorCode == 20101) {
        return 1;
    }
    if (thousands == 23) {
        return 1;
    }
    if (errorCode == 20108) {
        return 2;
    }
    if (errorCode == 20110) {
        return 3;
    }
    if (hundreds == 512) {
        return 4;
    }
    if (hundreds == 500) {
        return 5;
    }
    if (errorCode == 51103) {
        return 7;
    }
    if (hundreds == 510) {
        return 6;
    }
    if (hundreds == 511) {
        return 6;
    }
    if (hundreds == 513) {
        return 6;
    }
    if (errorCode >= 52000 && errorCode <= 52003) {
        return 8;
    }
    if (errorCode >= 52100 && errorCode <= 52103) {
        return 8;
    }
    if (errorCode >= 52200 && errorCode <= 52203) {
        return 8;
    }
    if (errorCode == 80430) {
        return 9;
    }
    if (thousands == 20) {
        return 0;
    }
    if (hundreds == 520) {
        return 0;
    }
    if (hundreds == 521) {
        return 0;
    }
    if (hundreds == 522) {
        return 0;
    }
    if (hundreds == 523) {
        return 0;
    }
    if (hundreds == 530) {
        return 0;
    }
    if (hundreds == 531) {
        return 0;
    }
    if (hundreds == 532) {
        return 0;
    }
    if (errorCode < 10000) {
        return 14;
    }
    if (thousands == 31) {
        return 12;
    }
    switch (context) {
    case 0:
    case 1:
    case 2:
        return 11;
    case 3:
    case 4:
    case 5:
    case 6:
        return 10;
    default:
        GF_AssertFail();
        break;
    case 7:
        return 15;
    }
    return -1;
}

int WifiConnection_Disconnect(int checkOnly) {
    if (checkOnly == 0) {
        switch (gWifiConnection->state) {
        case WIFI_CONNECTION_STATE_MATCHING:
        case WIFI_CONNECTION_STATE_CONNECTED:
        case WIFI_CONNECTION_STATE_CONNECTED_VOICE:
            if (gWifiConnection->voiceChatActive != 0) {
                VoiceChat_HangUp(gWifiConnection->voiceChatActive);
            }
            gWifiConnection->state = WIFI_CONNECTION_STATE_DISCONNECTING;
            break;
        case WIFI_CONNECTION_STATE_LOGGED_IN:
        case WIFI_CONNECTION_STATE_FATAL_ERROR:
        case WIFI_CONNECTION_STATE_DISCONNECTED:
        case WIFI_CONNECTION_STATE_TIMED_OUT:
            return 1;
        }
    } else {
        switch (gWifiConnection->state) {
        case WIFI_CONNECTION_STATE_LOGGED_IN:
        case WIFI_CONNECTION_STATE_DISCONNECTED:
        case WIFI_CONNECTION_STATE_TIMED_OUT:
            return 1;
        }
    }
    return 0;
}

int WifiConnection_FinishDisconnect(void) {
    if (gWifiConnection->state == WIFI_CONNECTION_STATE_DISCONNECTED || gWifiConnection->state == WIFI_CONNECTION_STATE_TIMED_OUT || gWifiConnection->state == WIFI_CONNECTION_STATE_LOGGED_IN) {
        gWifiConnection->state = WIFI_CONNECTION_STATE_LOGGED_IN;
        gWifiConnection->unk_1098 = -1;
        ov00_021E7130();
        return 1;
    }
    return 0;
}

void WifiConnection_SetErrorCallback(void (*callback)(int)) {
    if (gWifiConnection != NULL) {
        gWifiConnection->errorCallback = callback;
    }
}

void WifiConnection_Shutdown(void) {
    ov00_021ED9B4();
    ov00_021EC8D8();
    WifiConnection_StopVoiceChat();
    WifiConnection_Destroy();
}

void WifiConnection_SetFriendStatusBuffer(u8 *data, int stride) {
    gWifiConnection->friendStatusData = data;
    gWifiConnection->friendStatusStride = stride;
}

#pragma force_active on

void WifiConnection_UpdateFriendStatus(void) {
    int result;
    if (gWifiConnection->friendStatusData) {
        int n = gWifiConnection->friendStatusIndex & 31;
        if (DWC_IsBuddyFriendData(&gWifiConnection->friendList[n])) {
            gWifiConnection->friendStatus[n] = ov00_021EFDCC(&gWifiConnection->friendList[n], gWifiConnection->friendStatusData + gWifiConnection->friendStatusStride * n, &result);
        }
        gWifiConnection->friendStatusIndex = (gWifiConnection->friendStatusIndex + 1) & 31;
    }
}

#pragma force_active reset

void ov00_021E6D60(void *data, int size) {
    ov00_021EFE78();
}

u8 WifiConnection_GetFriendStatus(int index) {
    return gWifiConnection->friendStatus[index];
}

#pragma force_active on

int WifiConnection_StartMatchmaking(int friendIndex, int maxPlayers, BOOL alternate) {
    int result;

    if (ov00_021E7314()) {
        return -4;
    }
    if (gWifiConnection->state != WIFI_CONNECTION_STATE_LOGGED_IN) {
        gWifiConnection->retryCount++;
        if (gWifiConnection->retryCount > 120) {
            return -3;
        }
        return -1;
    }
    WifiConnection_FreeRecvBuffers();
    gWifiConnection->unk_10D8 = 0;
    gWifiConnection->unk_10E5 = 1;
    gWifiConnection->targetFriendIndex = friendIndex;
    gWifiConnection->maxPlayers = maxPlayers;
    if (alternate) {
        maxPlayers = 2;
    }
    gWifiConnection->unk_10A0 = 1;
    if (friendIndex < 0) {
        result = ov00_021EDFBC(maxPlayers, ov00_021E6ED8, NULL, ov00_021E6FE0, NULL);
        gWifiConnection->matchType = 1;
    } else {
        result = ov00_021EE054(friendIndex, ov00_021E6FBC, NULL, ov00_021E6FE0, NULL);
        gWifiConnection->matchType = 2;
    }
    if (!result) {
        gWifiConnection->retryCount++;
        if (gWifiConnection->retryCount > 120) {
            return -3;
        }
        return -2;
    }
    gWifiConnection->retryCount = 0;
    gWifiConnection->state = WIFI_CONNECTION_STATE_MATCHING;
    ov00_021F989C(WifiConnection_SendDoneCallback);
    ov00_021F98BC(WifiConnection_ReceiveCallback);
    ov00_021EE24C(WifiConnection_ConnectionClosedCallback, 0);
    ov00_021F98DC(WifiConnection_TimeoutCallback);
    gWifiConnection->sendBusy = 0;
    return 0;
}

#pragma force_active reset

int WifiConnection_GetTargetFriendIndex(void) {
    if (gWifiConnection != NULL) {
        return gWifiConnection->targetFriendIndex;
    }
    return -1;
}

#pragma force_active on

void ov00_021E6ED8(int result, int cancelled, int cancelledByMe, int unused, int aid) {
    int reject = 0;
    gWifiConnection->unk_10A0 = 0;
    if (result == 0) {
        if (cancelled == 0) {
            u32 mask;
            u32 current;
            CommState_GetLinkMode();
            if (sub_02034098() && aid == -1) {
                reject = 1;
            }
            if (gWifiConnection->connectFilterCallback && gWifiConnection->connectFilterCallback(aid) == 0) {
                reject = 1;
            }
            if (gWifiConnection->unk_10E0 || reject) {
                mask = ov00_021EE4FC() & ~gWifiConnection->unk_10D8;
                current = ov00_021EE4FC();
                if (mask) {
                    ov00_021EE384(&mask);
                    if ((mask ^ current) == 1) {
                        gWifiConnection->state = WIFI_CONNECTION_STATE_MATCHED;
                    }
                    return;
                }
            }
            gWifiConnection->targetFriendIndex = aid;
            gWifiConnection->unk_10D8 = ov00_021EE4FC();
            if (gWifiConnection->unk_10D8 == 1) {
                gWifiConnection->state = WIFI_CONNECTION_STATE_MATCHED;
                return;
            }
            ov00_021E6388(aid);
        } else if (cancelledByMe == 0) {
            gWifiConnection->unk_1098 = -1;
        }
    }
}

#pragma force_active reset

void ov00_021E6FBC(u32 result, u32 cancelled, u32 a2, u32 a3, u32 aid) {
    gWifiConnection->unk_10A0 = 0;
    if (result == 0 && cancelled == 0) {
        ov00_021E6388(aid);
    }
}

#pragma force_active on

void ov00_021E6FE0(int index) {
    void (*callback)(int, void *);
    gWifiConnection->unk_1098 = index;
    callback = gWifiConnection->clientCallback;
    if (callback) {
        callback((u16)index, gWifiConnection->clientCallbackArg);
    }
}

#pragma force_active reset

void ov00_021E700C(void) {
    u8 profileToken[12];
    u8 newToken[12];
    int i;
    int offset;

    if (!DWC_CheckHasProfile(gWifiConnection->userData)) {
        DWC_CreateExchangeToken(gWifiConnection->userData, profileToken);
    } else {
        DWC_CreateExchangeToken(gWifiConnection->userData, newToken);
    }
    for (i = 0, offset = 0; i < 32; i++, offset += 12) {
        DWC_GetFriendDataType((u8 *)gWifiConnection->friendList + offset);
        DWC_IsBuddyFriendData((u8 *)gWifiConnection->friendList + offset);
    }
}

BOOL WifiConnection_GetVoiceActivity(void) {
    return VoiceChat_GetVoiceActivity();
}

int ov00_021E7080(void) {
    if (gWifiConnection != NULL) {
        return gWifiConnection->unk_109C;
    }
    return 0;
}

int ov00_021E709C(void) {
    if (gWifiConnection != NULL) {
        return gWifiConnection->unk_1098;
    }
    return 0;
}

void ov00_021E70B8(int value) {
    gWifiConnection->unk_109C = value;
}

void ov00_021E70CC(u16 value) {
    gWifiConnection->unk_10D6 = value;
}

void ov00_021E70E0(void) {
    gWifiConnection->unk_10E0 = 2;
}

#pragma force_active on

BOOL ov00_021E70F4(void) {
    if (gWifiConnection->unk_10E0 == 0) {
        gWifiConnection->unk_10E0 = 1;
        ov00_021F12F0(ov00_021E70E0, 0);
    }
    if (gWifiConnection->unk_10E0 == 2) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

void ov00_021E7130(void) {
    gWifiConnection->unk_10E0 = 0;
}

#pragma force_active on

void ov00_021E7144(void) {
    ov00_021EE448();
}

void WifiConnection_SetExtraHeapEnabled(BOOL enable, int heapId) {
    WifiConnection *connection;
    if (enable) {
        gWifiConnection->heapId2 = 11;
        if (gWifiConnection->extraHeapMemory == NULL) {
            gWifiConnection->extraHeapMemory = Heap_Alloc((enum HeapID)heapId, 0xF020);
            gWifiConnection->extraHeap = NNS_FndCreateExpHeapEx((void *)(((u32)gWifiConnection->extraHeapMemory + 31) & ~31), 0xF000, 0);
            NNS_FndSetGroupIDForExpHeap(gWifiConnection->extraHeap, 16);
            gWifiConnection->extraHeapFreeSize = NNS_FndGetTotalFreeSizeForExpHeap(gWifiConnection->extraHeap);
        }
    } else {
        gWifiConnection->heapId2 = gWifiConnection->heapId;
        connection = gWifiConnection;
        if (connection->extraHeapMemory) {
            if (connection->extraHeapFreeSize != NNS_FndGetTotalFreeSizeForExpHeap(connection->extraHeap)) {
                CommState_SetError(1);
                return;
            }
            NNS_FndDestroyExpHeap(connection->extraHeap);
            Heap_Free(gWifiConnection->extraHeapMemory);
            gWifiConnection->extraHeapMemory = NULL;
        }
    }
}

#pragma force_active reset

void ov00_021E7220(u8 value) {
    gWifiConnection->unk_10E5 = value;
}

#pragma force_active on

void WifiConnection_FreeRecvBuffer(int aid) {
    void *buffer = gWifiConnection->recvBuffers[aid];
    if (buffer) {
        if (gWifiConnection->recvBufferFromGameHeap[aid] == 1) {
            Heap_Free(buffer);
        } else {
            WifiConnection_Free(0, buffer, 0);
        }
        gWifiConnection->recvBuffers[aid] = NULL;
    }
}

void WifiConnection_AllocRecvBuffer(int aid) {
    WifiConnection_FreeRecvBuffer(aid);
    if (gWifiConnection->recvBuffers[aid] == NULL) {
        if (gWifiConnection->heapId2 == 11) {
            gWifiConnection->recvBuffers[aid] = Heap_Alloc((enum HeapID)gWifiConnection->heapId2, 0x1020);
            gWifiConnection->recvBufferFromGameHeap[aid] = 1;
        } else {
            gWifiConnection->recvBuffers[aid] = WifiConnection_Alloc(0, 0x1000, 32);
            gWifiConnection->recvBufferFromGameHeap[aid] = 0;
        }
        ov00_021F9850((u8)aid, (void *)(((u32)gWifiConnection->recvBuffers[aid] + 31) & ~31), 0x1000);
    }
}

#pragma force_active reset

void WifiConnection_FreeRecvBuffers(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        WifiConnection_FreeRecvBuffer(i);
    }
}

u8 ov00_021E7314(void) {
    return gWifiConnection->friendDataChanged;
}

void ov00_021E7328(void) {
    gWifiConnection->friendDataChanged = 0;
}
