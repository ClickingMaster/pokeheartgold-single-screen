#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef NNSG2dRendererInstance Renderer;
typedef struct RendererState {
    u16 cacheCount, reserved;
    BOOL translateOnly;
    Renderer *renderer;
    s32 matrixIndex;
} RendererState;
typedef struct IndexRecord {
    u16 cache, id, state, pad;
} IndexRecord;
extern volatile RendererState _021D94D8;
extern fx32 _021D94E8[32];
extern IndexRecord _021D9568[32];
extern MtxFx32 _021D9668[32];
extern MtxFx32 _021D9968[32];
void NNS_G2dPushMtx(void);

static inline BOOL IsPushable(s32 next) {
    return next < 32 ? TRUE : FALSE;
}

static inline void Push(s32 next) {
    if (_021D94D8.translateOnly) {
        s32 cur = _021D94D8.matrixIndex;
        _021D9668[next]._20 = _021D9668[cur]._20;
        _021D9668[next]._21 = _021D9668[cur]._21;
        _021D94E8[next] = _021D94E8[cur];
    } else {
        s32 cur = _021D94D8.matrixIndex;
        _021D9668[next] = _021D9668[cur];
        _021D9968[next] = _021D9968[cur];
        _021D94E8[next] = _021D94E8[cur];
    }
    _021D94D8.matrixIndex = next;
}

#pragma push
#pragma opt_propagation off
// @fn NNS_G2dPushMtx
void NNS_G2dPushMtx(void) {
    if (!(_021D94D8.renderer->opzHint & 1)) {
        s32 idx = _021D94D8.matrixIndex;
        u16 prev = (u16)idx;
        s32 next = idx + 1;
        u16 now;
        if (IsPushable(next)) {
            Push(next);
        }
        now = (u16)_021D94D8.matrixIndex;
        _021D9568[now] = _021D9568[prev];
        if (_021D9568[prev].state == 1) {
            _021D9568[now].state = 2;
        } else {
            _021D9568[now].state = _021D9568[prev].state;
        }
    } else {
        s32 next = _021D94D8.matrixIndex + 1;
        if (IsPushable(next)) {
            Push(next);
        }
    }
}
#pragma pop
