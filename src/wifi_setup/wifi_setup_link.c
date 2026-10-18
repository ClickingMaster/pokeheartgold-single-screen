#include "global.h"

#include "wifi_setup/wifi_setup_internal.h"
#include "wifi_setup/wifi_setup_wcm.h"

// Second copy of the WCM wrapper, on the worker's state gWifiSetupWork, plus
// the worker's link steps: open (start WCM), connect to the chosen access point and start the
// socket library, disconnect, close. WCM events are queued (gWifiSetupLinkEventQueue) and polled by the worker.

extern void (*gWifiSetupLinkEventCallback)(u32 event, int arg);
extern u32 gWifiSetupLinkEventQueue[4];
extern OSMutex gWifiSetupLinkMutex;
extern u8 gWifiSetupLinkSearchBssid[6];
extern u8 gWifiSetupLinkSearchSsid[32];
extern u8 sWifiSetupLinkSocConfig[]; // socket library configuration
extern u8 WCM_Bssid_Any[];           // BSSID ff:ff:ff:ff:ff:ff (any)
extern u8 WCM_Essid_Any[];           // SSID of 0xFF bytes (any)

static void WifiSetupLink_HandleEvent(WifiSetupWcmEvent *event);
static int WifiSetupLink_ReturnToIdle(void);
static int WifiSetupLink_StartDisconnect(void);
static int WifiSetupLink_StartCleanup(void);
static int WifiSetupLink_StartConnect(void *ap, void *key, int option);
static int WifiSetupLink_Init(void (*eventCallback)(u32 event, int arg), u8 *buffer, int size);
static void WifiSetupLink_ClearEvents(void);
static void WifiSetupLink_PushEvent(u32 event, int unused);
static inline u32 AlignDown32(u32 value);

int SOC_Startup(void *config);
void SOC_Cleanup(void);

static void WifiSetupLink_HandleEvent(WifiSetupWcmEvent *event) {
    if (!event) {
        return;
    }
    switch (event->type) {
    case 1:
        if (event->result == 0) {
            if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_BUSY) {
                gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
                if (gWifiSetupLinkEventCallback) {
                    gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_STARTED, 0);
                }
            } else if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_SEARCH_STARTING) {
                if (WCM_SearchAsync(gWifiSetupWork.searchBssid, gWifiSetupWork.searchSsid, gWifiSetupWork.searchOption) != 3) {
                    gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupLinkEventCallback) {
                        gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_CONNECTING) {
                if (WCM_ConnectAsync(gWifiSetupWork.connectAp, gWifiSetupWork.connectKey, gWifiSetupWork.connectOption) != 3) {
                    gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupLinkEventCallback) {
                        gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            }
        } else {
            gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_READY;
            if (gWifiSetupLinkEventCallback) {
                gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
            }
        }
        break;
    case 3:
        if (event->result == 0) {
            if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_SEARCH_STARTING) {
                gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_SEARCHING;
                if (gWifiSetupLinkEventCallback) {
                    gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_SEARCH_STARTED, 0);
                }
            }
        } else {
            gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
            if (gWifiSetupLinkEventCallback) {
                gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_SEARCH_FAILED, 0);
            }
        }
        break;
    case 5:
        if (event->result == 0) {
            if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_CONNECTING) {
                gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_CONNECTED;
                if (gWifiSetupLinkEventCallback) {
                    gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_CONNECTED, 0);
                }
            }
        } else {
            gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
            if (gWifiSetupLinkEventCallback) {
                gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_CONNECT_FAILED, 0);
            }
        }
        break;
    case 4:
        if (event->result == 0) {
            if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_BUSY) {
                gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
                if (gWifiSetupLinkEventCallback) {
                    gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_SEARCH_ENDED, 0);
                }
            } else if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_SEARCH_STARTING) {
                if (WCM_SearchAsync(gWifiSetupWork.searchBssid, gWifiSetupWork.searchSsid, gWifiSetupWork.searchOption) != 3) {
                    gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupLinkEventCallback) {
                        gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_CLEANUP) {
                if (WCM_CleanupAsync() != 3) {
                    gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupLinkEventCallback) {
                        gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_CONNECTING) {
                if (WCM_ConnectAsync(gWifiSetupWork.connectAp, gWifiSetupWork.connectKey, gWifiSetupWork.connectOption) != 3) {
                    gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupLinkEventCallback) {
                        gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            }
        } else {
            gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
            if (gWifiSetupLinkEventCallback) {
                gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_SEARCH_END_FAILED, 0);
            }
        }
        break;
    case 6:
        if (event->result == 0) {
            if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_BUSY) {
                gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
                if (gWifiSetupLinkEventCallback) {
                    gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_DISCONNECTED, 0);
                }
            } else if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_SEARCH_STARTING) {
                if (WCM_SearchAsync(gWifiSetupWork.searchBssid, gWifiSetupWork.searchSsid, gWifiSetupWork.searchOption) != 3) {
                    gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupLinkEventCallback) {
                        gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_CLEANUP) {
                if (WCM_CleanupAsync() != 3) {
                    gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupLinkEventCallback) {
                        gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_CONNECTING) {
                if (WCM_ConnectAsync(gWifiSetupWork.connectAp, gWifiSetupWork.connectKey, gWifiSetupWork.connectOption) != 3) {
                    gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
                    if (gWifiSetupLinkEventCallback) {
                        gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_CONNECTED) {
                gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
            }
        } else {
            gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
            if (gWifiSetupLinkEventCallback) {
                gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_DISCONNECT_FAILED, 0);
            }
        }
        break;
    case 2:
        if (event->result == 0) {
            if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_CLEANUP) {
                WCM_Finish();
                gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_NONE;
                if (gWifiSetupLinkEventCallback) {
                    gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_CLEANED_UP, 0);
                }
            }
        } else {
            gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_IDLE;
            if (gWifiSetupLinkEventCallback) {
                gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_FAILED, 0);
            }
        }
        break;
    case 7:
        if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_SEARCHING) {
            if (gWifiSetupLinkEventCallback) {
                gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_FOUND_AP, 0);
            }
        }
        break;
    case 8:
        if (gWifiSetupLinkEventCallback) {
            gWifiSetupLinkEventCallback(4, 0);
        }
        break;
    case 9:
        gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_NONE;
        if (gWifiSetupLinkEventCallback) {
            gWifiSetupLinkEventCallback(3, 0);
        }
        break;
    default:
        if (gWifiSetupLinkEventCallback) {
            gWifiSetupLinkEventCallback(WIFI_SETUP_WCM_EVENT_UNKNOWN, 0);
        }
        break;
    }
}

// Starts the request that leads back to the idle state (stop the search, disconnect, or start up).
static int WifiSetupLink_ReturnToIdle(void) {
    switch (gWifiSetupWork.wcmState) {
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
        if (WCM_StartupAsync(gWifiSetupWork.wcmConfig, WifiSetupLink_HandleEvent) != 3) {
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

// Copies up to max access point descriptions; returns how many were found.
int WifiSetupLink_CopyApList(WifiSetupApInfo *dst, int max) {
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
                MIi_CpuCopy32(WCM_PointApList((u16)i), (u32 *)dst, sizeof(WifiSetupApInfo));
                i++;
                dst++;
            } while (i < count);
        }
    }
    WCM_LockApList(0);
    return count;
}

int WifiSetupLink_StartSearch(const u8 *bssid, const u8 *ssid, int len, int option) {
    OSIntrMode irq;
    int i;
    irq = OS_DisableInterrupts();
    gWifiSetupWork.searchOption = option;
    gWifiSetupWork.searchBssid = gWifiSetupLinkSearchBssid;
    if (bssid != NULL) {
        for (i = 0; i < 6; i++) {
            gWifiSetupLinkSearchBssid[i] = *bssid++;
        }
    } else {
        MI_CpuFill8(gWifiSetupLinkSearchBssid, 0xFF, 6);
        gWifiSetupWork.searchBssid = WCM_Bssid_Any;
    }
    gWifiSetupWork.searchSsid = gWifiSetupLinkSearchSsid;
    if (ssid != NULL && len > 0 && len < 32) {
        for (i = 0; i < len; i++) {
            gWifiSetupLinkSearchSsid[i] = *ssid++;
        }
        for (; i < 32; i++) {
            gWifiSetupLinkSearchSsid[i] = 0;
        }
    } else {
        MI_CpuFill8(gWifiSetupLinkSearchSsid, 0xFF, 32);
        gWifiSetupWork.searchSsid = WCM_Essid_Any;
    }
    if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_IDLE) {
        if (WCM_SearchAsync(gWifiSetupLinkSearchBssid, gWifiSetupWork.searchSsid, gWifiSetupWork.searchOption) == 3) {
            gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_SEARCH_STARTING;
            OS_RestoreInterrupts(irq);
            return 1;
        }
    } else if (WifiSetupLink_ReturnToIdle() == 1) {
        gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_SEARCH_STARTING;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

static int WifiSetupLink_StartDisconnect(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_CONNECTED && WCM_DisconnectAsync() == 3) {
        gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_BUSY;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

static int WifiSetupLink_StartCleanup(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_IDLE) {
        if (WCM_CleanupAsync() != 3) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_CLEANUP;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (WifiSetupLink_ReturnToIdle() == 1) {
        gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_CLEANUP;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

static int WifiSetupLink_StartConnect(void *ap, void *key, int option) {
    OSIntrMode irq = OS_DisableInterrupts();
    gWifiSetupWork.connectOption = option;
    if (key) {
        MI_CpuCopy8(key, gWifiSetupWork.connectKey, 0x60);
    } else {
        MI_CpuFill8(gWifiSetupWork.connectKey, 0, 0x60);
    }
    MIi_CpuCopy32(ap, gWifiSetupWork.connectAp, sizeof(WifiSetupApInfo));
    if (WifiSetupLink_ReturnToIdle() == 1) {
        gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_CONNECTING;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_IDLE) {
        if (WCM_ConnectAsync(gWifiSetupWork.connectAp, gWifiSetupWork.connectKey, gWifiSetupWork.connectOption) == 3) {
            gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_CONNECTING;
            OS_RestoreInterrupts(irq);
            return 1;
        }
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

// Carves the key, start-up config, WCM work area and connect buffer out of buffer, then initialises
// WCM and starts it up; WCM events go to eventCallback.
static int WifiSetupLink_Init(void (*eventCallback)(u32 event, int arg), u8 *buffer, int size) {
    OSIntrMode irq = OS_DisableInterrupts();
    u32 cursor = ((u32)buffer + 99) & ~3;
    gWifiSetupWork.connectKey = buffer;
    gWifiSetupWork.wcmConfig = (WifiSetupWcmConfig *)cursor;
    cursor = (cursor + 47) & ~31;
    gWifiSetupWork.wcmWork = (void *)cursor;
    cursor = (cursor + 8991) & ~31;
    gWifiSetupWork.connectAp = (void *)cursor;
    cursor = (cursor + 223) & ~31;
    gWifiSetupWork.wcmConfig->buffer = (void *)cursor;
    gWifiSetupWork.wcmConfig->bufferSize = (buffer + size) - (u8 *)gWifiSetupWork.wcmConfig->buffer;
    gWifiSetupWork.wcmConfig->apListMode = 0;
    gWifiSetupWork.wcmConfig->dmaNo = 3;
    gWifiSetupLinkEventCallback = eventCallback;
    if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_NONE) {
        if (WCM_Init(gWifiSetupWork.wcmWork, 8960) != 0) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_READY;
    }
    if (gWifiSetupWork.wcmState == WIFI_SETUP_WCM_STATE_READY) {
        if (WCM_StartupAsync(gWifiSetupWork.wcmConfig, WifiSetupLink_HandleEvent) != 3) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        gWifiSetupWork.wcmState = WIFI_SETUP_WCM_STATE_BUSY;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupLink_GetState(void) {
    return gWifiSetupWork.wcmState;
}

void *WifiSetupLink_Alloc(int unused, int size) {
    void *result;
    if (size > 0) {
        OS_LockMutex(&gWifiSetupLinkMutex);
        result = gWifiSetupWork.alloc(size);
        OS_UnlockMutex(&gWifiSetupLinkMutex);
        return result;
    }
    return NULL;
}

void WifiSetupLink_Free(int unused, void *ptr, int size) {
    if (ptr != NULL && size > 0) {
        OS_LockMutex(&gWifiSetupLinkMutex);
        gWifiSetupWork.free(ptr);
        OS_UnlockMutex(&gWifiSetupLinkMutex);
    }
}

static void WifiSetupLink_ClearEvents(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    int i;
    gWifiSetupWork.eventWrite = 0;
    gWifiSetupWork.eventRead = 0;
    for (i = 0; i < 4; i++) {
        gWifiSetupLinkEventQueue[i] = 0;
    }
    OS_RestoreInterrupts(irq);
}

// Returns the oldest queued event, 0 when the queue is empty.
u32 WifiSetupLink_PopEvent(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    int read = gWifiSetupWork.eventRead;
    u32 result;
    if (gWifiSetupWork.eventWrite == read) {
        result = 0;
    } else {
        result = gWifiSetupLinkEventQueue[read];
        gWifiSetupWork.eventRead = read + 1;
        if (gWifiSetupWork.eventRead >= 4) {
            gWifiSetupWork.eventRead = 0;
        }
    }
    OS_RestoreInterrupts(irq);
    return result;
}

// WCM event callback: queues the event unless the queue is full.
static void WifiSetupLink_PushEvent(u32 event, int unused) {
    int write = gWifiSetupWork.eventWrite;
    int read = gWifiSetupWork.eventRead;
    int next = write + 1;
    if (next != read && write != read + 3) {
        gWifiSetupLinkEventQueue[write] = event;
        gWifiSetupWork.eventWrite = next;
        if (next >= 4) {
            gWifiSetupWork.eventWrite = 0;
        }
    }
}

// Alarm handler: queues a timeout event.
void WifiSetupLink_PostEvent(u32 event) {
    WifiSetupLink_PushEvent(event, 0);
}

// The mask is computed at run time (-32 as a 16-bit value) so the alignment keeps its instruction
// sequence.
static inline u32 AlignDown32(u32 value) {
    s16 m1 = 31;
    return value & (m1 - 63);
}

#pragma push
#pragma opt_foldconstants off
#pragma opt_loop_invariants off
#pragma opt_strength_reduction off

// Allocates the WCM buffers and room for count access points, starts WCM and waits until it is up.
// Returns 1, -1 (no memory) or -2.
int WifiSetupLink_Open(u32 count) {
    u16 m = 31;
    s16 k = -32;
    int event;
    int running = TRUE;
    int result;
    u32 size;
    u32 aligned;
    u32 apList;
    gWifiSetupWork.apListCount = count;
    WifiSetupLink_ClearEvents();
    size = count * 0xD0;
    gWifiSetupWork.allocation = gWifiSetupWork.alloc(size + 0x24D0 + count * 0xC0);
    if (!gWifiSetupWork.allocation) {
        return -1;
    }
    aligned = AlignDown32((u32)gWifiSetupWork.allocation + m);
    apList = aligned + (size + 0x2490);
    apList += m;
    apList &= k;
    gWifiSetupWork.buffer = (void *)aligned;
    gWifiSetupWork.apList = (WifiSetupApInfo *)apList;
    if (!WifiSetupLink_Init(WifiSetupLink_PushEvent, (u8 *)aligned, size + 0x2490)) {
        return -2;
    }
    do {
        OS_Sleep(10);
        event = WifiSetupLink_PopEvent();
        while (event != 0) {
            if (event != 4 && event != 5) {
                if (event == 6) {
                    running = FALSE;
                    result = 1;
                } else {
                    running = FALSE;
                    result = -2;
                }
            }
            event = WifiSetupLink_PopEvent();
        }
    } while (running);
    return result;
}

#pragma pop

// Shuts WCM down (waits for event 20) and frees the buffers.
int WifiSetupLink_Close(void) {
    int running = 1;

    if (WifiSetupLink_StartCleanup()) {
        int zero = 0;
        int keepGoing = zero;
        do {
            OS_Sleep(10);
            int event = WifiSetupLink_PopEvent();
            while (event != 0) {
                switch (event) {
                case 4:
                case WIFI_SETUP_WCM_EVENT_FOUND_AP:
                    break;
                case WIFI_SETUP_WCM_EVENT_CLEANED_UP:
                    running = keepGoing;
                    break;
                default:
                    running = zero;
                    break;
                }
                event = WifiSetupLink_PopEvent();
            }
        } while (running != 0);
    }

    if (gWifiSetupWork.allocation != NULL) {
        WifiSetupWorkFreeFunc freeFunc = gWifiSetupWork.free;
        freeFunc(gWifiSetupWork.allocation);
        gWifiSetupWork.allocation = NULL;
    }
    return 1;
}

// Connects to the selected access point (retrying after event 13), then starts the socket library.
// Returns 1, 0 (no access point), -2, -3 (deadline passed) or -8 (stopped).
int WifiSetupLink_ConnectSelectedAp(void) {
    OSAlarm alarm;
    int active = 1;
    int result = -2;
    int event;
    WifiSetupApInfo *ap = &gWifiSetupWork.apList[gWifiSetupWork.selection];
    if (!ap) {
        return 0;
    }
    if (!WifiSetupLink_StartConnect(ap, NULL, 0x30000)) {
        return -2;
    }
    OS_CreateAlarm(&alarm);
    OS_SetAlarm(&alarm, 0x3FEC42, (OSAlarmHandler)WifiSetupLink_PostEvent, (void *)18);
    do {
        if (WifiSetup_GetMilliseconds() >= gWifiSetupConfig.deadline) {
            result = -3;
            break;
        }
        if (gWifiSetupWork.stop) {
            result = -8;
            break;
        }
        OS_Sleep(10);
        event = WifiSetupLink_PopEvent();
        while (event) {
            switch (event) {
            case 4:
            case WIFI_SETUP_WCM_EVENT_FOUND_AP:
            case 18:
            case 19:
                break;
            case WIFI_SETUP_WCM_EVENT_CONNECTED:
                active = 0;
                result = 1;
                break;
            case WIFI_SETUP_WCM_EVENT_CONNECT_FAILED:
                if (gWifiSetupWork.stop) {
                    active = 0;
                    result = -8;
                } else if (!WifiSetupLink_StartConnect(ap, NULL, 0x30000)) {
                    return result;
                }
                break;
            default:
                active = 0;
                break;
            }
            event = WifiSetupLink_PopEvent();
        }
    } while (active);
    OS_CancelAlarm(&alarm);
    while (WifiSetupLink_PopEvent()) {}
    if (result > 0) {
        gWifiSetupWork.connected = 1;
        if (SOC_Startup(sWifiSetupLinkSocConfig) < 0) {
            result = -2;
        } else {
            gWifiSetupWork.socketStarted = 1;
        }
    }
    return result;
}

// Disconnects (waits for event 14) and stops the socket library.
void WifiSetupLink_DisconnectAp(void) {
    int running = 1;

    if (gWifiSetupWork.connected != 0) {
        if (WifiSetupLink_StartDisconnect()) {
            int zero = 0;
            int keepGoing = zero;
            do {
                OS_Sleep(10);
                int event = WifiSetupLink_PopEvent();
                while (event != 0) {
                    switch (event) {
                    case 4:
                    case WIFI_SETUP_WCM_EVENT_FOUND_AP:
                        break;
                    case WIFI_SETUP_WCM_EVENT_DISCONNECTED:
                        running = keepGoing;
                        break;
                    default:
                        running = zero;
                        break;
                    }
                    event = WifiSetupLink_PopEvent();
                }
            } while (running != 0);
        }
        gWifiSetupWork.connected = 0;
    }
    if (gWifiSetupWork.socketStarted != 0) {
        gWifiSetupWork.socketStarted = 0;
        SOC_Cleanup();
    }
}
