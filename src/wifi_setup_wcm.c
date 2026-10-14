#include "wifi_setup_wcm.h"

#include "global.h"

#include "wifi_setup.h"

extern WifiSetupWcm gWifiSetupWcm;
extern u8 gWifiSetupWcmSearchBssid[6];
extern u8 gWifiSetupWcmSearchSsid[32];
extern u8 WCM_Bssid_Any[]; // BSSID ff:ff:ff:ff:ff:ff (any)
extern u8 WCM_Essid_Any[]; // SSID of 0xFF bytes (any)

void WifiSetup_Sleep(s32 ticks);
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

#pragma force_active on

void WifiSetupWcm_HandleEvent(WifiSetupWcmEvent *event) {
    if (!event) {
        return;
    }
    switch (event->type) {
    case 1:
        if (event->result == 0) {
            if (gWifiSetupWcm.state == 4) {
                gWifiSetupWcm.state = 3;
                if (gWifiSetupWcm.callback) {
                    gWifiSetupWcm.callback(6, 0);
                }
            } else if (gWifiSetupWcm.state == 6) {
                if (WCM_SearchAsync(gWifiSetupWcm.searchBssid, gWifiSetupWcm.searchSsid, gWifiSetupWcm.searchOption) != 3) {
                    gWifiSetupWcm.state = 3;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(2, 0);
                    }
                }
            } else if (gWifiSetupWcm.state == 8) {
                if (WCM_ConnectAsync(gWifiSetupWcm.connectAp, gWifiSetupWcm.connectKey, gWifiSetupWcm.connectOption) != 3) {
                    gWifiSetupWcm.state = 3;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(2, 0);
                    }
                }
            }
        } else {
            gWifiSetupWcm.state = 1;
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(2, 0);
            }
        }
        break;
    case 3:
        if (event->result == 0) {
            if (gWifiSetupWcm.state == 6) {
                gWifiSetupWcm.state = 5;
                if (gWifiSetupWcm.callback) {
                    gWifiSetupWcm.callback(8, 0);
                }
            }
        } else {
            gWifiSetupWcm.state = 3;
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(9, 0);
            }
        }
        break;
    case 5:
        if (event->result == 0) {
            if (gWifiSetupWcm.state == 8) {
                gWifiSetupWcm.state = 7;
                if (gWifiSetupWcm.callback) {
                    gWifiSetupWcm.callback(12, 0);
                }
            }
        } else {
            gWifiSetupWcm.state = 3;
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(13, 0);
            }
        }
        break;
    case 4:
        if (event->result == 0) {
            if (gWifiSetupWcm.state == 4) {
                gWifiSetupWcm.state = 3;
                if (gWifiSetupWcm.callback) {
                    gWifiSetupWcm.callback(10, 0);
                }
            } else if (gWifiSetupWcm.state == 6) {
                if (WCM_SearchAsync(gWifiSetupWcm.searchBssid, gWifiSetupWcm.searchSsid, gWifiSetupWcm.searchOption) != 3) {
                    gWifiSetupWcm.state = 3;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(2, 0);
                    }
                }
            } else if (gWifiSetupWcm.state == 2) {
                if (WCM_CleanupAsync() != 3) {
                    gWifiSetupWcm.state = 3;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(2, 0);
                    }
                }
            } else if (gWifiSetupWcm.state == 8) {
                if (WCM_ConnectAsync(gWifiSetupWcm.connectAp, gWifiSetupWcm.connectKey, gWifiSetupWcm.connectOption) != 3) {
                    gWifiSetupWcm.state = 3;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(2, 0);
                    }
                }
            }
        } else {
            gWifiSetupWcm.state = 3;
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(11, 0);
            }
        }
        break;
    case 6:
        if (event->result == 0) {
            if (gWifiSetupWcm.state == 4) {
                gWifiSetupWcm.state = 3;
                if (gWifiSetupWcm.callback) {
                    gWifiSetupWcm.callback(14, 0);
                }
            } else if (gWifiSetupWcm.state == 6) {
                if (WCM_SearchAsync(gWifiSetupWcm.searchBssid, gWifiSetupWcm.searchSsid, gWifiSetupWcm.searchOption) != 3) {
                    gWifiSetupWcm.state = 3;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(2, 0);
                    }
                }
            } else if (gWifiSetupWcm.state == 2) {
                if (WCM_CleanupAsync() != 3) {
                    gWifiSetupWcm.state = 3;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(2, 0);
                    }
                }
            } else if (gWifiSetupWcm.state == 8) {
                if (WCM_ConnectAsync(gWifiSetupWcm.connectAp, gWifiSetupWcm.connectKey, gWifiSetupWcm.connectOption) != 3) {
                    gWifiSetupWcm.state = 3;
                    if (gWifiSetupWcm.callback) {
                        gWifiSetupWcm.callback(2, 0);
                    }
                }
            } else {
                gWifiSetupWcm.state = 3;
            }
        } else {
            gWifiSetupWcm.state = 3;
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(15, 0);
            }
        }
        break;
    case 2:
        if (event->result == 0) {
            if (gWifiSetupWcm.state == 2) {
                WCM_Finish();
                gWifiSetupWcm.state = 0;
                if (gWifiSetupWcm.callback) {
                    gWifiSetupWcm.callback(20, 0);
                }
            }
        } else {
            gWifiSetupWcm.state = 3;
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(2, 0);
            }
        }
        break;
    case 7:
        if (gWifiSetupWcm.state == 5) {
            if (gWifiSetupWcm.callback) {
                gWifiSetupWcm.callback(5, 0);
            }
        }
        break;
    default:
        if (gWifiSetupWcm.callback) {
            gWifiSetupWcm.callback(1, 0);
        }
        break;
    }
}

#pragma force_active reset

// Starts the request that leads back to the idle state (stop the search, disconnect, or start up).
int WifiSetupWcm_ReturnToIdle(void) {
    switch (gWifiSetupWcm.state) {
    case 5:
        if (WCM_SearchAsync(NULL, NULL, 0) != 3) {
            return 0;
        }
        break;
    case 7:
        if (WCM_DisconnectAsync() != 3) {
            return 0;
        }
        break;
    case 1:
        if (WCM_StartupAsync(gWifiSetupWcm.config, WifiSetupWcm_HandleEvent) != 3) {
            return 0;
        }
        break;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 8:
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
    if (gWifiSetupWcm.state == 3) {
        if (WCM_SearchAsync(gWifiSetupWcm.searchBssid, gWifiSetupWcm.searchSsid, gWifiSetupWcm.searchOption) == 3) {
            gWifiSetupWcm.state = 6;
            OS_RestoreInterrupts(irq);
            return 1;
        }
    } else if (WifiSetupWcm_ReturnToIdle() == 1) {
        gWifiSetupWcm.state = 6;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupWcm_StopSearch(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (gWifiSetupWcm.state == 5 && WCM_SearchAsync(NULL, NULL, 0) == 3) {
        gWifiSetupWcm.state = 4;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupWcm_StartDisconnect(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (gWifiSetupWcm.state == 7 && WCM_DisconnectAsync() == 3) {
        gWifiSetupWcm.state = 4;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupWcm_StartCleanup(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (gWifiSetupWcm.state == 3) {
        if (WCM_CleanupAsync() != 3) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        gWifiSetupWcm.state = 2;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (WifiSetupWcm_ReturnToIdle() == 1) {
        gWifiSetupWcm.state = 2;
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
        gWifiSetupWcm.state = 8;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (gWifiSetupWcm.state == 3 && WCM_ConnectAsync(gWifiSetupWcm.connectAp, gWifiSetupWcm.connectKey, gWifiSetupWcm.connectOption) == 3) {
        gWifiSetupWcm.state = 8;
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
    gWifiSetupWcm.config->unk_0C = 0;
    gWifiSetupWcm.config->unk_00 = 3;
    gWifiSetupWcm.callback = callback;
    if (gWifiSetupWcm.state == 0) {
        if (WCM_Init(gWifiSetupWcm.wcmWork, 0x2300) != 0) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        gWifiSetupWcm.state = 1;
    }
    if (gWifiSetupWcm.state == 1) {
        if (WCM_StartupAsync(gWifiSetupWcm.config, WifiSetupWcm_HandleEvent) != 3) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        gWifiSetupWcm.state = 4;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}
