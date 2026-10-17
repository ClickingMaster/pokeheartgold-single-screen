#include "wifi_setup/wifi_setup_wcm.h"

#include "global.h"

#include "wifi_setup/wifi_setup.h"

extern WifiSetupWcm gWifiSetupWcm;
extern u8 gWifiSetupWcmSearchBssid[6];
extern u8 gWifiSetupWcmSearchSsid[32];
extern u8 WCM_Bssid_Any[]; // BSSID ff:ff:ff:ff:ff:ff (any)
extern u8 WCM_Essid_Any[]; // SSID of 0xFF bytes (any)

s32 WifiSetup_NotifyStatus(s32 arg);
void WifiSetupWcm_HandleEvent(WifiSetupWcmEvent *event);
int WifiSetupWcm_ReturnToIdle(void);
int WifiSetupWcm_CopyApList(u8 *dst, int max);
int WifiSetupWcm_StartSearch(const u8 *bssid, const u8 *ssid, int len, int option);
int WifiSetupWcm_StopSearch(void);
int WifiSetupWcm_StartDisconnect(void);
int WifiSetupWcm_StartCleanup(void);
int WifiSetupWcm_StartConnect(void *ap, void *key, int option);
BOOL WifiSetupWcm_Init(void (*callback)(int event, int arg), void *allocation, u32 size);

void WifiSetup_Sleep(s32 ticks) {
    OS_Sleep(ticks);
}

s32 WifiSetup_NotifyStatus(s32 arg) {
    s32 (*callback)(s32) = gWifiSetupRuntime.callback;
    if (callback != NULL) {
        callback(arg);
    }
    return 0;
}

void WifiSetupWcm_HandleEvent(WifiSetupWcmEvent *event) {
    if (!event) {
        return;
    }
    switch (event->type) {
    case 1:
        if (event->result == 0) {
            if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_BUSY) {
                gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
                if (gWifiSetupWcm.callback) {
                    gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_STARTED, 0);
                }
            } else if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_SEARCH_STARTING) {
                if (WCM_SearchAsync(gWifiSetupWcm.searchBssid, gWifiSetupWcm.searchSsid, gWifiSetupWcm.searchOption) != 3) {
                    gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_CONNECTING) {
                if (WCM_ConnectAsync(gWifiSetupWcm.connectAp, gWifiSetupWcm.connectKey, gWifiSetupWcm.connectOption) != 3) {
                    gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            }
        } else {
            gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_READY;
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
            }
        }
        break;
    case 3:
        if (event->result == 0) {
            if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_SEARCH_STARTING) {
                gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_SEARCHING;
                if (gWifiSetupWcm.callback) {
                    gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_SEARCH_STARTED, 0);
                }
            }
        } else {
            gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_SEARCH_FAILED, 0);
            }
        }
        break;
    case 5:
        if (event->result == 0) {
            if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_CONNECTING) {
                gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_CONNECTED;
                if (gWifiSetupWcm.callback) {
                    gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_CONNECTED, 0);
                }
            }
        } else {
            gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_CONNECT_FAILED, 0);
            }
        }
        break;
    case 4:
        if (event->result == 0) {
            if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_BUSY) {
                gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
                if (gWifiSetupWcm.callback) {
                    gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_SEARCH_ENDED, 0);
                }
            } else if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_SEARCH_STARTING) {
                if (WCM_SearchAsync(gWifiSetupWcm.searchBssid, gWifiSetupWcm.searchSsid, gWifiSetupWcm.searchOption) != 3) {
                    gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_CLEANUP) {
                if (WCM_CleanupAsync() != 3) {
                    gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_CONNECTING) {
                if (WCM_ConnectAsync(gWifiSetupWcm.connectAp, gWifiSetupWcm.connectKey, gWifiSetupWcm.connectOption) != 3) {
                    gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            }
        } else {
            gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_SEARCH_END_FAILED, 0);
            }
        }
        break;
    case 6:
        if (event->result == 0) {
            if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_BUSY) {
                gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
                if (gWifiSetupWcm.callback) {
                    gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_DISCONNECTED, 0);
                }
            } else if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_SEARCH_STARTING) {
                if (WCM_SearchAsync(gWifiSetupWcm.searchBssid, gWifiSetupWcm.searchSsid, gWifiSetupWcm.searchOption) != 3) {
                    gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_CLEANUP) {
                if (WCM_CleanupAsync() != 3) {
                    gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_CONNECTING) {
                if (WCM_ConnectAsync(gWifiSetupWcm.connectAp, gWifiSetupWcm.connectKey, gWifiSetupWcm.connectOption) != 3) {
                    gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            } else {
                gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
            }
        } else {
            gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_DISCONNECT_FAILED, 0);
            }
        }
        break;
    case 2:
        if (event->result == 0) {
            if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_CLEANUP) {
                WCM_Finish();
                gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_NONE;
                if (gWifiSetupWcm.callback) {
                    gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_CLEANED_UP, 0);
                }
            }
        } else {
            gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_IDLE;
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
            }
        }
        break;
    case 7:
        if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_SEARCHING) {
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_FOUND_AP, 0);
            }
        }
        break;
    default:
        if (gWifiSetupWcm.callback) {
            gWifiSetupWcm.callback(WIFI_SETUP_WCM_EVENT_UNKNOWN, 0);
        }
        break;
    }
}

// Starts the request that leads back to the idle state (stop the search, disconnect, or start up).
int WifiSetupWcm_ReturnToIdle(void) {
    switch (gWifiSetupWcm.state) {
    case WIFI_SETUP_WCM_STATE_SEARCHING:
        if (WCM_SearchAsync(NULL, NULL, 0) != 3) {
            return 0;
        }
        break;
    case WIFI_SETUP_WCM_STATE_CONNECTED:
        if (WCM_DisconnectAsync() != 3) {
            return 0;
        }
        break;
    case WIFI_SETUP_WCM_STATE_READY:
        if (WCM_StartupAsync(gWifiSetupWcm.config, WifiSetupWcm_HandleEvent) != 3) {
            return 0;
        }
        break;
    case WIFI_SETUP_WCM_STATE_NONE:
    case WIFI_SETUP_WCM_STATE_CLEANUP:
    case WIFI_SETUP_WCM_STATE_IDLE:
    case WIFI_SETUP_WCM_STATE_BUSY:
    case WIFI_SETUP_WCM_STATE_SEARCH_STARTING:
    case WIFI_SETUP_WCM_STATE_CONNECTING:
    default:
        return 0;
    }
    return 1;
}

// Copies up to max access point descriptions (0xC0 bytes each); returns how many were found.
int WifiSetupWcm_CopyApList(u8 *dst, int max) {
    int count;
    int i;
    WCM_LockApList(1);
    count = WCM_CountApList();
    if (count > 0) {
        i = 0;
        if (count > 0) {
            do {
                if (i >= max) {
                    break;
                }
                MIi_CpuCopy32(WCM_PointApList((u16)i), (u32 *)dst, 0xC0);
                i++;
                dst += 0xC0;
            } while (i < count);
        }
    }
    WCM_LockApList(0);
    return count;
}

int WifiSetupWcm_StartSearch(const u8 *bssid, const u8 *ssid, int len, int option) {
    OSIntrMode irq;
    int i;
    irq = OS_DisableInterrupts();
    gWifiSetupWcm.searchOption = option;
    if (bssid != NULL) {
        for (i = 0; i < 6; i++) {
            gWifiSetupWcmSearchBssid[i] = *bssid++;
        }
        gWifiSetupWcm.searchBssid = gWifiSetupWcmSearchBssid;
    } else {
        MI_CpuFill8(gWifiSetupWcmSearchBssid, 0xFF, 6);
        gWifiSetupWcm.searchBssid = WCM_Bssid_Any;
    }
    if (ssid != NULL && len > 0 && len <= 32) {
        for (i = 0; i < len; i++) {
            gWifiSetupWcmSearchSsid[i] = *ssid++;
        }
        for (; i < 32; i++) {
            gWifiSetupWcmSearchSsid[i] = 0;
        }
        gWifiSetupWcm.searchSsid = gWifiSetupWcmSearchSsid;
    } else {
        MI_CpuFill8(gWifiSetupWcmSearchSsid, 0xFF, 32);
        gWifiSetupWcm.searchSsid = WCM_Essid_Any;
    }
    if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_IDLE) {
        if (WCM_SearchAsync(gWifiSetupWcm.searchBssid, gWifiSetupWcm.searchSsid, gWifiSetupWcm.searchOption) == 3) {
            gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_SEARCH_STARTING;
            OS_RestoreInterrupts(irq);
            return 1;
        }
    } else if (WifiSetupWcm_ReturnToIdle() == 1) {
        gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_SEARCH_STARTING;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupWcm_StopSearch(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_SEARCHING && WCM_SearchAsync(NULL, NULL, 0) == 3) {
        gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_BUSY;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupWcm_StartDisconnect(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_CONNECTED && WCM_DisconnectAsync() == 3) {
        gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_BUSY;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupWcm_StartCleanup(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_IDLE) {
        if (WCM_CleanupAsync() != 3) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_CLEANUP;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (WifiSetupWcm_ReturnToIdle() == 1) {
        gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_CLEANUP;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupWcm_StartConnect(void *ap, void *key, int option) {
    OSIntrMode irq;
    irq = OS_DisableInterrupts();
    gWifiSetupWcm.connectOption = option;
    if (key != NULL) {
        MI_CpuCopy8(key, gWifiSetupWcm.connectKey, 0x50);
    } else {
        MI_CpuFill8(gWifiSetupWcm.connectKey, 0, 0x50);
    }
    MIi_CpuCopy32(ap, gWifiSetupWcm.connectAp, 0xC0);
    if (WifiSetupWcm_ReturnToIdle() == 1) {
        gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_CONNECTING;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_IDLE && WCM_ConnectAsync(gWifiSetupWcm.connectAp, gWifiSetupWcm.connectKey, gWifiSetupWcm.connectOption) == 3) {
        gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_CONNECTING;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

// Carves the key, start-up config, WCM work area and connect buffer out of allocation, then
// initialises WCM and starts it up.
BOOL WifiSetupWcm_Init(void (*callback)(int event, int arg), void *allocation, u32 size) {
    OSIntrMode irq;
    u32 cursor;
    u32 cursor2;

    irq = OS_DisableInterrupts();
    cursor = (u32)allocation;
    cursor += 0x53;
    cursor &= ~3;
    cursor2 = cursor;
    gWifiSetupWcm.connectKey = allocation;
    gWifiSetupWcm.config = (WifiSetupWcmConfig *)cursor;
    cursor2 += 0x2F;
    cursor2 &= ~0x1F;
    gWifiSetupWcm.wcmWork = (void *)cursor2;
    cursor2 += 0x231F;
    cursor2 &= ~0x1F;
    gWifiSetupWcm.connectAp = (void *)cursor2;
    cursor2 += 0xDF;
    cursor2 &= ~0x1F;
    gWifiSetupWcm.config->buffer = (void *)cursor2;
    gWifiSetupWcm.config->bufferSize = (u32)allocation + size - (u32)gWifiSetupWcm.config->buffer;
    gWifiSetupWcm.config->apListMode = 0;
    gWifiSetupWcm.config->dmaNo = 3;
    gWifiSetupWcm.callback = callback;
    if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_NONE) {
        if (WCM_Init(gWifiSetupWcm.wcmWork, 0x2300) != 0) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_READY;
    }
    if (gWifiSetupWcm.state == WIFI_SETUP_WCM_STATE_READY) {
        if (WCM_StartupAsync(gWifiSetupWcm.config, WifiSetupWcm_HandleEvent) != 3) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        gWifiSetupWcm.state = WIFI_SETUP_WCM_STATE_BUSY;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}
