#include "global.h"

#include "wifi_setup.h"

extern u8 gWifiSetupPeerAddress[];
extern u8 ov13_0224CFBC[];
extern u8 ov13_0224CFB8[];

u32 WifiSetup_NextHostAddress(u32 value, u32 mask) {
    u32 common = value & mask;
    mask = ~mask;
    value &= mask;
    value++;
    value |= common;
    mask |= common;
    if (value >= mask) {
        value = common | 1;
    }
    return value;
}

void WifiSetup_FreeWorkBuffers(void) {
    if (gWifiSetupState.status != NULL) {
        WifiSetup_Free(gWifiSetupState.status);
        gWifiSetupState.status = NULL;
    }
    if (gWifiSetupState.connection != NULL) {
        WifiSetup_Free(gWifiSetupState.connection);
        gWifiSetupState.connection = NULL;
    }
}

void WifiSetup_InitState(WifiSetupRequest *request) {
    WifiSetup_MemFill(gWifiSetupPeerAddress, 0, 8);
    gWifiSetupState.error = 1;
    WifiSetup_MemFill(ov13_0224CFBC, 0, 0x1C);
    gWifiSetupState.unk_24 = request->key;
    gWifiSetupState.unk_28 = request->keyLength;
    gWifiSetupState.requestedMask = request->requestMask & 0xF;
    ov13_0224CFB8[0x1D] = request->unk_002;
    gWifiSetupState.receivedMask = 0;
    gWifiSetupState.address = 0xC0A80B01; // 192.168.11.1
    ov13_0224CFB8[0x1C] = 0;
}

void WifiSetup_SetError(int error) {
    gWifiSetupState.error = error;
}

int WifiSetup_GetError(void) {
    return gWifiSetupState.error;
}
