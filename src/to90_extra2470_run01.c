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
extern int NNS_G2dEntryOamManagerOamWithAffineIdx(struct OamState *, const GXOamAttr *, u16);
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

int NNS_G2dEntryOamManagerOamWithAffineIdx(struct OamState *m, const GXOamAttr *src, u16 idx) {
    BOOL available = canEntry((void *)&m->first);
    if (available) {
        u32 type = m->type;
        u8 *base = _021D8518 + type * 0x540 + 0x100;
        u8 *p = base + m->next * 8;
        ((GXOamAttr *)p)->attr0 = src->attr0;
        ((GXOamAttr *)p)->attr1 = src->attr1;
        ((GXOamAttr *)p)->attr2 = src->attr2;
        if (idx != 0xfffe) {
            GXOamAttr *attr = (GXOamAttr *)p;
            if (attr->rsMode & 1) {
                attr->rsParam = idx & 31;
            }
        }
        ++m->next;
        return 1;
    }
    return 0;
}
