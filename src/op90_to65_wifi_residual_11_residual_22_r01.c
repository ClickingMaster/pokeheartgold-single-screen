#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 prefix[18];
    int (*active)(void);
} Context;

extern Context gCpsState;

typedef union {
    u16 words[30];
    u8 bytes[60];
} Packet;

extern u32 CPS_SocGetLength(void);
extern u8 *CPS_SocRead(u32 *);
extern u8 *dns_skipname(u8 *);
extern void CPS_SocConsume(u32);
extern void OS_YieldThread__(void);
extern int CPS_SocWrite(void *, u32);
#define SWAP(x) ((u16)(((s32)(x) >> 8) | ((u32)(x) << 8)))
#define PAIR(p) ((u16)(((p)[0] << 8) | (p)[1]))
int resolve_common(u8 *, u32, int, void *, u32);

int resolve_common(u8 *name, u32 type, int id, void *out, u32 cap) {
    u32 length;
    u16 packet[30];
    u32 result;
    u32 start;
    {
        u8 *next;
        u32 count;
        int offset;
        int ch;
        u8 *label;
        packet[0] = SWAP(id);
        if (type != 32) {
            packet[1] = 1;
        } else {
            packet[1] = 0x1001;
        }
        packet[2] = 256;
        packet[3] = 0;
        count = 0;
        label = (u8 *)&packet[6];
        packet[4] = 0;
        packet[5] = 0;
        length = 0;
        next = label + 1;
        ch = *name++;
        if (ch) {
            do {
                if (ch != '.') {
                    offset = next - (u8 *)packet;
                    if (offset >= 60) {
                        return -1;
                    }
                    length++;
                    count = length;
                    *next++ = ch;
                } else {
                    *label = count;
                    length = 0;
                    label = next;
                    next++;
                    count = 0;
                }
                ch = *name++;
            } while (ch);
        }
        *label = length;
        next[0] = 0;
        next[1] = type >> 8;
        next[2] = type;
        next[3] = 0;
        next[4] = 1;
        CPS_SocWrite(packet, next + 5 - (u8 *)packet);
    }
    result = 0;
    start = OS_GetTick() >> 16;
    while (gCpsState.active() && !result && (int)((u32)(OS_GetTick() >> 16) - start) < 15) {
        u8 *p, *end;
        u32 n;
        if (!CPS_SocGetLength()) {
            OS_YieldThread__();
            continue;
        }
        p = CPS_SocRead(&length);
        if (length <= 12) {
            goto consume;
        }
        if (id != SWAP(*(u16 *)p)) {
            goto consume;
        }
        n = p[3] & 15;
        if (n == 3) {
            result = -1;
            goto consume;
        }
        if (n) {
            goto consume;
        }
        end = p + length;
        n = PAIR(p + 4);
        p += 12;
        while (n--) {
            p = dns_skipname(p) + 4;
        }
        while (p < end) {
            u32 size;
            p = dns_skipname(p);
            size = PAIR(p + 8);
            if (type == PAIR(p)) {
                if (type != 12) {
                    u8 *lo = p + 8;
                    u8 *hi = p + 6;
                    result = ((u32)PAIR(hi + size) << 16) | PAIR(lo + size);
                } else if (size > cap) {
                    result = 2;
                } else {
                    MI_CpuCopy8(p + 10, out, size);
                    result = 1;
                }
                break;
            }
            p += size + 10;
        }
consume:
        CPS_SocConsume(length);
    }
    return result;
}
