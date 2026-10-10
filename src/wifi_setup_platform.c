#include "global.h"

#include "wifi_setup.h"

extern int SOC_Startup(void *config);
extern int SOC_Cleanup(void);
extern int SOC_RecvFrom(int socket, void *buffer, int size, int flags, u8 *address);
extern int SOC_SendTo(int socket, const void *buffer, int size, int flags, u8 *address);
extern int SOC_Poll(WifiSetupPollTime *fds, int count, s64 timeout);
extern int SOC_Socket(int domain, int type, int protocol);
extern int SOC_Bind(int socket, void *address, int addressLength);
extern int SOC_Close(int socket);
extern int RTC_GetTime(void *time);
extern u8 ov13_022459B4[];
extern u8 ov13_022459BC[];
extern volatile u32 _021D4670;

typedef struct ChannelMask {
    u16 mask;
    u8 id;
    u8 padding;
} ChannelMask;

extern ChannelMask ov13_02245A28[];

typedef struct ScanRecord {
    u8 unk_00[4];
    u16 mac[3];
    u16 unk_0A;
    u16 ssid[16];
    u16 flags;
    u16 presentChannels;
    u16 enabledChannels;
    u16 unk_32;
    u16 unk_34;
    u16 number;
} ScanRecord;

typedef struct AccessPointInfo {
    u32 unk_00;
    u16 ssid[16];
    u32 number;
    u8 unk_28[8];
    u16 mac[3];
    u8 unk_36[2];
    int channelCount;
    u8 channels[16];
    int unk_4C;
    int mode;
} AccessPointInfo;

BOOL ov13_022236B8(BOOL (*callback)(void *), void *allocation, u32 size);
BOOL ov13_022235DC(void);
BOOL ov13_022235A4(void);
BOOL ov13_02223478(u32, u32, u32, u32);
int ov13_02223434(void *, u32);
BOOL ov13_02223568(void);

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
    *(u32 *)(ov13_022459B4 + 0x18) = WifiSetup_HostToNet32(address);
    *(u32 *)(ov13_022459B4 + 0x1C) = WifiSetup_HostToNet32(netmask);
    *(u32 *)(ov13_022459B4 + 0x20) = WifiSetup_HostToNet32(gateway);
    if (SOC_Startup(ov13_022459BC) < 0) {
        return -1;
    }
    while (_021D4670 == 0) {
        OS_Sleep(100);
    }
    return 0;
}

int WifiSetup_CleanupSocket(void) {
    if (SOC_Cleanup() < 0) {
        return -1;
    }
    return -!!ov13_02222D78();
}

int WifiSetup_MemCompare(const u8 *a, const u8 *b, int n) {
    int diff = 0;
    goto check;
    do {
        a++;
        b++;
check:
        if (n-- <= 0) {
            break;
        }
        diff = *a - *b;
    } while (diff == 0);
    return diff;
}

void WifiSetup_MemCopy(void *dst, const void *src, u32 size) {
    MI_CpuCopy8(src, dst, size);
}

void WifiSetup_MemFill(void *dst, int value, u32 size) {
    MI_CpuFill8(dst, value, size);
}

int WifiSetup_RecvFrom(int socket, void *buffer, int size, int flags, u8 *address, u32 *addressLength) {
    *address = *addressLength;
    return SOC_RecvFrom(socket, buffer, size, flags, address);
}

#pragma force_active on

int WifiSetup_Poll(void *unused, WifiSetupPollTime *fd, int unused2, int unused3, WifiSetupPollTime *timeout) {
    WifiSetupPollTime copy = *fd;
    u64 ticks = 0;
    ticks += (u64)((s64)timeout->seconds * 0x1FF6210) >> 6;
    ticks += (u64)((s64)timeout->microseconds * 0x1FF6210) >> 6;
    return SOC_Poll(&copy, 1, ticks);
}

#pragma force_active reset

int WifiSetup_SendTo(int socket, const void *buffer, int size, int flags, u8 *address, u32 addressLength) {
    *address = addressLength;
    return SOC_SendTo(socket, buffer, size, flags, address);
}

void ov13_022229FC(void) {
}

int WifiSetup_Socket(int domain, int type, int protocol) {
    return SOC_Socket(domain, type, protocol);
}

int WifiSetup_Bind(int socket, u8 *address, int addressLength) {
    *address = addressLength;
    return SOC_Bind(socket, address, addressLength);
}

int WifiSetup_Close(int socket) {
    return SOC_Close(socket);
}

#pragma force_active on

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

#pragma force_active reset

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

void WifiSetup_ClearPollSet(void *set) {
    u8 *p = set;
    *(u32 *)(p + 0) = 0;
    *(u16 *)(p + 4) = 0;
    *(u16 *)(p + 6) = 0;
}

void WifiSetup_AddToPollSet(u32 socket, void *set) {
    u8 *p = set;
    *(u32 *)(p + 0) = socket;
    *(u16 *)(p + 4) = 1;
}

#pragma force_active on

void ov13_02222B34(const void *src, void *dst) {
    const ScanRecord *s = src;
    AccessPointInfo *t = dst;
    int i;
    int count;
    ChannelMask *p;

    t->unk_00 = s->unk_0A;
    MIi_CpuCopy16(s->ssid, t->ssid, 32);
    t->number = s->number;
    MIi_CpuCopy16(s->mac, t->mac, 6);
    count = 0;
    p = ov13_02245A28;
    i = count;
    for (; i < 12; i++, p++) {
        if (s->enabledChannels & p->mask) {
            t->channels[count] = p->id;
            if (s->presentChannels & p->mask) {
                t->channels[count] |= 128;
            }
            count++;
        }
    }
    t->channelCount = count;
    t->unk_4C = s->unk_32;
    if ((s->flags & 3) == 1) {
        t->mode = 1;
    } else if ((s->flags & 3) == 2) {
        t->mode = 2;
    } else {
        t->mode = 0;
    }
}

#pragma force_active reset

void ov13_02222BC0(const void *src, void *dst) {
    u8 *p = dst;
    MIi_CpuClear16(0, (u16 *)(p + 0xC), 0x20);
    *(u16 *)(p + 0xA) = *(u32 *)src;
    MI_CpuCopy8((const u8 *)src + 4, p + 0xC, *(u16 *)(p + 0xA));
}

void ov13_02222BE4(void *dst, const void *src, u32 value) {
    *(u32 *)dst = value;
    ov13_02222B34(src, (u8 *)dst + 4);
}

BOOL ov13_02222BF4(void *message) {
    return OS_SendMessage(&gWifiSetupMessageQueue, message, OS_MESSAGE_NOBLOCK);
}

BOOL ov13_02222C08(void *message) {
    return OS_SendMessage(&gWifiSetupMessageQueue, message, OS_MESSAGE_NOBLOCK);
}

int ov13_02222C1C(WifiSetupAllocFunc alloc, WifiSetupFreeFunc free) {
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
    if (!ov13_022236B8(ov13_02222BF4, gWifiSetupRuntime.allocation, 0x5890)) {
        running = FALSE;
    }

    while (running) {
        OS_ReceiveMessage(&gWifiSetupMessageQueue, &message, OS_MESSAGE_BLOCK);
        switch ((u32)message) {
        case 6:
            return 0;
        case 4:
        case 5:
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            running = FALSE;
            break;
        }
    }

    gWifiSetupRuntime.free(gWifiSetupRuntime.allocation);
    return -1;
}

int ov13_02222CE0(void) {
    OSMessage message;
    BOOL running = TRUE;
    int result = -1;

    if (gWifiSetupRuntime.free == NULL) {
        return -1;
    }
    if (!ov13_022235DC()) {
        return -1;
    }

    while (running) {
        OS_ReceiveMessage(&gWifiSetupMessageQueue, &message, OS_MESSAGE_BLOCK);
        switch ((u32)message) {
        case 4:
        case 5:
            break;
        case 20:
            running = FALSE;
            result = 0;
            gWifiSetupRuntime.free(gWifiSetupRuntime.allocation);
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
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

int ov13_02222D78(void) {
    OSMessage message;
    int result = -1;
    BOOL running;

    if (ov13_022235A4()) {
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

int ov13_02222DB0(void **output) {
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
    if (ov13_02223478(0, 0, 0, 0x0030BFFE) != 0) {
        goto started;
    }
    goto cleanup;

started:

    OS_CreateAlarm(&local.alarm);
    OS_SetAlarm(&local.alarm, 0x003FEC42, (OSAlarmHandler)ov13_02222C08, (void *)0x13);
    do {
        OS_ReceiveMessage(&gWifiSetupMessageQueue, &local.message, OS_MESSAGE_BLOCK);
        switch ((u32)local.message) {
        case 19:
            if (state == 0) {
                if (count != 0) {
                    records = ov13_02223434(buffer, 0x40);
                }
                if (ov13_02223568()) {
                    state = 1;
                } else {
                    goto finish;
                }
            }
            break;
        case 5:
            if (state == 0) {
                if (count < 8) {
                    count++;
                } else {
                    records = ov13_02223434(buffer, 0x40);
                    if (ov13_02223568()) {
                        state = 1;
                    } else {
                        goto finish;
                    }
                }
            }
            break;
        case 10:
            local.running = FALSE;
            local.result = 0;
            break;
        case 4:
        case 8:
        case 18:
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 6:
        case 7:
        case 9:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
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
            ov13_02222B34(buffer, dst);
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
