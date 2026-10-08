#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct OamState { u32 type; u16 first,last,next,affFirst,affLast,affNext,id,pad; int fast; } OamState;
typedef struct Range { u16 first,last,next; } Range;
typedef struct OamBuf { u8 hdr[0x100]; GXOamAttr oam[128]; u8 tail[0x40]; } OamBuf;
extern OamBuf _021D8518[];
typedef void (*OamLoadFunc)(const void *, u32, u32);
extern OamLoadFunc _021109C8[];
static inline BOOL valid(Range *p) { BOOL result = FALSE; if (p->next <= p->last + 1) { if (p->first <= p->last) result = TRUE; } return result; }
static inline void LoadOam(OamState *m)
{
    u32 type = m->type;
    GXOamAttr *p = _021D8518[type].oam + m->first;
    u16 count = m->last - m->first + 1;
    u16 offset = m->first * 8;
    u16 i;
    OamLoadFunc func = _021109C8[type];
    for (i = 0; i < count; i++) {
        func(p, offset, 6);
        offset += 8;
        p++;
    }
}
static inline void LoadAffine(OamState *m)
{
    u32 type = m->type;
    u8 *q = (u8 *)_021D8518[type].oam + m->affFirst * 32;
    u16 count = m->affLast - m->affFirst + 1;
    u16 aoff = m->affFirst * 32;
    u16 i;
    OamLoadFunc func = _021109C8[type];
    for (i = 0; i < count; i++) {
        func(q + 6, aoff + 6, 2);
        func(q + 14, aoff + 14, 2);
        func(q + 22, aoff + 22, 2);
        func(q + 30, aoff + 30, 2);
        aoff += 32;
        q += 32;
    }
}
void NNS_G2dApplyOamManagerToHW(OamState *m);
// @fn NNS_G2dApplyOamManagerToHW
void NNS_G2dApplyOamManagerToHW(OamState *m)
{
    if (m->fast) {
        u32 type = m->type;
        GXOamAttr *p = _021D8518[type].oam + m->first;
        u16 offset = (u16)(m->first * 8);
        u16 size = (u16)((m->last - m->first + 1) * 8);
        DC_FlushRange(p, size);
        switch (type) {
        case 0:
            GX_LoadOAM(p, offset, size);
            break;
        case 1:
            GXS_LoadOAM(p, offset, size);
            break;
        }
    } else {
        LoadOam(m);
        if (valid((Range *)&m->affFirst)) {
            LoadAffine(m);
        }
    }
}
