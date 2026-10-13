#include "global.h"

#include "wifi_setup_internal.h"

void WifiSetup_EncodeLittleEndian(u8 *dst, const volatile u32 *src, u32 len) {
    u32 i = 0;
    while (i < len) {
        u8 *p = dst + i;
        dst[i] = *src;
        p[1] = *src >> 8;
        p[2] = *src >> 16;
        p[3] = *src++ >> 24;
        i += 4;
    }
}

void WifiSetup_DecodeLittleEndian(u32 *dst, const u8 *src, u32 len) {
    u32 i = 0;
    while (i < len) {
        *dst++ = src[i] | (src[i + 1] << 8) | (src[i + 2] << 16) | (src[i + 3] << 24);
        i += 4;
    }
}

void WifiSetup_CopyBytes(u8 *dst, const u8 *src, u32 len) {
    u32 i = 0;
    while (i < len) {
        dst[i] = src[i];
        i++;
    }
}

void WifiSetup_FillBytes(void *dst, int value, u32 count) {
    u32 i = 0;
    s8 *p = dst;
    for (; i < count;) {
        i++;
        *p++ = (s8)value;
    }
}

void WifiSetup_CalcMd5(u8 *digest, const u8 *src, u32 len) {
    WifiSetupMd5Context context;
    WifiSetup_Md5Init(&context);
    WifiSetup_Md5Update(&context, src, len);
    WifiSetup_Md5Final(digest, &context);
}

// Milliseconds since start-up.
u32 WifiSetup_GetMilliseconds(void) {
    return (u32)((OS_GetTick() << 6) / 0x82ea);
}

void *WifiSetup_WorkCalloc(u32 count, u32 size) {
    u32 total = count * size;
    void *ptr = ov13_0224DF30.alloc(total);
    if (ptr != NULL) {
        memset(ptr, 0, total);
    }
    return ptr;
}

void WifiSetup_WorkFree(void *ptr) {
    ov13_0224DF30.free(ptr);
}

#pragma force_active on

// Two uppercase hex digits of a byte.
int WifiSetup_FormatHexByte(char *dst, int value) {
    char *cursor = dst;
    int count;
    u8 byte = value;
    int digit = (byte & 0xF0) >> 4;
    count = 0;
    int next = byte & 0xF;
    do {
        if (digit <= 9) {
            digit += '0';
        } else {
            digit += 'A' - 10;
        }
        count++;
        *cursor = digit;
        cursor++;
        digit = next;
    } while (count < 2);
    *cursor = '\0';
    return cursor - dst;
}

#pragma force_active reset

// "XX:XX:XX:XX:XX:XX"
int WifiSetup_FormatMacAddress(char *dst, const s8 *mac) {
    char *p = dst;
    int i;
    for (i = 0; i < 6; i++) {
        p += WifiSetup_FormatHexByte(p, *mac++);
        if (i < 5) {
            *p++ = ':';
        }
    }
    *p = '\0';
    return p - dst;
}

// the worker thread changes the state: read and write it as volatile here
#define WORK_STATE (*(volatile s32 *)&ov13_0224DF30.state)

#pragma force_active on

int WifiSetup_StartSimpleStart(u32 priority, u32 count, void (*statusCallback)(s32 *), WifiSetupWorkAllocFunc alloc, WifiSetupWorkFreeFunc free, u32 stackSize) {
    int result;
    int state;

    state = WORK_STATE;
    if (state >= 1 && state <= 5) {
        return -10;
    }
    ov13_02245A58.count = count;
    WORK_STATE = 7;
    ov13_0224DF30.statusCallback = statusCallback;
    ov13_0224DF30.alloc = alloc;
    ov13_0224DF30.free = free;
    ov13_02245A58.threadStackSize = stackSize;
    result = WifiSetupLink_Open(count);
    ov13_0224DF30.resultReady = 1;
    if (result < 0) {
        ov13_0224DF30.error = result;
        return result;
    }
    ov13_0224DF30.threadStack = ov13_0224DF30.alloc(ov13_02245A58.threadStackSize);
    if (ov13_0224DF30.threadStack == NULL) {
        ov13_0224DF30.error = -1;
        return -1;
    }
    if (OS_IsThreadAvailable() != TRUE) {
        ov13_0224DF30.error = -9;
        return -9;
    }
    OS_CreateThread(&ov13_0224DFDC, WifiSetup_SimpleStartThread, NULL, (u8 *)ov13_0224DF30.threadStack + (ov13_02245A58.threadStackSize & ~7), ov13_02245A58.threadStackSize, priority);
    WORK_STATE = 1;
    ov13_02245A58.deadline = WifiSetup_GetMilliseconds() + 60000;
    ov13_0224DF30.stop = 0;
    memset(&ov13_0224E09C, 0, sizeof(ov13_0224E09C));
    WifiSetup_ReportSimpleStartStatus();
    OS_WakeupThreadDirect(&ov13_0224DFDC);
    ov13_0224DF30.active = 1;
    return 1;
}

#pragma force_active reset

#undef WORK_STATE

int WifiSetup_StopSimpleStart(void) {
    s32 old;
    if (ov13_0224DF30.active != 0) {
        old = ov13_0224DF30.state;
        ov13_0224DF30.stop = 1;
        while (ov13_0224DF30.state >= 1 && ov13_0224DF30.state <= 5) {
            OS_Sleep(100);
        }
        OS_Sleep(500);
        while (!OS_IsThreadTerminated(&ov13_0224DFDC)) {
            OS_WakeupThreadDirect(&ov13_0224DFDC);
            OS_JoinThread(&ov13_0224DFDC);
        }
        if (ov13_0224DF30.threadStack != NULL) {
            ov13_0224DF30.free(ov13_0224DF30.threadStack);
            ov13_0224DF30.threadStack = NULL;
        }
        ov13_0224DF30.active = 0;
        if (old != ov13_0224DF30.state) {
            WifiSetup_ReportSimpleStartStatus();
        }
    }
    if (ov13_0224DF30.resultReady > 0) {
        int result = WifiSetupLink_Close();
        ov13_0224DF30.resultReady = 0;
        return result;
    }
    return -10;
}

int WifiSetup_GetSimpleStartStatus(s32 *status) {
    status[0] = ov13_0224DF30.state;
    if (ov13_02245A58.deadline == -1) {
        status[1] = -1;
    } else {
        status[1] = ov13_02245A58.deadline - WifiSetup_GetMilliseconds();
    }
    status[2] = ov13_0224DF30.error;
    return 1;
}

int WifiSetup_GetSimpleStartResult(void *dst) {
    memcpy(dst, (u8 *)&ov13_0224E09C, sizeof(ov13_0224E09C));
    return 1;
}

void WifiSetup_ReportSimpleStartStatus(void) {
    s32 status[3];
    WifiSetup_GetSimpleStartStatus(status);
    ov13_0224DF30.statusCallback(status);
}
