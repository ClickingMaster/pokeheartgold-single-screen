#include "wifi_connection.h"

#include "global.h"

#include "heap.h"
#include "voice_chat.h"

typedef struct MatchParams {
    u32 a;
    u32 timeout;
} MatchParams;

extern const MatchParams sWifiConnectionAnybodyMatchParams;
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
extern int Comm_IsWifiPeerLinkMode(void);
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

extern int DWC_GetLastErrorEx(int *, u32 *);
extern void DWC_ClearError(void);
extern void DWC_SetMemFunc(void *(*alloc)(int, u32, int), void (*free)(int, void *, u32));
extern void DWC_InitInetEx(void *, int, int, int);
extern void DWC_SetAuthServer(int);
extern void DWC_ConnectInetAsync(void);
extern int DWC_CheckInet(void);
extern void DWC_ProcessInet(void);
extern int DWC_GetInetStatus(void);
extern void DWC_CleanupInet(void);
extern void DWC_InitFriendsMatch(void *, void *, int, char *, char *, int, int, void *, int);
extern void DWC_ShutdownFriendsMatch(void);
extern void DWC_ProcessFriendsMatch(void);
extern void DWC_LoginAsync(u16 *, int, void (*)(int, int, int), void *);
extern int DWC_UpdateServersAsync(int, void (*)(u32), void *, void (*)(void), int, void (*)(int, int), int);
extern void DWC_ConnectToAnybodyAsync(u8, void *, void (*)(int, int), int, BOOL (*)(void), int);
extern int DWC_SetupGameServer(u8, void (*)(int, int, int, int, int), void *, void (*)(int), void *);
extern int DWC_ConnectToGameServerAsync(int, void (*)(u32, u32, u32, u32, u32), void *, void (*)(int), void *);
extern void DWC_SetConnectionClosedCallback(void (*)(int, int, int, int), void *);
extern void DWC_CloseAllConnectionsHard(void);
extern void DWC_CloseConnectionHardBitmap(u32 *);
extern int DWC_GetNumConnectionHost(void);
extern int DWC_GetMyAID(void);
extern u32 DWC_GetAIDBitmap(void);
extern int DWC_IsValidAID(u8);
extern void DWC_SetOwnStatusData(void);
extern int DWC_GetFriendStatusData(void *, void *, int *);
extern void DWC_SetBuddyFriendCallback(void (*)(void), int);
extern void DWC_CancelMatching(void);
extern void DWC_StopSCMatchingAsync(void (*)(void), int);
extern int DWC_AddMatchKeyString(int, const char *, const char *);
extern int DWC_SetMatchingOption(int, const void *, u32);
extern int DWC_IsSendableReliable(u8);
extern u32 DWC_SendReliableBitmap(u32, void *, u32);
extern void DWC_SetRecvBuffer(u8, void *, int);
extern int DWC_SetRecvTimeoutTime(u8, int);
extern void DWC_SetUserSendCallback(void (*)(u32, s32));
extern void DWC_SetUserRecvCallback(void (*)(u16, const u8 *, int));
extern void DWC_SetUserRecvTimeoutCallback(void (*)(void));

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
    gWifiConnection->matchedAidBitmap = 0;
    gWifiConnection->newClientIndex = -1;
    gWifiConnection->voiceChatEnabled = 1;
    gWifiConnection->forceError = 0;
    gWifiConnection->retryCount = 0;
    gWifiConnection->sendSequence = 0;
    gWifiConnection->recvSequence = 0;
    gWifiConnection->ownVoiceFlag = 1;
    gWifiConnection->peerVoiceFlag = 1;
    gWifiConnection->voiceUnmuted = 1;
    if (saveData != NULL) {
        gWifiConnection->userData = FriendRoster_GetUserData(Save_FriendRoster_Get(gWifiConnection->saveData));
        gWifiConnection->friendList = FriendRoster_GetDwcFriendData(Save_FriendRoster_Get(gWifiConnection->saveData), 0);
    }
    gWifiConnection->disconnectWhenAlone = 1;
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
    DWC_ClearError();
}

#pragma force_active on

int WifiConnection_UpdateLogin(void) {
    OSOwnerInfo owner;
    int err;

    switch ((u32)gWifiConnection->state) {
    case WIFI_CONNECTION_STATE_INIT:
        DWC_SetMemFunc(WifiConnection_Alloc, WifiConnection_Free);
        DWC_InitInetEx(gWifiConnection->libConfig, 2, 1, 20);
        DWC_SetAuthServer(2);
        DWC_ConnectInetAsync();
        gWifiConnection->state = WIFI_CONNECTION_STATE_WAIT_LINK;
        gWifiConnection->voiceChatActive = 0;
    case WIFI_CONNECTION_STATE_WAIT_LINK:
        if (DWC_CheckInet()) {
            if (DWC_GetInetStatus() == 4) {
                gWifiConnection->state = WIFI_CONNECTION_STATE_LOGIN;
            } else {
                gWifiConnection->state = WIFI_CONNECTION_STATE_ERROR;
            }
        } else {
            DWC_ProcessInet();
            break;
        }
    case WIFI_CONNECTION_STATE_LOGIN:
        err = WifiConnection_CheckError();
        if (err) {
            return err;
        }
        DWC_InitFriendsMatch(gWifiConnection->loginControl, gWifiConnection->userData, 0x29E7, gWifiGameName, gWifiSecretKey, 0, 0, gWifiConnection->friendList, 32);
        OS_GetOwnerInfo(&owner);
        DWC_LoginAsync(owner.nickName, 0, WifiConnection_LoginCallback, NULL);
        gWifiConnection->state = WIFI_CONNECTION_STATE_LOGGING_IN;
    case WIFI_CONNECTION_STATE_LOGGING_IN:
        DWC_ProcessFriendsMatch();
        break;
    case WIFI_CONNECTION_STATE_LOGGED_IN:
        DWC_ProcessFriendsMatch();
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

void WifiConnection_SetClientCallback(void (*callback)(int, void *), void *arg) {
    gWifiConnection->clientCallback = callback;
    gWifiConnection->clientCallbackArg = arg;
}

#pragma force_active on

int WifiConnection_StartAnybodyMatchmaking(const char *name, int maxPlayers, int filterLocal, int timeoutSeconds) {
    MatchParams params;

    GF_ASSERT(gWifiConnection);
    if (gWifiConnection->state != WIFI_CONNECTION_STATE_LOGGED_IN) {
        return 0;
    }
    WifiConnection_FreeRecvBuffers();
    params = sWifiConnectionAnybodyMatchParams;
    params.timeout = 1000 * timeoutSeconds;
    GF_ASSERT(DWC_SetMatchingOption(0, &params, 8) == 0);
    GF_ASSERT(DWC_AddMatchKeyString(0, gWifiLobbyKeyName, name) != 0);
    MI_CpuFill8(gWifiConnection->matchFilter, 0, 128);
    sprintf(gWifiConnection->matchFilter, gWifiMatchFilterFormat, gWifiLobbyKeyName, name);
    GF_ASSERT(strlen(gWifiConnection->matchFilter) < 128);
    if (filterLocal) {
        DWC_AddMatchKeyString(1, gWifiConnection->matchFilter, gWifiConnection->matchFilter);
    }
    gWifiConnection->state = WIFI_CONNECTION_STATE_MATCHING;
    gWifiConnection->maxPlayers = maxPlayers;
    DWC_ConnectToAnybodyAsync((u8)maxPlayers, gWifiConnection->matchFilter, WifiConnection_AnybodyMatchedCallback, 0, WifiConnection_EvalPlayerCallback, 0);
    gWifiConnection->matchType = 0;
    DWC_SetUserSendCallback(WifiConnection_SendDoneCallback);
    DWC_SetUserRecvCallback(WifiConnection_ReceiveCallback);
    DWC_SetConnectionClosedCallback(WifiConnection_ConnectionClosedCallback, 0);
    DWC_SetUserRecvTimeoutCallback(WifiConnection_TimeoutCallback);
    gWifiConnection->sendBusy = 0;
    gWifiConnection->disconnectWhenAlone = 1;
    return 1;
}

#pragma force_active reset

void WifiConnection_FinishCancel(void) {
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
            DWC_CancelMatching();
        } else {
            DWC_CloseAllConnectionsHard();
            WifiConnection_FinishCancel();
        }
        break;
    case WIFI_CONNECTION_STATE_CONNECTED:
        CommState_GetLinkMode();
        if (!Comm_IsWifiPeerLinkMode()) {
            WifiConnection_StartVoiceChat(gWifiConnection->heapId);
        }
        {
            WifiConnection *connection = gWifiConnection;
            if (connection->voiceChatEnabled) {
                connection->ownVoiceFlag = 1;
            } else {
                connection->ownVoiceFlag = 0;
            }
        }
        gWifiConnection->state = WIFI_CONNECTION_STATE_CONNECTED_VOICE;
        return 20;
    case WIFI_CONNECTION_STATE_CANCELLED:
        gWifiConnection->state = WIFI_CONNECTION_STATE_LOGGED_IN;
        gWifiConnection->sendBusy = 0;
        gWifiConnection->newClientIndex = -1;
        return 21;
    case WIFI_CONNECTION_STATE_CANCELLED_BY_PEER:
        gWifiConnection->state = WIFI_CONNECTION_STATE_LOGGED_IN;
        gWifiConnection->sendBusy = 0;
        gWifiConnection->newClientIndex = -1;
        return 22;
    case WIFI_CONNECTION_STATE_ERROR:
        return WifiConnection_CheckError();
    case WIFI_CONNECTION_STATE_DISCONNECTING:
        if (!gWifiConnection->voiceChatActive) {
            DWC_CloseAllConnectionsHard();
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
    if (DWC_GetMyAID() == 0) {
        if (gWifiConnection->recvLocalCallback != NULL) {
            gWifiConnection->recvLocalCallback(0, data, (u16)size);
        }
        return 1;
    }
    if (gWifiConnection->sendBusy != 0 || WifiConnection_CanSendToAll() == 0) {
        return 0;
    }
    *(u32 *)gWifiConnection->sendBuffer = 1 | ((u32)gWifiConnection->ownVoiceFlag << 8);
    gWifiConnection->sendSequence++;
    gWifiConnection->sendBuffer[2] = gWifiConnection->sendSequence;
    MI_CpuCopy8(data, gWifiConnection->sendBuffer + 4, size);
    gWifiConnection->sendBusy = 1;
    DWC_SendReliableBitmap(1, gWifiConnection, size + 4);
    return 1;
}

int WifiConnection_SendFromHost(void *data, int size) {
    if (size >= 0x100) {
        return 0;
    }
    if (DWC_GetMyAID() != 0) {
        return 0;
    }
    if (gWifiConnection->sendBusy != 0 || WifiConnection_CanSendToAll() == 0) {
        return 0;
    }
    *(u32 *)gWifiConnection->sendBuffer = 1 | ((u32)gWifiConnection->ownVoiceFlag << 8);
    gWifiConnection->sendSequence++;
    gWifiConnection->sendBuffer[2] = gWifiConnection->sendSequence;
    MI_CpuCopy8(data, gWifiConnection->sendBuffer + 4, size);
    gWifiConnection->sendBusy = 1;
    if (DWC_SendReliableBitmap(DWC_GetAIDBitmap(), gWifiConnection, size + 4) == 0) {
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
    *(u32 *)gWifiConnection->sendBuffer = 1 | ((u32)gWifiConnection->ownVoiceFlag << 8);
    gWifiConnection->sendSequence++;
    gWifiConnection->sendBuffer[2] = gWifiConnection->sendSequence;
    MI_CpuCopy8(data, gWifiConnection->sendBuffer + 4, size);
    gWifiConnection->sendBusy = 1;
    aidMask = DWC_GetAIDBitmap();
    if (DWC_SendReliableBitmap(aidMask, gWifiConnection, size + 4) != aidMask) {
        gWifiConnection->sendBusy = 0;
        return 0;
    }
    if (gWifiConnection->recvCallback != NULL) {
        gWifiConnection->recvCallback(DWC_GetMyAID(), data, (u16)size);
    }
    return 1;
}

void WifiConnection_LoginCallback(int result, int unused, int arg) {
    if (DWC_CheckDirtyFlag(gWifiConnection->userData)) {
        DWC_ClearDirtyFlag(gWifiConnection->userData);
        gWifiConnection->friendDataChanged = 1;
    }
    if (result == 0) {
        if (DWC_UpdateServersAsync(0, WifiConnection_UpdateServersCallback, gWifiConnection->userData, WifiConnection_FriendStatusCallback, arg, WifiConnection_DeleteFriendCallback, arg) == 0) {
            CommState_SetError(1);
            return;
        }
        DWC_SetBuddyFriendCallback(WifiConnection_BuddyFriendCallback, 0);
        return;
    }
    gWifiConnection->state = WIFI_CONNECTION_STATE_ERROR;
}

void WifiConnection_TimeoutCallback(void) {
    if (gWifiConnection->recvTimeoutArmed != 0) {
        DWC_CloseAllConnectionsHard();
        gWifiConnection->newClientIndex = -1;
        gWifiConnection->state = WIFI_CONNECTION_STATE_TIMED_OUT;
    }
}

void WifiConnection_UpdateServersCallback(u32 result) {
    if (result == 0) {
        gWifiConnection->state = WIFI_CONNECTION_STATE_LOGGED_IN;
    } else {
        gWifiConnection->state = WIFI_CONNECTION_STATE_ERROR;
    }
}

void WifiConnection_FriendStatusCallback(void) {
}

#pragma force_active on

void WifiConnection_DeleteFriendCallback(int index, int srcIndex) {
    FriendRoster_MergeFriend(Save_FriendRoster_Get(gWifiConnection->saveData), index, srcIndex);
    FrontierSave_ClearRow(Save_Frontier_GetStatic(gWifiConnection->saveData), index, srcIndex);
}

#pragma force_active reset

void WifiConnection_BuddyFriendCallback(void) {
}

void WifiConnection_ResetKeepAliveTimers(void) {
    MI_CpuFill8(gWifiConnection->keepAliveTimers, 0, sizeof(gWifiConnection->keepAliveTimers));
}

#pragma force_active on

void WifiConnection_SetRecvTimeouts(void) {
    int i;
    for (i = 0; i < 4; i++) {
        DWC_SetRecvTimeoutTime((u8)i, 0);
    }
    if (DWC_GetMyAID() == 0) {
        for (i = 0; i < gWifiConnection->maxPlayers; i++) {
            if (i != DWC_GetMyAID() && (DWC_GetAIDBitmap() & (1 << i))) {
                if (!DWC_SetRecvTimeoutTime((u8)i, 10000)) {
                    GF_AssertFail();
                }
            }
        }
    } else {
        if (!DWC_SetRecvTimeoutTime(0, 10000)) {
            GF_AssertFail();
        }
    }
    gWifiConnection->recvTimeoutArmed = 1;
    WifiConnection_ResetKeepAliveTimers();
}

void WifiConnection_OnConnected(int aid) {
    int i;
    gWifiConnection->state = WIFI_CONNECTION_STATE_CONNECTED;
    for (i = 0; i < gWifiConnection->maxPlayers; i++) {
        if (i != DWC_GetMyAID()) {
            WifiConnection_AllocRecvBuffer(i);
        }
    }
    WifiConnection_SetRecvTimeouts();
}

void WifiConnection_AnybodyMatchedCallback(int result, int cancelled) {
    WifiConnection *connection;
    if (result == 0) {
        if (cancelled == 0) {
            WifiConnection_OnConnected(1 - DWC_GetMyAID());
        } else {
            WifiConnection_FinishCancel();
        }
    } else {
        gWifiConnection->state = WIFI_CONNECTION_STATE_ERROR;
    }
    connection = gWifiConnection;
    if (connection->clientCallback) {
        int aid = DWC_GetMyAID();
        gWifiConnection->clientCallback(aid, connection->clientCallbackArg);
    }
}

#pragma force_active reset

BOOL WifiConnection_EvalPlayerCallback(void) {
    return TRUE;
}

void WifiConnection_SendDoneCallback(u32 result, s32 aid) {
    gWifiConnection->sendBusy = 0;
    gWifiConnection->keepAliveTimers[aid] = 0;
}

void WifiConnection_ReadPeerVoiceFlag(u32 header) {
    CommState_GetLinkMode();
    if (Comm_IsWifiPeerLinkMode() == 0) {
        if (header & 0x100) {
            gWifiConnection->peerVoiceFlag = 1;
        } else {
            gWifiConnection->peerVoiceFlag = 0;
        }
    }
}

#pragma force_active on

void WifiConnection_ReceiveCallback(u16 aid, const u8 *data, int size) {
    void *buf;
    u32 header;

    header = data[0] | ((data[1] << 8) | ((data[3] << 24) | (data[2] << 16)));
    gWifiConnection->recvTimeoutArmed = 1;
    if ((u8)header == 1) {
        WifiConnection_ReadPeerVoiceFlag(header);
        gWifiConnection->recvSequence = data[2];
    } else {
        if (VoiceChat_ReceiveData(aid, (void *)data, (void *)size) == 0) {
            WifiConnection_ReadPeerVoiceFlag(header);
        }
        return;
    }
    size -= 4;
    buf = WifiConnection_Alloc(0, size, 4);
    if (buf != NULL) {
        MI_CpuCopy8(data + 4, buf, size);
        if (DWC_GetMyAID() == 0) {
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
    gWifiConnection->matchInProgress = 0;
    if (result == 0 && gWifiConnection->disconnectWhenAlone == 1 && DWC_GetNumConnectionHost() == 1) {
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

    status = DWC_GetLastErrorEx(&error, &type);
    if (status) {
        result = error;
        if (error == 0 || type == 1) {
            result = status;
        }
        switch (type) {
        case 1:
            DWC_ClearError();
            break;
        case 2:
            DWC_ClearError();
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
                    DWC_ShutdownFriendsMatch();
                case 0:
                case 1:
                case 2:
                    if (CommState_GetLinkMode() != 33) {
                        DWC_CleanupInet();
                    }
                    break;
                }
                DWC_ClearError();
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
            if (i != DWC_GetMyAID() && DWC_IsValidAID((u8)i) != 0) {
                found = 1;
                if (DWC_IsSendableReliable((u8)i) == 0) {
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
        u32 mask = (u32)DWC_GetAIDBitmap();
        if ((mask & 0xFFFE) != 0) {
            gWifiConnection->sendBusy = 1;
            *(u32 *)gWifiConnection->sendBuffer = 2 | ((u32)gWifiConnection->ownVoiceFlag << 8);
            DWC_SendReliableBitmap(DWC_GetAIDBitmap(), gWifiConnection, 4);
            gWifiConnection->keepAliveTimers[aid] = 0;
            return 1;
        }
    }
    return 0;
}

#pragma force_active on

int WifiConnection_UpdateConnected(void) {
    s32 state;

    DWC_ProcessFriendsMatch();
    WifiConnection_UpdateFriendStatus();
    if (gWifiConnection->voiceChatActive) {
        if (gWifiConnection->ownVoiceFlag == 1 && gWifiConnection->peerVoiceFlag == 1 && gWifiConnection->voiceUnmuted == 1) {
            VoiceChat_Unmute();
        } else {
            VoiceChat_Mute();
        }
        VoiceChat_Update();
        {
            WifiConnection *connection = gWifiConnection;
            if (connection->conferenceAidMask != DWC_GetAIDBitmap() && !connection->unk_10DF && connection->voiceChatEnabled) {
                s32 aidMask = DWC_GetAIDBitmap();
                s32 ownAid = DWC_GetMyAID();
                if (VoiceChat_AddConferenceClients(aidMask, ownAid)) {
                    gWifiConnection->conferenceAidMask = DWC_GetAIDBitmap();
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
        return DWC_GetMyAID();
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
    online = Comm_IsWifiPeerLinkMode();
    if (online) {
        count = Comm_CountConnected() - 1;
        if (count < 1) {
            count = 1;
        }
        gWifiConnection->ownVoiceFlag = 1;
        gWifiConnection->peerVoiceFlag = 1;
        gWifiConnection->voiceUnmuted = 1;
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
        gWifiConnection->newClientIndex = -1;
        WifiConnection_ResetStopServerMatching();
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
    DWC_ShutdownFriendsMatch();
    DWC_CleanupInet();
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
            gWifiConnection->friendStatus[n] = DWC_GetFriendStatusData(&gWifiConnection->friendList[n], gWifiConnection->friendStatusData + gWifiConnection->friendStatusStride * n, &result);
        }
        gWifiConnection->friendStatusIndex = (gWifiConnection->friendStatusIndex + 1) & 31;
    }
}

#pragma force_active reset

void WifiConnection_SetOwnStatusData(void *data, int size) {
    DWC_SetOwnStatusData();
}

u8 WifiConnection_GetFriendStatus(int index) {
    return gWifiConnection->friendStatus[index];
}

#pragma force_active on

int WifiConnection_StartMatchmaking(int friendIndex, int maxPlayers, BOOL alternate) {
    int result;

    if (WifiConnection_IsFriendDataChanged()) {
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
    gWifiConnection->matchedAidBitmap = 0;
    gWifiConnection->disconnectWhenAlone = 1;
    gWifiConnection->targetFriendIndex = friendIndex;
    gWifiConnection->maxPlayers = maxPlayers;
    if (alternate) {
        maxPlayers = 2;
    }
    gWifiConnection->matchInProgress = 1;
    if (friendIndex < 0) {
        result = DWC_SetupGameServer(maxPlayers, WifiConnection_GameServerMatchedCallback, NULL, WifiConnection_NewClientCallback, NULL);
        gWifiConnection->matchType = 1;
    } else {
        result = DWC_ConnectToGameServerAsync(friendIndex, WifiConnection_ConnectedToServerCallback, NULL, WifiConnection_NewClientCallback, NULL);
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
    DWC_SetUserSendCallback(WifiConnection_SendDoneCallback);
    DWC_SetUserRecvCallback(WifiConnection_ReceiveCallback);
    DWC_SetConnectionClosedCallback(WifiConnection_ConnectionClosedCallback, 0);
    DWC_SetUserRecvTimeoutCallback(WifiConnection_TimeoutCallback);
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

void WifiConnection_GameServerMatchedCallback(int result, int cancelled, int cancelledByMe, int unused, int aid) {
    int reject = 0;
    gWifiConnection->matchInProgress = 0;
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
            if (gWifiConnection->stopMatchingState || reject) {
                mask = DWC_GetAIDBitmap() & ~gWifiConnection->matchedAidBitmap;
                current = DWC_GetAIDBitmap();
                if (mask) {
                    DWC_CloseConnectionHardBitmap(&mask);
                    if ((mask ^ current) == 1) {
                        gWifiConnection->state = WIFI_CONNECTION_STATE_MATCHED;
                    }
                    return;
                }
            }
            gWifiConnection->targetFriendIndex = aid;
            gWifiConnection->matchedAidBitmap = DWC_GetAIDBitmap();
            if (gWifiConnection->matchedAidBitmap == 1) {
                gWifiConnection->state = WIFI_CONNECTION_STATE_MATCHED;
                return;
            }
            WifiConnection_OnConnected(aid);
        } else if (cancelledByMe == 0) {
            gWifiConnection->newClientIndex = -1;
        }
    }
}

#pragma force_active reset

void WifiConnection_ConnectedToServerCallback(u32 result, u32 cancelled, u32 a2, u32 a3, u32 aid) {
    gWifiConnection->matchInProgress = 0;
    if (result == 0 && cancelled == 0) {
        WifiConnection_OnConnected(aid);
    }
}

#pragma force_active on

void WifiConnection_NewClientCallback(int index) {
    void (*callback)(int, void *);
    gWifiConnection->newClientIndex = index;
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

int WifiConnection_IsVoiceChatEnabled(void) {
    if (gWifiConnection != NULL) {
        return gWifiConnection->voiceChatEnabled;
    }
    return 0;
}

int WifiConnection_GetNewClientIndex(void) {
    if (gWifiConnection != NULL) {
        return gWifiConnection->newClientIndex;
    }
    return 0;
}

void WifiConnection_SetVoiceChatEnabled(int value) {
    gWifiConnection->voiceChatEnabled = value;
}

void WifiConnection_SetVoiceUnmuted(u16 value) {
    gWifiConnection->voiceUnmuted = value;
}

void WifiConnection_StopServerMatchingCallback(void) {
    gWifiConnection->stopMatchingState = 2;
}

#pragma force_active on

BOOL WifiConnection_StopServerMatching(void) {
    if (gWifiConnection->stopMatchingState == 0) {
        gWifiConnection->stopMatchingState = 1;
        DWC_StopSCMatchingAsync(WifiConnection_StopServerMatchingCallback, 0);
    }
    if (gWifiConnection->stopMatchingState == 2) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

void WifiConnection_ResetStopServerMatching(void) {
    gWifiConnection->stopMatchingState = 0;
}

#pragma force_active on

void ov00_021E7144(void) {
    DWC_GetNumConnectionHost();
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

void WifiConnection_SetDisconnectWhenAlone(u8 value) {
    gWifiConnection->disconnectWhenAlone = value;
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
        DWC_SetRecvBuffer((u8)aid, (void *)(((u32)gWifiConnection->recvBuffers[aid] + 31) & ~31), 0x1000);
    }
}

#pragma force_active reset

void WifiConnection_FreeRecvBuffers(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        WifiConnection_FreeRecvBuffer(i);
    }
}

u8 WifiConnection_IsFriendDataChanged(void) {
    return gWifiConnection->friendDataChanged;
}

void WifiConnection_ClearFriendDataChanged(void) {
    gWifiConnection->friendDataChanged = 0;
}
