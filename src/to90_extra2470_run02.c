#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

struct OamState {
    u32 type;
    u16 first, last, next, affFirst, affLast, affNext, id, pad;
    int fast;
};

extern u8 _021D8518[];
extern u16 _021D8514;
extern u32 GXi_DmaId;
extern int NNS_G2dEntryOamManagerOamWithAffineIdx(struct OamState *, const u16 *, u16);
extern u16 NNS_G2dEntryOamManagerAffine(struct OamState *, const s32 *);
extern void NNS_G2dResetOamManagerBuffer(struct OamState *);

struct Range {
    u16 first, last, next;
};

static inline BOOL valid(struct Range *p) {
    BOOL result = FALSE;
    if (p->next <= p->last + 1) {
        if (p->first <= p->last) {
            result = TRUE;
        }
    }
    return result;
}

static inline u16 capacity(struct Range *p) {
    if (valid(p)) {
        return p->last - p->next + 1;
    }
    return 0;
}

static inline BOOL canEntry(struct Range *p) {
    return capacity(p) >= 1;
}

#pragma opt_propagation off

u16 NNS_G2dEntryOamManagerAffine(struct OamState *m, const s32 *matrix) {
    u8 *p;
    u32 type;
    u8 *base;
    BOOL available = canEntry((void *)&m->affFirst);
    if (available) {
        type = m->type;
        base = _021D8518;
        p = base + type * 0x540 + 0x100 + m->affNext * 32;
        u16 idx = m->affNext;
        *(u16 *)(p + 6) = matrix[0] >> 4;
        *(u16 *)(p + 14) = matrix[1] >> 4;
        *(u16 *)(p + 22) = matrix[2] >> 4;
        *(u16 *)(p + 30) = matrix[3] >> 4;
        ++m->affNext;
        return idx;
    }
    return 0xfffe;
}
