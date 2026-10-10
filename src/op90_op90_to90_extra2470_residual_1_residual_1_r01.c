#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Range {
    u16 first, last, next;
};

struct OamState {
    u32 type;
    struct Range oam, aff;
    u16 id, pad;
    int fast;
};

struct OamEntry {
    u16 attr0, attr1, attr2, affine;
};

struct Bank {
    u8 reserved[0x100];
    struct OamEntry entries[128];
    u8 tail[0x40];
};
extern struct Bank _021D8518[];
extern u32 GXi_DmaId;

static inline BOOL valid(struct Range *p) {
    BOOL result = FALSE;
    if (p->next <= p->last + 1) {
        if (p->first <= p->last) {
            result = TRUE;
        }
    }
    return result;
}

void NNS_G2dResetOamManagerBuffer(struct OamState *);

void NNS_G2dResetOamManagerBuffer(struct OamState *m) {
    u8 *base;
    if (m->fast) {
        u32 bytes = valid(&m->oam) ? (u16)(m->oam.last - m->oam.first + 1) * 8 : 0;
        u32 addr = m->type;
        addr = (u32)&_021D8518[addr].entries[m->oam.first];
        base = (u8 *)addr;
        DC_InvalidateRange(base, bytes);
        if (GXi_DmaId != ~0u) {
            MI_DmaFill32(GXi_DmaId, base, 0xc0, bytes);
        } else {
            MIi_CpuClear32(0xc0, (void *)base, bytes);
        }
    } else {
        u16 n, i = 0;
        u32 addr = m->type;
        addr = (u32)&_021D8518[addr].entries[m->oam.first];
        base = (u8 *)addr;
        n = m->oam.last - m->oam.first + 1;
        for (; i < n; i++, base += 8) {
            *(u16 *)base = 0xc0;
        }
    }
    m->oam.next = m->oam.first;
    m->aff.next = m->aff.first;
}
