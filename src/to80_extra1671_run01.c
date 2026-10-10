#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0x28];
    u32 a9, n9;
    u8 gap[8];
    u8 *a7;
    u32 n7;
} Header;

typedef struct {
    u32 src, dst, size;
    u32 flags : 1;
    u32 rest : 31;
} Segment;

void MBi_SetSegmentInfo(Header *, u32 *, Segment *, u32 *);

void MBi_SetSegmentInfo(Header *h, u32 *type, Segment *s, u32 *target) {
    switch (*type) {
    case 0:
        if (h->a9 >= 0x2000000 && h->a9 < 0x22c0000 && h->a9 + h->n9 <= 0x22c0000) {
            s->size = h->n9;
            s->dst = h->a9;
            s->src = s->dst;
            s->flags = 0;
        } else {
            OS_Terminate();
        }
        return;
    case 1: {
        int bad = 0, copy = 0;
        u32 size = h->n7;
        u8 *addr = h->a7;
        u32 end = (u32)(addr + size);
        if ((u32)addr >= 0x2000000 && (u32)addr < 0x23fe800) {
            if (end <= 0x2300000) {
            } else if (end < 0x23fe800 && size <= 0x40000) {
                copy = 1;
            } else {
                bad = 1;
            }
        } else if ((u32)addr >= 0x37f8000 && (u32)addr < 0x380f000) {
            if (end <= 0x380f000) {
                copy = 1;
            } else {
                bad = 1;
            }
        } else {
            bad = 1;
        }
        if (bad == 1) {
            OS_Terminate();
        }
        s->size = h->n7;
        s->dst = (u32)h->a7;
        if (copy == 0) {
            s->src = s->dst;
        } else {
            s->src = *target;
            *target += s->size;
        }
        s->flags = 1;
        return;
    }
    case 2: {
        s->size = 0x160;
        s->dst = 0x27ffe00;
        s->src = 0x27ffe00;
        s->flags = 0;
        return;
    }
    }
}
