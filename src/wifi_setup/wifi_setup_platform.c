#include "global.h"

#include "wifi_setup/wifi_setup.h"
#include "wifi_setup/wifi_setup_wcm.h"

extern int RTC_GetTime(void *time);
extern u8 gWifiSetupSocket[];
extern SocConfig sWifiSetupSocConfig;

typedef struct RateMask {
    u16 mask;
    u8 rate;
    u8 padding;
} RateMask;

extern RateMask sWifiSetupRateMasks[];

typedef struct ScanRecord {
    u8 unk_00[4];
    u16 mac[3];
    u16 ssidLength;
    u16 ssid[16];
    u16 flags;
    u16 basicRates;
    u16 supportedRates;
    u16 beaconPeriod;
    u16 dtimPeriod;
    u16 channel;
} ScanRecord;

typedef struct AccessPointInfo {
    u32 ssidLength;
    u16 ssid[16];
    u32 channel;
    u8 unk_28[8];
    u16 mac[3];
    u8 unk_36[2];
    int rateCount;
    u8 rates[16];
    int beaconPeriod;
    int mode;
} AccessPointInfo;

static void WifiSetup_ConvertBssDesc(const void *src, void *dst);
static BOOL WifiSetup_OnWcmNotify(void *message);
static int WifiSetup_DisconnectAccessPoint(void);

BOOL WifiSetupWcm_Init(BOOL (*callback)(void *), void *allocation, u32 size);
BOOL WifiSetupWcm_StartSearch(u32, u32, u32, u32);
int WifiSetupWcm_CopyApList(void *, u32);

void *WifiSetup_SocketAlloc(s32 unused, s32 size) {
    void *(*alloc)(s32);
    if (size > 0) {
        alloc = (void *(*)(s32))gWifiSetupRuntime.alloc;
        return alloc(size);
    }
    return NULL;
}

void *WifiSetup_SocketFree(s32 unused, s32 ptr) {
    void *(*free)(s32) = (void *(*)(s32))gWifiSetupFreeHook;
    return free(ptr);
}

int WifiSetup_StartupSocket(u32 address, u32 netmask, u32 gateway) {
    *(u32 *)(gWifiSetupSocket + 0x18) = WifiSetup_HostToNet32(address);
    *(u32 *)(gWifiSetupSocket + 0x1C) = WifiSetup_HostToNet32(netmask);
    *(u32 *)(gWifiSetupSocket + 0x20) = WifiSetup_HostToNet32(gateway);
    if (SOC_Startup(&sWifiSetupSocConfig) < 0) {
        return -1;
    }
    while (gSoclHostAddress == 0) {
        OS_Sleep(100);
    }
    return 0;
}

int WifiSetup_CleanupSocket(void) {
    if (SOC_Cleanup() < 0) {
        return -1;
    }
    return -!!WifiSetup_DisconnectAccessPoint();
}

int WifiSetup_MemCompare(const u8 *a, const u8 *b, int n) {
    int diff = 0;
    while (n-- > 0 && (diff = *a - *b) == 0) {
        a++;
        b++;
    }
    return diff;
}

void WifiSetup_MemCopy(void *dst, const void *src, u32 size) {
    MI_CpuCopy8(src, dst, size);
}

void WifiSetup_MemFill(void *dst, int value, u32 size) {
    MI_CpuFill8(dst, value, size);
}

int WifiSetup_RecvFrom(int socket, void *buffer, int size, int flags, SocAddress *address, int *addressLength) {
    address->length = *addressLength;
    return SOC_RecvFrom(socket, buffer, size, flags, address);
}

int WifiSetup_Poll(int count, SocPollFd *readSet, void *writeSet, void *exceptSet, WifiSetupPollTime *timeout) {
    SocPollFd copy = *readSet;
    u64 ticks = 0;
    ticks += (u64)((s64)timeout->seconds * 0x1FF6210) >> 6;
    ticks += (u64)((s64)timeout->microseconds * 0x1FF6210) >> 6;
    return SOC_Poll(&copy, 1, ticks);
}

int WifiSetup_SendTo(int socket, const void *buffer, int size, int flags, SocAddress *address, int addressLength) {
    address->length = addressLength;
    return SOC_SendTo(socket, buffer, size, flags, address);
}

// Socket options are not supported; the socket number comes back as a success result.
int WifiSetup_SetSockOpt(int socket, int level, int name, const void *value, int length) {
    return socket;
}

int WifiSetup_Socket(int domain, int type, int protocol) {
    return SOC_Socket(domain, type, protocol);
}

int WifiSetup_Bind(int socket, SocAddress *address, int addressLength) {
    address->length = addressLength;
    return SOC_Bind(socket, address, addressLength);
}

int WifiSetup_Close(int socket) {
    return SOC_Close(socket);
}

u32 WifiSetup_HostToNet32(u32 x) {
    return (u8)(x >> 24) | ((x >> 8) & 0xFF00) | ((x << 8) & 0xFF0000) | ((x << 24) & 0xFF000000);
}

u16 WifiSetup_HostToNet16(int x) {
    return (u8)(x >> 8) | ((x << 8) & 0xFF00);
}

u32 WifiSetup_NetToHost32(u32 x) {
    return (u8)(x >> 24) | ((x >> 8) & 0xFF00) | ((x << 8) & 0xFF0000) | ((x << 24) & 0xFF000000);
}

u16 WifiSetup_NetToHost16(int x) {
    return (u8)(x >> 8) | ((x << 8) & 0xFF00);
}

s32 WifiSetup_StrLen(const char *str) {
    s32 len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

u16 WifiSetup_Random(void) {
    u32 time[3];
    if (gWifiSetupState.randomSeeded == 0) {
        u32 seed = 0;
        WifiSetup_MemFill(time, 0, 12);
        if (RTC_GetTime(time) == 0) {
            seed += time[0] << 10;
            seed += time[1] << 3;
            seed += time[2];
        }
        gWifiSetupState.randomState = seed;
        gWifiSetupState.randomMultiplier = 0x5D588B65;
        gWifiSetupState.randomIncrement = 0x269EC3;
        gWifiSetupState.randomSeeded = 1;
    }
    gWifiSetupState.randomState = gWifiSetupState.randomMultiplier * gWifiSetupState.randomState + gWifiSetupState.randomIncrement;
    return (u16)(((gWifiSetupState.randomState >> 16) * 0x7FFF) >> 16);
}

void WifiSetup_ClearPollSet(SocPollFd *set) {
    set->fd = 0;
    set->events = 0;
    set->revents = 0;
}

void WifiSetup_AddToPollSet(int socket, SocPollFd *set) {
    set->fd = socket;
    set->events = 1;
}

static void WifiSetup_ConvertBssDesc(const void *src, void *dst) {
    const ScanRecord *s = src;
    AccessPointInfo *t = dst;
    int i;
    int count;
    RateMask *p;

    t->ssidLength = s->ssidLength;
    MIi_CpuCopy16(s->ssid, t->ssid, 32);
    t->channel = s->channel;
    MIi_CpuCopy16(s->mac, t->mac, 6);
    count = 0;
    p = sWifiSetupRateMasks;
    i = count;
    for (; i < 12; i++, p++) {
        if (s->supportedRates & p->mask) {
            t->rates[count] = p->rate;
            if (s->basicRates & p->mask) {
                t->rates[count] |= 128;
            }
            count++;
        }
    }
    t->rateCount = count;
    t->beaconPeriod = s->beaconPeriod;
    if ((s->flags & 3) == 1) {
        t->mode = 1;
    } else if ((s->flags & 3) == 2) {
        t->mode = 2;
    } else {
        t->mode = 0;
    }
}

void WifiSetup_CopyConnectSsid(const void *src, void *dst) {
    u8 *p = dst;
    MIi_CpuClear16(0, (u16 *)(p + 0xC), 0x20);
    *(u16 *)(p + 0xA) = *(u32 *)src;
    MI_CpuCopy8((const u8 *)src + 4, p + 0xC, *(u16 *)(p + 0xA));
}

void WifiSetup_StoreConnectResult(void *dst, const void *src, u32 value) {
    *(u32 *)dst = value;
    WifiSetup_ConvertBssDesc(src, (u8 *)dst + 4);
}

static BOOL WifiSetup_OnWcmNotify(void *message) {
    return OS_SendMessage(&gWifiSetupMessageQueue, message, OS_MESSAGE_NOBLOCK);
}

BOOL WifiSetup_OnTimeoutAlarm(void *message) {
    return OS_SendMessage(&gWifiSetupMessageQueue, message, OS_MESSAGE_NOBLOCK);
}

int WifiSetup_Startup(WifiSetupAllocFunc alloc, WifiSetupFreeFunc free) {
    OSMessage message;
    BOOL running = TRUE;

    OS_InitMessageQueue(&gWifiSetupMessageQueue, gWifiSetupMessages, 4);
    if (alloc == NULL || free == NULL) {
        return -1;
    }

    OSIntrMode enabled = OS_DisableInterrupts();
    gWifiSetupRuntime.alloc = alloc;
    gWifiSetupRuntime.free = free;
    OS_RestoreInterrupts(enabled);

    gWifiSetupRuntime.allocation = gWifiSetupRuntime.alloc(0x5890);
    if (gWifiSetupRuntime.allocation == NULL) {
        return -1;
    }
    if (!WifiSetupWcm_Init(WifiSetup_OnWcmNotify, gWifiSetupRuntime.allocation, 0x5890)) {
        running = FALSE;
    }

    while (running) {
        OS_ReceiveMessage(&gWifiSetupMessageQueue, &message, OS_MESSAGE_BLOCK);
        switch ((u32)message) {
        case WIFI_SETUP_WCM_EVENT_STARTED:
            return 0;
        case 4:
        case WIFI_SETUP_WCM_EVENT_FOUND_AP:
            break;
        case 0:
        case WIFI_SETUP_WCM_EVENT_UNKNOWN:
        case WIFI_SETUP_WCM_EVENT_FAILED:
        case 3:
        case 7:
        case WIFI_SETUP_WCM_EVENT_SEARCH_STARTED:
        case WIFI_SETUP_WCM_EVENT_SEARCH_FAILED:
        case WIFI_SETUP_WCM_EVENT_SEARCH_ENDED:
        case WIFI_SETUP_WCM_EVENT_SEARCH_END_FAILED:
        case WIFI_SETUP_WCM_EVENT_CONNECTED:
        case WIFI_SETUP_WCM_EVENT_CONNECT_FAILED:
        case WIFI_SETUP_WCM_EVENT_DISCONNECTED:
        case WIFI_SETUP_WCM_EVENT_DISCONNECT_FAILED:
        default:
            running = FALSE;
            break;
        }
    }

    gWifiSetupRuntime.free(gWifiSetupRuntime.allocation);
    return -1;
}

int WifiSetup_Cleanup(void) {
    OSMessage message;
    BOOL running = TRUE;
    int result = -1;

    if (gWifiSetupRuntime.free == NULL) {
        return -1;
    }
    if (!WifiSetupWcm_StartCleanup()) {
        return -1;
    }

    while (running) {
        OS_ReceiveMessage(&gWifiSetupMessageQueue, &message, OS_MESSAGE_BLOCK);
        switch ((u32)message) {
        case 4:
        case WIFI_SETUP_WCM_EVENT_FOUND_AP:
            break;
        case WIFI_SETUP_WCM_EVENT_CLEANED_UP:
            running = FALSE;
            result = 0;
            gWifiSetupRuntime.free(gWifiSetupRuntime.allocation);
            break;
        case 0:
        case WIFI_SETUP_WCM_EVENT_UNKNOWN:
        case WIFI_SETUP_WCM_EVENT_FAILED:
        case 3:
        case WIFI_SETUP_WCM_EVENT_STARTED:
        case 7:
        case WIFI_SETUP_WCM_EVENT_SEARCH_STARTED:
        case WIFI_SETUP_WCM_EVENT_SEARCH_FAILED:
        case WIFI_SETUP_WCM_EVENT_SEARCH_ENDED:
        case WIFI_SETUP_WCM_EVENT_SEARCH_END_FAILED:
        case WIFI_SETUP_WCM_EVENT_CONNECTED:
        case WIFI_SETUP_WCM_EVENT_CONNECT_FAILED:
        case WIFI_SETUP_WCM_EVENT_DISCONNECTED:
        case WIFI_SETUP_WCM_EVENT_DISCONNECT_FAILED:
        case 16:
        case 17:
        case 18:
        case 19:
        default:
            running = FALSE;
            break;
        }
    }

    OSIntrMode enabled = OS_DisableInterrupts();
    gWifiSetupRuntime.alloc = NULL;
    gWifiSetupRuntime.free = NULL;
    OS_RestoreInterrupts(enabled);
    return result;
}

static int WifiSetup_DisconnectAccessPoint(void) {
    OSMessage message;
    int result = -1;
    BOOL running;

    if (WifiSetupWcm_StartDisconnect()) {
        do {
            OS_ReceiveMessage(&gWifiSetupMessageQueue, &message, OS_MESSAGE_BLOCK);
            if ((u32)message == 14) {
                running = FALSE;
                result = running;
            } else {
                running = FALSE;
            }
        } while (running);
    }
    return result;
}

typedef struct ScanLocals {
    BOOL running;
    int result;
    OSMessage message;
    OSAlarm alarm;
} ScanLocals;

int WifiSetup_SearchAccessPoints(void **output) {
    ScanLocals local;
    local.result = -1;
    local.running = TRUE;
    u8 *buffer;
    int state = 0;
    int records = 0;
    int count = 0;
    u8 *base;

    if (gWifiSetupRuntime.alloc == NULL || gWifiSetupRuntime.free == NULL) {
        return -1;
    }

    buffer = gWifiSetupRuntime.alloc(0x3000);
    if (buffer == NULL) {
        return -1;
    }
    base = buffer;
    if (WifiSetupWcm_StartSearch(0, 0, 0, 0x0030BFFE) != 0) {
        goto started;
    }
    goto cleanup;

started:

    OS_CreateAlarm(&local.alarm);
    OS_SetAlarm(&local.alarm, 0x003FEC42, (OSAlarmHandler)WifiSetup_OnTimeoutAlarm, (void *)0x13);
    do {
        OS_ReceiveMessage(&gWifiSetupMessageQueue, &local.message, OS_MESSAGE_BLOCK);
        switch ((u32)local.message) {
        case 19:
            if (state == 0) {
                if (count != 0) {
                    records = WifiSetupWcm_CopyApList(buffer, 0x40);
                }
                if (WifiSetupWcm_StopSearch()) {
                    state = 1;
                } else {
                    goto finish;
                }
            }
            break;
        case WIFI_SETUP_WCM_EVENT_FOUND_AP:
            if (state == 0) {
                if (count < 8) {
                    count++;
                } else {
                    records = WifiSetupWcm_CopyApList(buffer, 0x40);
                    if (WifiSetupWcm_StopSearch()) {
                        state = 1;
                    } else {
                        goto finish;
                    }
                }
            }
            break;
        case WIFI_SETUP_WCM_EVENT_SEARCH_ENDED:
            local.running = FALSE;
            local.result = 0;
            break;
        case 4:
        case WIFI_SETUP_WCM_EVENT_SEARCH_STARTED:
        case 18:
            break;
        case 0:
        case WIFI_SETUP_WCM_EVENT_UNKNOWN:
        case WIFI_SETUP_WCM_EVENT_FAILED:
        case 3:
        case WIFI_SETUP_WCM_EVENT_STARTED:
        case 7:
        case WIFI_SETUP_WCM_EVENT_SEARCH_FAILED:
        case WIFI_SETUP_WCM_EVENT_SEARCH_END_FAILED:
        case WIFI_SETUP_WCM_EVENT_CONNECTED:
        case WIFI_SETUP_WCM_EVENT_CONNECT_FAILED:
        case WIFI_SETUP_WCM_EVENT_DISCONNECTED:
        case WIFI_SETUP_WCM_EVENT_DISCONNECT_FAILED:
        case 16:
        case 17:
        default:
            goto finish;
        }
    } while (local.running);

    void *allocation;
    if (records != 0) {
        allocation = gWifiSetupRuntime.alloc((records - 1) * 0x54 + 0x58);
        if (allocation != NULL) {
            goto allocated;
        }
        goto finish;
    } else {
        allocation = gWifiSetupRuntime.alloc(0x58);
        if (allocation == NULL) {
            goto finish;
        }
    }
allocated: {
    u8 *dst;
    int i = 0;

    *output = allocation;
    *(u32 *)allocation = records;
    if (records > 0) {
        dst = (u8 *)allocation + 4;
        do {
            WifiSetup_ConvertBssDesc(buffer, dst);
            i++;
            buffer += 0xc0;
            dst += 0x54;
        } while (i < records);
    }
}

finish:
    OS_CancelAlarm(&local.alarm);
    do {
    } while (OS_ReceiveMessage(&gWifiSetupMessageQueue, &local.message, OS_MESSAGE_NOBLOCK) == TRUE);

cleanup:
    gWifiSetupRuntime.free(base);
    return local.result;
}
