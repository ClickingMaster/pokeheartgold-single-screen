#include "wifi_setup.h"

#include "global.h"

extern int ov13_022459B4;

void *WifiSetup_Alloc(int size) {
    return gWifiSetupRuntime.alloc(size);
}

void WifiSetup_Free(void *ptr) {
    gWifiSetupFreeHook(ptr);
}

#pragma force_active on

int WifiSetup_RunAoss(WifiSetupRequest *request) {
    int valid;

    if (request->connectRetries == 0 || request->connectRetries < -1 || request->connectDelay < -1 || request->receiveRetries == 0 || request->receiveRetries < -1 || request->receiveDelay < -1 || request->receiveTimeout < -1 || request->keyLength == 0 || request->keyLength > 256 || request->key[request->keyLength - 1] != 0) {
        valid = -1;
    } else {
        valid = 0;
    }
    if (!gWifiSetupRuntime.alloc || !gWifiSetupFreeHook) {
        valid = -1;
    }
    if (valid == -1) {
        request->error = 15;
        WifiSetup_FreeWorkBuffers();
        return -1;
    }
    gWifiSetupState.data = WifiSetup_Alloc(1528);
    if (gWifiSetupState.data == NULL) {
        request->error = 15;
        WifiSetup_FreeWorkBuffers();
        return -1;
    }
    WifiSetup_SetStatus(-1);
    int result = WifiSetup_RunAossSession(request);
    WifiSetup_Free(gWifiSetupState.data);
    WifiSetup_FreeWorkBuffers();
    if (ov13_022459B4 != -1) {
        WifiSetup_Close(ov13_022459B4);
    }
    return result;
}

#pragma force_active reset
