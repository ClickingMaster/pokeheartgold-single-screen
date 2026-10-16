#include "WM.h"
#include "to55_overlay13_r14_private.h"
#include "wifi_setup_wcm.h"

typedef struct ConnectKey {
    u8 data[0x14];
} ConnectKey;

typedef struct ConnectSetting {
    u8 keyType;
    u8 keyIndex;
    ConnectKey keys[4];
    u8 unk52[0xE];
} ConnectSetting;

typedef struct ApInfo {
    u8 unk00[0xA];
    u16 ssidLength;
} ApInfo;

typedef struct ConnectParam {
    u32 ssidLength;
    u8 ssid[0x20];
    int mode;
    u32 keyLength;
    u8 key[0x20];
} ConnectParam;

extern ConnectSetting gWifiSetupConnectSetting;
extern ApInfo gWifiSetupConnectApInfo;
void MI_CpuFill8(void *dest, u8 value, u32 size);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void WCM_ClearApList(void);
void WifiSetup_CopyConnectSsid(ConnectParam *param, ApInfo *info);
BOOL WifiSetupWcm_StartConnect(ApInfo *info, ConnectSetting *setting, u32 mode);
void WifiSetup_StoreConnectResult(void *out, ApInfo *info, BOOL failed);
int WifiSetup_ConnectAccessPoint(ConnectParam *param, void *out);
#define APINFO2 ((ApInfo *)((u8 *)&gWifiSetupRuntime + 0x40))
// @fn WifiSetup_ConnectAccessPoint
#pragma optimization_level 2
#pragma opt_propagation off
#pragma opt_loop_invariants on
int WifiSetup_ConnectAccessPoint(ConnectParam *param, void *out) {
    OSAlarm alarm;
    OSMessage message;
    BOOL running = TRUE;
    int result = -1;
    int retries;
    u32 mode;
    int connected = 0;
    int n;
    int i;

    if (param->mode == 0) {
        mode = 0x80000;
    } else if (param->mode == 1) {
        mode = 0xC0000;
    }
    mode |= 0x30000;
    MI_CpuFill8(&gWifiSetupConnectSetting, 0, sizeof(ConnectSetting));
    if (param->keyLength == 5) {
        gWifiSetupConnectSetting.keyType = WM_WEPMODE_40BIT;
    } else if (param->keyLength == 13) {
        gWifiSetupConnectSetting.keyType = WM_WEPMODE_104BIT;
    } else if (param->keyLength == 16) {
        gWifiSetupConnectSetting.keyType = WM_WEPMODE_128BIT;
    } else {
        return -1;
    }
    gWifiSetupConnectSetting.keyIndex = connected;
    MI_CpuCopy8(param->key, (u8 *)&gWifiSetupConnectSetting + (connected * sizeof(ConnectKey) + 2), param->keyLength);
    WCM_ClearApList();
    if (WifiSetupWcm_StartSearch(0, (u32)param->ssid, param->ssidLength, 0x0030BFFE)) {
        retries = 0;
        OS_CreateAlarm(&alarm);
        OS_SetAlarm(&alarm, 0x003FEC42, (OSAlarmHandler)WifiSetup_OnTimeoutAlarm, (void *)0x12);
        while (running) {
            OS_ReceiveMessage(&gWifiSetupMessageQueue, &message, OS_MESSAGE_BLOCK);
            switch ((u32)message) {
            case 18:
                if (!connected) {
                    running = FALSE;
                }
                break;
            case 19:
                if (connected) {
                    break;
                }
                break;
            case WIFI_SETUP_WCM_EVENT_FOUND_AP:
                if (!connected) {
                    OS_CancelAlarm(&alarm);
                    if (WifiSetupWcm_CopyApList(&gWifiSetupConnectApInfo, 1) != 1) {
                        running = FALSE;
                    } else {
                        WifiSetup_CopyConnectSsid(param, &gWifiSetupConnectApInfo);
                        i = 0;
                        n = APINFO2->ssidLength;
                        for (; i < n; i++) {
                        }
                        if (!WifiSetupWcm_StartConnect(&gWifiSetupConnectApInfo, &gWifiSetupConnectSetting, mode)) {
                            running = FALSE;
                        } else {
                            connected = 1;
                        }
                    }
                }
                break;
            case WIFI_SETUP_WCM_EVENT_SEARCH_ENDED:
                WifiSetup_CopyConnectSsid(param, &gWifiSetupConnectApInfo);
                if (!WifiSetupWcm_StartConnect(&gWifiSetupConnectApInfo, &gWifiSetupConnectSetting, mode)) {
                    running = FALSE;
                }
                break;
            case WIFI_SETUP_WCM_EVENT_CONNECTED:
                running = FALSE;
                result = 0;
                break;
            case WIFI_SETUP_WCM_EVENT_CONNECT_FAILED:
                if (++retries < 3) {
                    if (!WifiSetupWcm_StartConnect(&gWifiSetupConnectApInfo, &gWifiSetupConnectSetting, mode)) {
                        running = FALSE;
                    }
                } else {
                    running = FALSE;
                }
                break;
            case 4:
            case WIFI_SETUP_WCM_EVENT_SEARCH_STARTED:
                break;
            case 0:
            default:
                running = FALSE;
                break;
            }
        }
        OS_CancelAlarm(&alarm);
        while (OS_ReceiveMessage(&gWifiSetupMessageQueue, &message, OS_MESSAGE_NOBLOCK) == TRUE) {
        }
    }
    WifiSetup_StoreConnectResult(out, &gWifiSetupConnectApInfo, result == 0);
    return result;
}
#pragma optimization_level reset
#pragma opt_propagation reset
#pragma opt_loop_invariants reset
