#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct { u16 cacheCount, reserved; BOOL translateOnly; NNSG2dRendererInstance *renderer; s32 matrixIndex; } RendererState;
typedef struct { u16 transferIndex, reserved; u32 transferHandle; MtxFx32 matrix; } RendererContext;
typedef struct { u16 cache, id, state, pad; } Entry;
extern RendererState _021D94D8;
extern Entry _021D9568[];
extern NNSG2dRndCore2DMtxCache _021D9C68[];
extern MtxFx32 _021D9668[32];
extern MtxFx32 _021D9968[32];
extern RendererContext _021109DC;
void NNSi_G2dMCMStoreCurrentMtxToMtxCache(void);
static inline BOOL IsValid(const Entry *e) {
    BOOL valid = TRUE;
    if (e->state != 1 && e->state != 2) {
        valid = FALSE;
    }
    return valid;
}
static inline void SetCache(NNSG2dRndCore2DMtxCache *d, const MtxFx32 *m) {
    d->m22._00 = m->_00;
    d->m22._01 = m->_01;
    d->m22._10 = m->_10;
    d->m22._11 = m->_11;
}
static inline void InitCache(NNSG2dRndCore2DMtxCache *p) {
    MIi_CpuClearFast(0xFFFFFFFF, (u32 *)p->affineIndex, sizeof(p->affineIndex));
}

#pragma push
#pragma opt_arithtransformation off
#pragma opt_propagation off

// @fn NNSi_G2dMCMStoreCurrentMtxToMtxCache
void NNSi_G2dMCMStoreCurrentMtxToMtxCache(void) {
    Entry *e = &_021D9568[(u16)_021D94D8.matrixIndex];
    if (IsValid(e)) {
        RendererState *st;
        u16 id;
        u16 slot = _021D94D8.cacheCount;
        const MtxFx32 *m;
        int i;
        if (slot < 31) {
            _021D94D8.cacheCount = slot + 1;
        }
        id = e->id;
        e->cache = slot;
        InitCache(&_021D9C68[slot]);
        if (!_021D94D8.translateOnly) {
            m = &_021D9968[_021D94D8.matrixIndex];
        } else {
            _021109DC.matrix._20 = _021D9668[_021D94D8.matrixIndex]._20;
            _021109DC.matrix._21 = _021D9668[_021D94D8.matrixIndex]._21;
            m = &_021109DC.matrix;
        }
        {
            NNSG2dRndCore2DMtxCache *d = _021D9C68;
            u16 c = _021D9568[(u16)_021D94D8.matrixIndex].cache;
            d[c].m22._00 = m->_00;
            d += c;
            d->m22._01 = m->_01;
            st = &_021D94D8;
            d->m22._10 = m->_10;
            d->m22._11 = m->_11;
        }
        for (i = (u16)st->matrixIndex; i >= 0; i--) {
            if (_021D9568[i].id != id) {
                return;
            }
            _021D9568[i].state = 3;
            _021D9568[i].cache = slot;
        }
    }
}
#pragma pop
