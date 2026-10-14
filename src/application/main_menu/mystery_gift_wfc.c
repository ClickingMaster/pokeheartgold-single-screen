#include "application/main_menu/mystery_gift_wfc.h"

#include "global.h"

#include "application/main_menu/mystery_gift_app.h"
#include "application/main_menu/mystery_gift_app_internal.h"

#include "heap.h"
#include "overlay_manager.h"
#include "system.h"
#include "comm_wireless.h"
#include "wifi_connection.h"

// Mystery Gift > Get via Nintendo Wi-Fi Connection: connects, downloads the gift file of the game (into
// MysteryGiftAppData.wfcRecvBuffer) and disconnects. MysteryGiftApp_RunWfcDownload runs MysteryGiftWfc_Run every frame.

// states of MysteryGiftWfc_Run (MysteryGiftAppData.wfcState)
#define WFC_STATE_LOAD_OVERLAYS  0x1000
#define WFC_STATE_WAIT_OVERLAYS  0x1001
#define WFC_STATE_INIT           0x1002
#define WFC_STATE_CONNECT        0x1003
#define WFC_STATE_LOGIN          0x1004
#define WFC_STATE_WAIT_LOGIN     0x1005
#define WFC_STATE_INIT_DOWNLOAD  0x1006
#define WFC_STATE_SET_ATTRIBUTES 0x1007
#define WFC_STATE_GET_FILE_COUNT 0x1008
#define WFC_STATE_GET_FILE_LIST  0x1009
#define WFC_STATE_GET_FILE       0x100A
#define WFC_STATE_DOWNLOADING    0x100B
#define WFC_STATE_CLEANUP        0x100C
#define WFC_STATE_DISCONNECT     0x100D
#define WFC_STATE_ERROR          0x100F
#define WFC_STATE_WAIT_BUTTON    0x1010
#define WFC_STATE_ERROR_SHOWN    0x1011
#define WFC_STATE_WAIT_REQUEST   0x1012 // until MysteryGiftWfc_OnRequestDone, then wfcNextState
#define WFC_STATE_WAIT_CLEANUP   0x1013 // until MysteryGiftWfc_OnCleanupDone, then wfcNextState

// The state of the download library callbacks (sMysteryGiftWfcStatus)
typedef struct MysteryGiftWfcStatus {
    BOOL requestDone; // MysteryGiftWfc_OnRequestDone
    BOOL cleanupDone; // MysteryGiftWfc_OnCleanupDone
    int result;       // MYSTERY_GIFT_WFC_RESULT_*
    int requestError;
    BOOL errorShown;
} MysteryGiftWfcStatus;

int DWC_GetLastErrorEx(s32 *errorCode, s32 *errorType);
int DWC_GetInetStatus(void);
void DWC_GetApInfo(void *data);
int DWC_NdCleanupAsync(void (*callback)(void));
void DWC_ClearError(void);
void DWC_SetMemFunc(void *(*alloc)(void *, u32), void (*free)(void *, void *));
void DWC_InitInetEx(void *config, int a1, int a2, int a3);
void DWC_SetAuthServer(int a0);
void DWC_ConnectInetAsync(void);
int DWC_CheckInet(void);
void DWC_ProcessInet(void);
void DWC_CleanupInet(void);
int DWC_CleanupInetAsync(void);
int DWC_UpdateConnection(void);
int DWC_NASLoginAsync(void);
int DWC_NASLoginProcess(void);
void DWC_NASLoginAbort(void);
int DWC_NdInitAsync(void (*callback)(int, int), const char *gameCode, const char *secretKey);
int DWC_NdSetAttr(const char *attribute1, const char *attribute2, const char *attribute3);
int DWC_NdGetFileListNumAsync(int *count);
int DWC_NdGetFileListAsync(void *fileList, int start, int count);
int DWC_NdGetFileAsync(void *fileInfo, void *buffer, int size);
int DWC_NdCancelAsync(void);
int DWC_NdGetProgress(u32 *received, u32 *total);
void Comm_InitDwc(int heapID);
void CommState_InitMysteryGiftWfcSession(SaveData *saveData);
void CommState_FreeMysteryGiftWfcSession(void);
void LoadDwcOverlay(void);
void UnloadDwcOverlay(void);
void LoadOVY38(void);
void UnloadOVY38(void);
void WirelessStatusIcon_Create(void);
void WirelessStatusIcon_Destroy(void);
void CommWireless_Terminate(void);

extern MysteryGiftWfcStatus sMysteryGiftWfcStatus;
extern const char sStr_IPGE[];
extern const char sStr_K5WfPEw9T3cvqzHa[];
extern const char ov74_0223D054[];
extern u8 sMysteryGiftWfcFileList[];

#pragma force_active on
#pragma push
#pragma opt_propagation off

// Allocator of the Wi-Fi library: 32-byte aligned blocks, the raw block is stored in the word before
void *MysteryGiftWfc_Alloc(void *unused, u32 size) {
    void *raw;
    u32 result;
    size += 36;
    size &= ~31u;
    size += 32;
    raw = Heap_Alloc(HEAP_ID_84, size);
    result = (u32)raw + 32;
    result &= ~31u;
    *(void **)(result - 4) = raw;
    return (void *)result;
}

#pragma pop
#pragma force_active reset

void MysteryGiftWfc_Free(void *unused, void *ptr) {
    if (ptr != NULL) {
        Heap_Free(((void **)ptr)[-1]);
    }
}

#pragma force_active on

// Shows the error of the Wi-Fi library (MysteryGiftApp_ShowWfcError)
int MysteryGiftWfc_HandleError(MysteryGiftAppData *data) {
    s32 error[2];
    DWC_GetLastErrorEx(&error[0], &error[1]);
    data->wfcErrorMsgId = WifiConnection_ClassifyErrorCode(-error[0], error[1]);
    data->wfcErrorCode = -error[0];
    data->wfcErrorType = error[1];
    data->wfcErrorReady = TRUE;
    sMysteryGiftWfcStatus.errorShown = TRUE;
    data->wfcCallback = NULL;
    ov74_02229E60(data, FALSE);
    return WFC_STATE_ERROR;
}

BOOL ov74_02236A2C(MysteryGiftAppData *data) {
    u8 buffer[60];
    switch (DWC_GetInetStatus()) {
    case 4:
        DWC_GetApInfo(buffer);
        return TRUE;
    case 7:
        break;
    case 8:
        break;
    }
    return FALSE;
}

// Waits for the request just started, then continues with nextState
void MysteryGiftWfc_WaitForRequest(MysteryGiftAppData *data, int *state, int nextState) {
    sMysteryGiftWfcStatus.requestDone = FALSE;
    sMysteryGiftWfcStatus.requestError = 0;
    data->wfcNextState = nextState;
    *state = WFC_STATE_WAIT_REQUEST;
}

// Ends the download library with the given result, then continues with nextState (or failState at once)
void MysteryGiftWfc_StartCleanup(MysteryGiftAppData *data, int result, int *state, int nextState, int failState) {
    sMysteryGiftWfcStatus.cleanupDone = FALSE;
    sMysteryGiftWfcStatus.result = result;
    data->wfcNextState = nextState;
    *state = WFC_STATE_WAIT_CLEANUP;
    if (!DWC_NdCleanupAsync(MysteryGiftWfc_OnCleanupDone)) {
        *state = failState;
    }
}

#pragma force_active reset

void MysteryGiftWfc_OnRequestDone(int unused, int error) {
    sMysteryGiftWfcStatus.requestDone = TRUE;
    sMysteryGiftWfcStatus.requestError = error;
}

void MysteryGiftWfc_OnCleanupDone(void) {
    sMysteryGiftWfcStatus.cleanupDone = TRUE;
}

void MysteryGiftWfc_Shutdown(void) {
    WirelessStatusIcon_Destroy();
    CommState_FreeMysteryGiftWfcSession();
    UnloadOVY38();
    UnloadDwcOverlay();
    CommWireless_Terminate();
}

#pragma force_active on

// One frame of the download; returns MYSTERY_GIFT_WFC_RESULT_* (0 while running)
int MysteryGiftWfc_Run(OverlayManager *manager, int *state) {
    MysteryGiftAppData *data = OverlayManager_GetData(manager);
    if (data->wfcCallback) {
        if (data->wfcCallback() == 1) {
            *state = MysteryGiftWfc_HandleError(data);
        }
    }
    switch (*state) {
    case WFC_STATE_LOAD_OVERLAYS:
        CommWireless_StartUp();
        LoadDwcOverlay();
        LoadOVY38();
        Comm_InitDwc(3);
        *state = WFC_STATE_WAIT_OVERLAYS;
        break;
    case WFC_STATE_WAIT_OVERLAYS:
        if (CommWireless_IsStartedUp()) {
            DWC_SetMemFunc(MysteryGiftWfc_Alloc, MysteryGiftWfc_Free);
            CommState_InitMysteryGiftWfcSession(data->saveData);
            *state = WFC_STATE_INIT;
        }
        break;
    case WFC_STATE_INIT:
        DWC_InitInetEx(data->wfcConfig, 2, 1, 20);
        DWC_SetAuthServer(2);
        DWC_ConnectInetAsync();
        WirelessStatusIcon_Create();
        *state = WFC_STATE_CONNECT;
        data->wfcCancel = FALSE;
        break;
    case WFC_STATE_CONNECT:
        DWC_ProcessInet();
        if (DWC_CheckInet()) {
            if (ov74_02236A2C(data) == TRUE) {
                if (data->wfcCancel == TRUE) {
                    sMysteryGiftWfcStatus.result = MYSTERY_GIFT_WFC_RESULT_CANCELLED;
                    *state = WFC_STATE_DISCONNECT;
                } else {
                    *state = WFC_STATE_LOGIN;
                }
            } else {
                *state = MysteryGiftWfc_HandleError(data);
            }
        }
        if (gSystem.newKeys & PAD_BUTTON_B) {
            data->wfcCancel = TRUE;
        }
        break;
    case WFC_STATE_LOGIN:
        if (!DWC_NASLoginAsync()) {
            *state = MysteryGiftWfc_HandleError(data);
        } else {
            *state = WFC_STATE_WAIT_LOGIN;
        }
        break;
    case WFC_STATE_WAIT_LOGIN: {
        int result = DWC_NASLoginProcess();
        if (result == 3) {
            *state = WFC_STATE_INIT_DOWNLOAD;
            data->wfcCallback = DWC_UpdateConnection;
        } else if (result == 4) {
            *state = MysteryGiftWfc_HandleError(data);
            DWC_CleanupInet();
        } else if (result == 5) {
            sMysteryGiftWfcStatus.result = MYSTERY_GIFT_WFC_RESULT_CANCELLED;
            *state = WFC_STATE_DISCONNECT;
        }
        if (gSystem.newKeys & PAD_BUTTON_B) {
            DWC_NASLoginAbort();
        }
        break;
    }
    case WFC_STATE_INIT_DOWNLOAD:
        if (!DWC_NdInitAsync(MysteryGiftWfc_OnRequestDone, sStr_IPGE, sStr_K5WfPEw9T3cvqzHa)) { // "IPGE", the secret key
            *state = MysteryGiftWfc_HandleError(data);
        } else {
            MysteryGiftWfc_WaitForRequest(data, state, WFC_STATE_SET_ATTRIBUTES);
        }
        break;
    case WFC_STATE_SET_ATTRIBUTES:
        if (data->wfcCancel == TRUE) {
            MysteryGiftWfc_StartCleanup(data, MYSTERY_GIFT_WFC_RESULT_CANCELLED, state, WFC_STATE_CLEANUP, WFC_STATE_CLEANUP);
        } else {
            if (!DWC_NdSetAttr(ov74_0223D054, ov74_0223D054, ov74_0223D054)) { // ""
                *state = MysteryGiftWfc_HandleError(data);
            } else {
                *state = WFC_STATE_GET_FILE_COUNT;
            }
        }
        break;
    case WFC_STATE_GET_FILE_COUNT:
        if (!DWC_NdGetFileListNumAsync(&data->wfcFileCount)) {
            *state = MysteryGiftWfc_HandleError(data);
        } else {
            MysteryGiftWfc_WaitForRequest(data, state, WFC_STATE_GET_FILE_LIST);
        }
        break;
    case WFC_STATE_GET_FILE_LIST:
        if (data->wfcFileCount != 1) {
            MysteryGiftWfc_StartCleanup(data, MYSTERY_GIFT_WFC_RESULT_NO_GIFT, state, WFC_STATE_DISCONNECT, WFC_STATE_DISCONNECT);
        } else {
            if (!DWC_NdGetFileListAsync(sMysteryGiftWfcFileList, 0, 10)) {
                *state = MysteryGiftWfc_HandleError(data);
            } else {
                MysteryGiftWfc_WaitForRequest(data, state, WFC_STATE_GET_FILE);
            }
        }
        break;
    case WFC_STATE_GET_FILE:
        if (!DWC_NdGetFileAsync(sMysteryGiftWfcFileList, data->wfcRecvBuffer, sizeof(data->wfcRecvBuffer))) {
            *state = MysteryGiftWfc_HandleError(data);
        } else {
            *state = WFC_STATE_DOWNLOADING;
            data->wfcProgress = 0;
        }
        break;
    case WFC_STATE_DOWNLOADING:
        if (!sMysteryGiftWfcStatus.requestDone) {
            if (gSystem.newKeys & PAD_BUTTON_B) {
                MysteryGiftWfc_StartCleanup(data, MYSTERY_GIFT_WFC_RESULT_CANCELLED, state, WFC_STATE_CLEANUP, WFC_STATE_CLEANUP);
            } else if (DWC_NdGetProgress(&data->wfcBytesReceived, &data->wfcBytesTotal) == 1) {
                u32 progress = 100 * data->wfcBytesReceived / data->wfcBytesTotal;
                if (data->wfcProgress != progress) {
                    data->wfcProgress = progress;
                }
            }
        } else {
            if (sMysteryGiftWfcStatus.requestError) {
                *state = MysteryGiftWfc_HandleError(data);
            } else if (data->wfcCancel == FALSE) {
                MysteryGiftWfc_StartCleanup(data, MYSTERY_GIFT_WFC_RESULT_RECEIVED, state, WFC_STATE_DISCONNECT, WFC_STATE_DISCONNECT);
            } else {
                MysteryGiftWfc_StartCleanup(data, MYSTERY_GIFT_WFC_RESULT_CANCELLED, state, WFC_STATE_DISCONNECT, WFC_STATE_DISCONNECT);
            }
        }
        break;
    case WFC_STATE_CLEANUP:
        if (!DWC_NdCancelAsync()) {
            *state = WFC_STATE_DISCONNECT;
        } else {
            data->wfcCallback = NULL;
            MysteryGiftWfc_Shutdown();
            return sMysteryGiftWfcStatus.result;
        }
        break;
    case WFC_STATE_DISCONNECT:
        if (DWC_CleanupInetAsync() == 1) {
            data->wfcCallback = NULL;
            MysteryGiftWfc_Shutdown();
            return sMysteryGiftWfcStatus.result;
        }
        break;
    case WFC_STATE_ERROR:
        if (sMysteryGiftWfcStatus.errorShown == TRUE) {
            MysteryGiftApp_ShowWfcError(data);
            if (data->wfcErrorType == 5 || data->wfcErrorType == 6) {
                MysteryGiftWfc_StartCleanup(data, MYSTERY_GIFT_WFC_RESULT_CANCELLED, state, WFC_STATE_ERROR_SHOWN, WFC_STATE_WAIT_BUTTON);
            } else {
                *state = WFC_STATE_ERROR_SHOWN;
            }
        }
        break;
    case WFC_STATE_ERROR_SHOWN:
        DWC_CleanupInet();
        *state = WFC_STATE_WAIT_BUTTON;
        break;
    case WFC_STATE_WAIT_BUTTON:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            data->wfcCallback = NULL;
            DWC_ClearError();
            MysteryGiftWfc_Shutdown();
            return MYSTERY_GIFT_WFC_RESULT_ERROR;
        }
        break;
    case WFC_STATE_WAIT_REQUEST:
        if (sMysteryGiftWfcStatus.requestDone == TRUE) {
            sMysteryGiftWfcStatus.requestDone = FALSE;
            if (sMysteryGiftWfcStatus.requestError) {
                *state = MysteryGiftWfc_HandleError(data);
            } else {
                *state = data->wfcNextState;
            }
        } else if (gSystem.newKeys & PAD_BUTTON_B) {
            data->wfcCancel = TRUE;
        }
        break;
    case WFC_STATE_WAIT_CLEANUP:
        if (sMysteryGiftWfcStatus.cleanupDone == TRUE) {
            sMysteryGiftWfcStatus.cleanupDone = FALSE;
            *state = data->wfcNextState;
        } else if (gSystem.newKeys & PAD_BUTTON_B) {
            data->wfcCancel = TRUE;
        }
        break;
    }
    return 0;
}

#pragma force_active reset
