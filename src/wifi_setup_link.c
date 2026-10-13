#include "global.h"

#include "wifi_setup_internal.h"
#include "wifi_setup_wcm.h"

// Second copy of the WCM wrapper (see wifi_setup_wcm.h), on the worker's state ov13_0224DF30, plus
// the worker's link steps: open (start WCM), connect to the chosen access point and start the
// socket library, disconnect, close. WCM events are queued (ov13_0224DFB4) and polled by the worker.

extern void (*ov13_0224DFB0)(u32 event, int arg);
extern u32 ov13_0224DFB4[4];
extern OSMutex ov13_0224DFC4;
extern u8 ov13_0224E3E8[6];
extern u8 ov13_0224E420[32];
extern u8 ov13_02245A6C[]; // socket library configuration
extern u8 _02108FC0[];     // BSSID ff:ff:ff:ff:ff:ff (any)
extern u8 _02108FC8[];     // SSID of 0xFF bytes (any)

int SOC_Startup(void *config);
void SOC_Cleanup(void);

#pragma force_active on

void WifiSetupLink_HandleEvent(WifiSetupWcmEvent *event) {
    if (!event) {
        return;
    }
    switch (event->type) {
    case 1:
        if (event->result == 0) {
            if (ov13_0224DF30.wcmState == 4) {
                ov13_0224DF30.wcmState = 3;
                if (ov13_0224DFB0) {
                    ov13_0224DFB0(6, 0);
                }
            } else if (ov13_0224DF30.wcmState == 6) {
                if (WCM_SearchAsync(ov13_0224DF30.searchBssid, ov13_0224DF30.searchSsid, ov13_0224DF30.searchOption) != 3) {
                    ov13_0224DF30.wcmState = 3;
                    if (ov13_0224DFB0) {
                        ov13_0224DFB0(2, 0);
                    }
                }
            } else if (ov13_0224DF30.wcmState == 8) {
                if (WCM_ConnectAsync(ov13_0224DF30.connectAp, ov13_0224DF30.connectKey, ov13_0224DF30.connectOption) != 3) {
                    ov13_0224DF30.wcmState = 3;
                    if (ov13_0224DFB0) {
                        ov13_0224DFB0(2, 0);
                    }
                }
            }
        } else {
            ov13_0224DF30.wcmState = 1;
            if (ov13_0224DFB0) {
                ov13_0224DFB0(2, 0);
            }
        }
        break;
    case 3:
        if (event->result == 0) {
            if (ov13_0224DF30.wcmState == 6) {
                ov13_0224DF30.wcmState = 5;
                if (ov13_0224DFB0) {
                    ov13_0224DFB0(8, 0);
                }
            }
        } else {
            ov13_0224DF30.wcmState = 3;
            if (ov13_0224DFB0) {
                ov13_0224DFB0(9, 0);
            }
        }
        break;
    case 5:
        if (event->result == 0) {
            if (ov13_0224DF30.wcmState == 8) {
                ov13_0224DF30.wcmState = 7;
                if (ov13_0224DFB0) {
                    ov13_0224DFB0(12, 0);
                }
            }
        } else {
            ov13_0224DF30.wcmState = 3;
            if (ov13_0224DFB0) {
                ov13_0224DFB0(13, 0);
            }
        }
        break;
    case 4:
        if (event->result == 0) {
            if (ov13_0224DF30.wcmState == 4) {
                ov13_0224DF30.wcmState = 3;
                if (ov13_0224DFB0) {
                    ov13_0224DFB0(10, 0);
                }
            } else if (ov13_0224DF30.wcmState == 6) {
                if (WCM_SearchAsync(ov13_0224DF30.searchBssid, ov13_0224DF30.searchSsid, ov13_0224DF30.searchOption) != 3) {
                    ov13_0224DF30.wcmState = 3;
                    if (ov13_0224DFB0) {
                        ov13_0224DFB0(2, 0);
                    }
                }
            } else if (ov13_0224DF30.wcmState == 2) {
                if (WCM_CleanupAsync() != 3) {
                    ov13_0224DF30.wcmState = 3;
                    if (ov13_0224DFB0) {
                        ov13_0224DFB0(2, 0);
                    }
                }
            } else if (ov13_0224DF30.wcmState == 8) {
                if (WCM_ConnectAsync(ov13_0224DF30.connectAp, ov13_0224DF30.connectKey, ov13_0224DF30.connectOption) != 3) {
                    ov13_0224DF30.wcmState = 3;
                    if (ov13_0224DFB0) {
                        ov13_0224DFB0(2, 0);
                    }
                }
            }
        } else {
            ov13_0224DF30.wcmState = 3;
            if (ov13_0224DFB0) {
                ov13_0224DFB0(11, 0);
            }
        }
        break;
    case 6:
        if (event->result == 0) {
            if (ov13_0224DF30.wcmState == 4) {
                ov13_0224DF30.wcmState = 3;
                if (ov13_0224DFB0) {
                    ov13_0224DFB0(14, 0);
                }
            } else if (ov13_0224DF30.wcmState == 6) {
                if (WCM_SearchAsync(ov13_0224DF30.searchBssid, ov13_0224DF30.searchSsid, ov13_0224DF30.searchOption) != 3) {
                    ov13_0224DF30.wcmState = 3;
                    if (ov13_0224DFB0) {
                        ov13_0224DFB0(2, 0);
                    }
                }
            } else if (ov13_0224DF30.wcmState == 2) {
                if (WCM_CleanupAsync() != 3) {
                    ov13_0224DF30.wcmState = 3;
                    if (ov13_0224DFB0) {
                        ov13_0224DFB0(2, 0);
                    }
                }
            } else if (ov13_0224DF30.wcmState == 8) {
                if (WCM_ConnectAsync(ov13_0224DF30.connectAp, ov13_0224DF30.connectKey, ov13_0224DF30.connectOption) != 3) {
                    ov13_0224DF30.wcmState = 3;
                    if (ov13_0224DFB0) {
                        ov13_0224DFB0(2, 0);
                    }
                }
            } else if (ov13_0224DF30.wcmState == 7) {
                ov13_0224DF30.wcmState = 3;
            }
        } else {
            ov13_0224DF30.wcmState = 3;
            if (ov13_0224DFB0) {
                ov13_0224DFB0(15, 0);
            }
        }
        break;
    case 2:
        if (event->result == 0) {
            if (ov13_0224DF30.wcmState == 2) {
                WCM_Finish();
                ov13_0224DF30.wcmState = 0;
                if (ov13_0224DFB0) {
                    ov13_0224DFB0(20, 0);
                }
            }
        } else {
            ov13_0224DF30.wcmState = 3;
            if (ov13_0224DFB0) {
                ov13_0224DFB0(2, 0);
            }
        }
        break;
    case 7:
        if (ov13_0224DF30.wcmState == 5) {
            if (ov13_0224DFB0) {
                ov13_0224DFB0(5, 0);
            }
        }
        break;
    case 8:
        if (ov13_0224DFB0) {
            ov13_0224DFB0(4, 0);
        }
        break;
    case 9:
        ov13_0224DF30.wcmState = 0;
        if (ov13_0224DFB0) {
            ov13_0224DFB0(3, 0);
        }
        break;
    default:
        if (ov13_0224DFB0) {
            ov13_0224DFB0(1, 0);
        }
        break;
    }
}

#pragma force_active reset

// Starts the request that leads back to the idle state (stop the search, disconnect, or start up).
int WifiSetupLink_ReturnToIdle(void) {
    switch (ov13_0224DF30.wcmState) {
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
        if (WCM_StartupAsync(ov13_0224DF30.wcmConfig, WifiSetupLink_HandleEvent) != 3) {
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
    ov13_0224DF30.searchOption = option;
    ov13_0224DF30.searchBssid = ov13_0224E3E8;
    if (bssid != NULL) {
        for (i = 0; i < 6; i++) {
            ov13_0224E3E8[i] = *bssid++;
        }
    } else {
        MI_CpuFill8(ov13_0224E3E8, 0xFF, 6);
        ov13_0224DF30.searchBssid = _02108FC0;
    }
    ov13_0224DF30.searchSsid = ov13_0224E420;
    if (ssid != NULL && len > 0 && len < 32) {
        for (i = 0; i < len; i++) {
            ov13_0224E420[i] = *ssid++;
        }
        for (; i < 32; i++) {
            ov13_0224E420[i] = 0;
        }
    } else {
        MI_CpuFill8(ov13_0224E420, 0xFF, 32);
        ov13_0224DF30.searchSsid = _02108FC8;
    }
    if (ov13_0224DF30.wcmState == 3) {
        if (WCM_SearchAsync(ov13_0224E3E8, ov13_0224DF30.searchSsid, ov13_0224DF30.searchOption) == 3) {
            ov13_0224DF30.wcmState = 6;
            OS_RestoreInterrupts(irq);
            return 1;
        }
    } else if (WifiSetupLink_ReturnToIdle() == 1) {
        ov13_0224DF30.wcmState = 6;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupLink_StartDisconnect(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (ov13_0224DF30.wcmState == 7 && WCM_DisconnectAsync() == 3) {
        ov13_0224DF30.wcmState = 4;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

int WifiSetupLink_StartCleanup(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (ov13_0224DF30.wcmState == 3) {
        if (WCM_CleanupAsync() != 3) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        ov13_0224DF30.wcmState = 2;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (WifiSetupLink_ReturnToIdle() == 1) {
        ov13_0224DF30.wcmState = 2;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

#pragma force_active on

int WifiSetupLink_StartConnect(void *ap, void *key, int option) {
    OSIntrMode irq = OS_DisableInterrupts();
    ov13_0224DF30.connectOption = option;
    if (key) {
        MI_CpuCopy8(key, ov13_0224DF30.connectKey, 0x60);
    } else {
        MI_CpuFill8(ov13_0224DF30.connectKey, 0, 0x60);
    }
    MIi_CpuCopy32(ap, ov13_0224DF30.connectAp, sizeof(WifiSetupApInfo));
    if (WifiSetupLink_ReturnToIdle() == 1) {
        ov13_0224DF30.wcmState = 8;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (ov13_0224DF30.wcmState == 3) {
        if (WCM_ConnectAsync(ov13_0224DF30.connectAp, ov13_0224DF30.connectKey, ov13_0224DF30.connectOption) == 3) {
            ov13_0224DF30.wcmState = 8;
            OS_RestoreInterrupts(irq);
            return 1;
        }
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

// Carves the key, start-up config, WCM work area and connect buffer out of buffer, then initialises
// WCM and starts it up; WCM events go to eventCallback.
int WifiSetupLink_Init(void (*eventCallback)(u32 event, int arg), u8 *buffer, int size) {
    OSIntrMode irq = OS_DisableInterrupts();
    u32 cursor = ((u32)buffer + 99) & ~3;
    ov13_0224DF30.connectKey = buffer;
    ov13_0224DF30.wcmConfig = (WifiSetupWcmConfig *)cursor;
    cursor = (cursor + 47) & ~31;
    ov13_0224DF30.wcmWork = (void *)cursor;
    cursor = (cursor + 8991) & ~31;
    ov13_0224DF30.connectAp = (void *)cursor;
    cursor = (cursor + 223) & ~31;
    ov13_0224DF30.wcmConfig->buffer = (void *)cursor;
    ov13_0224DF30.wcmConfig->bufferSize = (buffer + size) - (u8 *)ov13_0224DF30.wcmConfig->buffer;
    ov13_0224DF30.wcmConfig->unk_0C = 0;
    ov13_0224DF30.wcmConfig->unk_00 = 3;
    ov13_0224DFB0 = eventCallback;
    if (ov13_0224DF30.wcmState == 0) {
        if (WCM_Init(ov13_0224DF30.wcmWork, 8960) != 0) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        ov13_0224DF30.wcmState = 1;
    }
    if (ov13_0224DF30.wcmState == 1) {
        if (WCM_StartupAsync(ov13_0224DF30.wcmConfig, WifiSetupLink_HandleEvent) != 3) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        ov13_0224DF30.wcmState = 4;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

#pragma force_active reset

int WifiSetupLink_GetState(void) {
    return ov13_0224DF30.wcmState;
}

void *WifiSetupLink_Alloc(int unused, int size) {
    void *result;
    if (size > 0) {
        OS_LockMutex(&ov13_0224DFC4);
        result = ov13_0224DF30.alloc(size);
        OS_UnlockMutex(&ov13_0224DFC4);
        return result;
    }
    return NULL;
}

void WifiSetupLink_Free(int unused, void *ptr, int size) {
    if (ptr != NULL && size > 0) {
        OS_LockMutex(&ov13_0224DFC4);
        ov13_0224DF30.free(ptr);
        OS_UnlockMutex(&ov13_0224DFC4);
    }
}

void WifiSetupLink_ClearEvents(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    int i;
    ov13_0224DF30.eventWrite = 0;
    ov13_0224DF30.eventRead = 0;
    for (i = 0; i < 4; i++) {
        ov13_0224DFB4[i] = 0;
    }
    OS_RestoreInterrupts(irq);
}

// Returns the oldest queued event, 0 when the queue is empty.
u32 WifiSetupLink_PopEvent(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    int read = ov13_0224DF30.eventRead;
    u32 result;
    if (ov13_0224DF30.eventWrite == read) {
        result = 0;
    } else {
        result = ov13_0224DFB4[read];
        ov13_0224DF30.eventRead = read + 1;
        if (ov13_0224DF30.eventRead >= 4) {
            ov13_0224DF30.eventRead = 0;
        }
    }
    OS_RestoreInterrupts(irq);
    return result;
}

// WCM event callback: queues the event unless the queue is full.
void WifiSetupLink_PushEvent(u32 event, int unused) {
    int write = ov13_0224DF30.eventWrite;
    int read = ov13_0224DF30.eventRead;
    int next = write + 1;
    if (next != read && write != read + 3) {
        ov13_0224DFB4[write] = event;
        ov13_0224DF30.eventWrite = next;
        if (next >= 4) {
            ov13_0224DF30.eventWrite = 0;
        }
    }
}

// Alarm handler: queues a timeout event.
void WifiSetupLink_PostEvent(u32 event) {
    WifiSetupLink_PushEvent(event, 0);
}

#pragma force_active on

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
    ov13_0224DF30.apListCount = count;
    WifiSetupLink_ClearEvents();
    size = count * 0xD0;
    ov13_0224DF30.allocation = ov13_0224DF30.alloc(size + 0x24D0 + count * 0xC0);
    if (!ov13_0224DF30.allocation) {
        return -1;
    }
    aligned = AlignDown32((u32)ov13_0224DF30.allocation + m);
    apList = aligned + (size + 0x2490);
    apList += m;
    apList &= k;
    ov13_0224DF30.buffer = (void *)aligned;
    ov13_0224DF30.apList = (WifiSetupApInfo *)apList;
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

#pragma force_active reset

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
                case 5:
                    break;
                case 20:
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

    if (ov13_0224DF30.allocation != NULL) {
        WifiSetupWorkFreeFunc freeFunc = ov13_0224DF30.free;
        freeFunc(ov13_0224DF30.allocation);
        ov13_0224DF30.allocation = NULL;
    }
    return 1;
}

#pragma force_active on

// Connects to the selected access point (retrying after event 13), then starts the socket library.
// Returns 1, 0 (no access point), -2, -3 (deadline passed) or -8 (stopped).
int WifiSetupLink_ConnectSelectedAp(void) {
    OSAlarm alarm;
    int active = 1;
    int result = -2;
    int event;
    WifiSetupApInfo *ap = &ov13_0224DF30.apList[ov13_0224DF30.selection];
    if (!ap) {
        return 0;
    }
    if (!WifiSetupLink_StartConnect(ap, NULL, 0x30000)) {
        return -2;
    }
    OS_CreateAlarm(&alarm);
    OS_SetAlarm(&alarm, 0x3FEC42, (OSAlarmHandler)WifiSetupLink_PostEvent, (void *)18);
    do {
        if (WifiSetup_GetMilliseconds() >= ov13_02245A58.deadline) {
            result = -3;
            break;
        }
        if (ov13_0224DF30.stop) {
            result = -8;
            break;
        }
        OS_Sleep(10);
        event = WifiSetupLink_PopEvent();
        while (event) {
            switch (event) {
            case 4:
            case 5:
            case 18:
            case 19:
                break;
            case 12:
                active = 0;
                result = 1;
                break;
            case 13:
                if (ov13_0224DF30.stop) {
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
        ov13_0224DF30.connected = 1;
        if (SOC_Startup(ov13_02245A6C) < 0) {
            result = -2;
        } else {
            ov13_0224DF30.socketStarted = 1;
        }
    }
    return result;
}

#pragma force_active reset

// Disconnects (waits for event 14) and stops the socket library.
void WifiSetupLink_DisconnectAp(void) {
    int running = 1;

    if (ov13_0224DF30.connected != 0) {
        if (WifiSetupLink_StartDisconnect()) {
            int zero = 0;
            int keepGoing = zero;
            do {
                OS_Sleep(10);
                int event = WifiSetupLink_PopEvent();
                while (event != 0) {
                    switch (event) {
                    case 4:
                    case 5:
                        break;
                    case 14:
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
        ov13_0224DF30.connected = 0;
    }
    if (ov13_0224DF30.socketStarted != 0) {
        ov13_0224DF30.socketStarted = 0;
        SOC_Cleanup();
    }
}
