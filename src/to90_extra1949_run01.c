#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Input {
    u32 a, b;
    volatile u32 c, d;
    u16 e;
} Input;

typedef struct Date {
    int year, month, day, week;
} Date;

typedef struct Time {
    int hour, minute, second;
} Time;

extern char ov00_02216550[];
extern char ov00_02216558[];
extern char ov00_02216560[];
extern char ov00_02216568[];
extern char ov00_02216574[];
extern char ov00_02216590[];
void OS_GetMacAddress(u8 *);
void *WCM_GetApMacAddress(void);
void *WCM_GetApEssid(u16 *);
int ov00_021FAA40(void);
void ov00_021FAA70(char *);
RTCResult RTC_GetDateTime(RTCDate *, RTCTime *);
int ov00_021E89AC(char *, Input *);

typedef struct {
    char byte[2];
} HexChars;

static inline void MacString(char *dest, u8 *src) {
    int index;
    HexChars *output = (HexChars *)dest;
    for (index = 0; index < 6; index++, output++) {
        OS_SPrintf(output->byte, ov00_02216560, *src++);
    }
}

#pragma force_active on

int ov00_021E89AC(char *out, Input *input) {
    char owner[84];
    Date date;
    Time time;
    u8 mac[6];
    u16 essidLen;
    int i;
    s32 source;
    OSIntrMode irq;
    u8 *ap;
    char *essid, *cursor;
    u32 a, b;
    MI_CpuFill8(out, 0, 0x94);
    a = input->a;
    b = input->b;
    if (b || a) {
        OS_SNPrintf(out, 14, ov00_02216550, a, b);
    } else {
        a = input->c;
        b = input->d;
        OS_SNPrintf(out, 14, ov00_02216550, a, b);
    }
    OS_SNPrintf(out + 14, 7, ov00_02216558, input->e);
    if (!*(volatile u8 *)0x27ffe0c) {
        OS_Terminate();
    }
    MI_CpuCopy8((void *)0x27ffe0c, out + 21, 4);
    if (!*(volatile u8 *)0x27ffe10) {
        OS_Terminate();
    }
    MI_CpuCopy8((void *)0x27ffe10, out + 26, 2);
    out[29] = 48;
    OS_GetMacAddress(mac);
    source = (s32)mac;
    cursor = out + 31;
    for (i = 0; i < 6; i++, cursor += 2, source++) {
        OS_SPrintf(cursor, ov00_02216560, *(u8 *)source);
    }
    OS_GetOwnerInfo((OSOwnerInfo *)owner);
    if ((u8)owner[0] >= 8) {
        owner[0] = 1;
    }
    OS_SNPrintf(out + 44, 3, ov00_02216560, (u8)owner[0]);
    MI_CpuCopy8(owner + 4, out + 126, 20);
    OS_SNPrintf(out + 47, 5, ov00_02216568, (u8)owner[2], (u8)owner[3]);
    if (RTC_GetDateTime((RTCDate *)&date, (RTCTime *)&time) != 0) {
        return 0;
    }
    OS_SNPrintf(out + 52, 13, ov00_02216574, date.year, date.month, date.day, time.hour, time.minute, time.second);
    irq = OS_DisableInterrupts();
    ap = WCM_GetApMacAddress();
    DC_InvalidateRange(ap, 6);
    if (!ap) {
        OS_RestoreInterrupts(irq);
        return 0;
    }
    for (i = 0, cursor = out + 65; i < 6; i++, cursor += 2) {
        OS_SPrintf(cursor, ov00_02216560, ap[i]);
    }
    {
        unsigned strength = ov00_021FAA40();
        if (strength == 255 || strength > 99) {
            strength = 99;
        }
        OS_SNPrintf(out + 111, 14, ov00_02216590, strength);
    }
    essid = WCM_GetApEssid(&essidLen);
    DC_InvalidateRange(essid, 32);
    if (!essid) {
        OS_RestoreInterrupts(irq);
        return 0;
    }
    MI_CpuCopy8(essid, out + 78, 32);
    ov00_021FAA70(out + 114);
    OS_RestoreInterrupts(irq);
    return 1;
}
