#include "wifi_setup_wcm.h"

#include "global.h"

#include "wifi_setup.h"

extern WifiSetupWcm ov13_0224DEE0;
extern u8 ov13_0224DF08[6];
extern u8 ov13_0224DF10[32];
extern u8 _02108FC0[]; // BSSID ff:ff:ff:ff:ff:ff (any)
extern u8 _02108FC8[]; // SSID of 0xFF bytes (any)

void WifiSetup_Sleep(s32 ticks);
s32 ov13_02223100(s32 arg);
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

s32 ov13_02223100(s32 arg) {
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
            if (ov13_0224DEE0.state == 4) {
                ov13_0224DEE0.state = 3;
                if (ov13_0224DEE0.callback) {
                    ov13_0224DEE0.callback(6, 0);
                }
            } else if (ov13_0224DEE0.state == 6) {
                if (WCM_SearchAsync(ov13_0224DEE0.searchBssid, ov13_0224DEE0.searchSsid, ov13_0224DEE0.searchOption) != 3) {
                    ov13_0224DEE0.state = 3;
                    if (ov13_0224DEE0.callback) {
                        ov13_0224DEE0.callback(2, 0);
                    }
                }
            } else if (ov13_0224DEE0.state == 8) {
                if (WCM_ConnectAsync(ov13_0224DEE0.connectAp, ov13_0224DEE0.connectKey, ov13_0224DEE0.connectOption) != 3) {
                    ov13_0224DEE0.state = 3;
                    if (ov13_0224DEE0.callback) {
                        ov13_0224DEE0.callback(2, 0);
                    }
                }
            }
        } else {
            ov13_0224DEE0.state = 1;
            if (ov13_0224DEE0.callback) {
                ov13_0224DEE0.callback(2, 0);
            }
        }
        break;
    case 3:
        if (event->result == 0) {
            if (ov13_0224DEE0.state == 6) {
                ov13_0224DEE0.state = 5;
                if (ov13_0224DEE0.callback) {
                    ov13_0224DEE0.callback(8, 0);
                }
            }
        } else {
            ov13_0224DEE0.state = 3;
            if (ov13_0224DEE0.callback) {
                ov13_0224DEE0.callback(9, 0);
            }
        }
        break;
    case 5:
        if (event->result == 0) {
            if (ov13_0224DEE0.state == 8) {
                ov13_0224DEE0.state = 7;
                if (ov13_0224DEE0.callback) {
                    ov13_0224DEE0.callback(12, 0);
                }
            }
        } else {
            ov13_0224DEE0.state = 3;
            if (ov13_0224DEE0.callback) {
                ov13_0224DEE0.callback(13, 0);
            }
        }
        break;
    case 4:
        if (event->result == 0) {
            if (ov13_0224DEE0.state == 4) {
                ov13_0224DEE0.state = 3;
                if (ov13_0224DEE0.callback) {
                    ov13_0224DEE0.callback(10, 0);
                }
            } else if (ov13_0224DEE0.state == 6) {
                if (WCM_SearchAsync(ov13_0224DEE0.searchBssid, ov13_0224DEE0.searchSsid, ov13_0224DEE0.searchOption) != 3) {
                    ov13_0224DEE0.state = 3;
                    if (ov13_0224DEE0.callback) {
                        ov13_0224DEE0.callback(2, 0);
                    }
                }
            } else if (ov13_0224DEE0.state == 2) {
                if (WCM_CleanupAsync() != 3) {
                    ov13_0224DEE0.state = 3;
                    if (ov13_0224DEE0.callback) {
                        ov13_0224DEE0.callback(2, 0);
                    }
                }
            } else if (ov13_0224DEE0.state == 8) {
                if (WCM_ConnectAsync(ov13_0224DEE0.connectAp, ov13_0224DEE0.connectKey, ov13_0224DEE0.connectOption) != 3) {
                    ov13_0224DEE0.state = 3;
                    if (ov13_0224DEE0.callback) {
                        ov13_0224DEE0.callback(2, 0);
                    }
                }
            }
        } else {
            ov13_0224DEE0.state = 3;
            if (ov13_0224DEE0.callback) {
                ov13_0224DEE0.callback(11, 0);
            }
        }
        break;
    case 6:
        if (event->result == 0) {
            if (ov13_0224DEE0.state == 4) {
                ov13_0224DEE0.state = 3;
                if (ov13_0224DEE0.callback) {
                    ov13_0224DEE0.callback(14, 0);
                }
            } else if (ov13_0224DEE0.state == 6) {
                if (WCM_SearchAsync(ov13_0224DEE0.searchBssid, ov13_0224DEE0.searchSsid, ov13_0224DEE0.searchOption) != 3) {
                    ov13_0224DEE0.state = 3;
                    if (ov13_0224DEE0.callback) {
                        ov13_0224DEE0.callback(2, 0);
                    }
                }
            } else if (ov13_0224DEE0.state == 2) {
                if (WCM_CleanupAsync() != 3) {
                    ov13_0224DEE0.state = 3;
                    if (ov13_0224DEE0.callback) {
                        ov13_0224DEE0.callback(2, 0);
                    }
                }
            } else if (ov13_0224DEE0.state == 8) {
                if (WCM_ConnectAsync(ov13_0224DEE0.connectAp, ov13_0224DEE0.connectKey, ov13_0224DEE0.connectOption) != 3) {
                    ov13_0224DEE0.state = 3;
                    if (ov13_0224DEE0.callback) {
                        ov13_0224DEE0.callback(2, 0);
                    }
                }
            } else {
                ov13_0224DEE0.state = 3;
            }
        } else {
            ov13_0224DEE0.state = 3;
            if (ov13_0224DEE0.callback) {
                ov13_0224DEE0.callback(15, 0);
            }
        }
        break;
    case 2:
        if (event->result == 0) {
            if (ov13_0224DEE0.state == 2) {
                WCM_Finish();
                ov13_0224DEE0.state = 0;
                if (ov13_0224DEE0.callback) {
                    ov13_0224DEE0.callback(20, 0);
                }
            }
        } else {
            ov13_0224DEE0.state = 3;
            if (ov13_0224DEE0.callback) {
                ov13_0224DEE0.callback(2, 0);
            }
        }
        break;
    case 7:
        if (ov13_0224DEE0.state == 5) {
            if (ov13_0224DEE0.callback) {
                ov13_0224DEE0.callback(5, 0);
            }
        }
        break;
    default:
        if (ov13_0224DEE0.callback) {
            ov13_0224DEE0.callback(1, 0);
        }
        break;
    }
}

#pragma force_active reset

// Starts the request that leads back to the idle state (stop the search, disconnect, or start up).
int WifiSetupWcm_ReturnToIdle(void) {
    switch (ov13_0224DEE0.state) {
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
        if (WCM_StartupAsync(ov13_0224DEE0.config, WifiSetupWcm_HandleEvent) != 3) {
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
    ov13_0224DEE0.searchOption = option;
    if (bssid != NULL) {
        for (i = 0; i < 6; i++) {
            ov13_0224DF08[i] = *bssid++;
        }
        ov13_0224DEE0.searchBssid = ov13_0224DF08;
    } else {
        MI_CpuFill8(ov13_0224DF08, 0xFF, 6);
        ov13_0224DEE0.searchBssid = _02108FC0;
    }
    if (ssid != NULL && len > 0 && len <= 32) {
        for (i = 0; i < len; i++) {
            ov13_0224DF10[i] = *ssid++;
        }
        for (; i < 32; i++) {
            ov13_0224DF10[i] = 0;
        }
        ov13_0224DEE0.searchSsid = ov13_0224DF10;
    } else {
        MI_CpuFill8(ov13_0224DF10, 0xFF, 32);
        ov13_0224DEE0.searchSsid = _02108FC8;
    }
    if (ov13_0224DEE0.state == 3) {
        if (WCM_SearchAsync(ov13_0224DEE0.searchBssid, ov13_0224DEE0.searchSsid, ov13_0224DEE0.searchOption) == 3) {
            ov13_0224DEE0.state = 6;
            OS_RestoreInterrupts(irq);
            return 1;
        }
    } else if (WifiSetupWcm_ReturnToIdle() == 1) {
        ov13_0224DEE0.state = 6;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupWcm_StopSearch(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (ov13_0224DEE0.state == 5 && WCM_SearchAsync(NULL, NULL, 0) == 3) {
        ov13_0224DEE0.state = 4;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupWcm_StartDisconnect(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (ov13_0224DEE0.state == 7 && WCM_DisconnectAsync() == 3) {
        ov13_0224DEE0.state = 4;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupWcm_StartCleanup(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (ov13_0224DEE0.state == 3) {
        if (WCM_CleanupAsync() != 3) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        ov13_0224DEE0.state = 2;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (WifiSetupWcm_ReturnToIdle() == 1) {
        ov13_0224DEE0.state = 2;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupWcm_StartConnect(void *ap, void *key, int option) {
    OSIntrMode irq;
    irq = OS_DisableInterrupts();
    ov13_0224DEE0.connectOption = option;
    if (key != NULL) {
        MI_CpuCopy8(key, ov13_0224DEE0.connectKey, 0x50);
    } else {
        MI_CpuFill8(ov13_0224DEE0.connectKey, 0, 0x50);
    }
    MIi_CpuCopy32(ap, ov13_0224DEE0.connectAp, 0xC0);
    if (WifiSetupWcm_ReturnToIdle() == 1) {
        ov13_0224DEE0.state = 8;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (ov13_0224DEE0.state == 3 && WCM_ConnectAsync(ov13_0224DEE0.connectAp, ov13_0224DEE0.connectKey, ov13_0224DEE0.connectOption) == 3) {
        ov13_0224DEE0.state = 8;
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
    ov13_0224DEE0.connectKey = allocation;
    ov13_0224DEE0.config = (WifiSetupWcmConfig *)cursor;
    cursor2 += 0x2F;
    cursor2 &= ~0x1F;
    ov13_0224DEE0.wcmWork = (void *)cursor2;
    cursor2 += 0x231F;
    cursor2 &= ~0x1F;
    ov13_0224DEE0.connectAp = (void *)cursor2;
    cursor2 += 0xDF;
    cursor2 &= ~0x1F;
    ov13_0224DEE0.config->buffer = (void *)cursor2;
    ov13_0224DEE0.config->bufferSize = (u32)allocation + size - (u32)ov13_0224DEE0.config->buffer;
    ov13_0224DEE0.config->unk_0C = 0;
    ov13_0224DEE0.config->unk_00 = 3;
    ov13_0224DEE0.callback = callback;
    if (ov13_0224DEE0.state == 0) {
        if (WCM_Init(ov13_0224DEE0.wcmWork, 0x2300) != 0) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        ov13_0224DEE0.state = 1;
    }
    if (ov13_0224DEE0.state == 1) {
        if (WCM_StartupAsync(ov13_0224DEE0.config, WifiSetupWcm_HandleEvent) != 3) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        ov13_0224DEE0.state = 4;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}
