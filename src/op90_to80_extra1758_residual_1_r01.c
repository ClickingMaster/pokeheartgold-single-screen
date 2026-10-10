#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct RendererState {
    u16 cacheCount, reserved;
    BOOL translateOnly;
    NNSG2dRendererInstance *renderer;
    s32 matrixIndex;
} RendererState;

typedef struct RendererContext {
    u16 transferIndex, reserved;
    u32 transferHandle;
    MtxFx32 matrix;
} RendererContext;

typedef char StateSizeProof[(sizeof(RendererState) == 16) ? 1 : -1];
typedef char ContextSizeProof[(sizeof(RendererContext) == 32) ? 1 : -1];
typedef char MatrixSizeProof[(sizeof(MtxFx32) == 24) ? 1 : -1];
extern RendererState _021D94D8;
extern MtxFx32 _021D9668[32];
extern fx32 _021D9678[];
extern fx32 _021D967C[];
extern RendererContext _021109DC;
extern MtxFx32 _021109E4;

static inline const MtxFx32 *GetMtxA(void) {
    if (!_021D94D8.translateOnly) {
        return &_021D9668[_021D94D8.matrixIndex];
    }
    _021109DC.matrix._20 = *(fx32 *)((u8 *)_021D9678 + _021D94D8.matrixIndex * sizeof(MtxFx32));
    _021109DC.matrix._21 = *(fx32 *)((u8 *)_021D967C + _021D94D8.matrixIndex * sizeof(MtxFx32));
    return &_021109E4;
}

static inline const MtxFx32 *GetMtxB(void) {
    const MtxFx32 *m;
    if (!_021D94D8.translateOnly) {
        m = &_021D9668[_021D94D8.matrixIndex];
    } else {
        _021109DC.matrix._20 = *(fx32 *)((u8 *)_021D9678 + _021D94D8.matrixIndex * sizeof(MtxFx32));
        _021109DC.matrix._21 = *(fx32 *)((u8 *)_021D967C + _021D94D8.matrixIndex * sizeof(MtxFx32));
        m = &_021109E4;
    }
    return m;
}

static inline const MtxFx32 *GetMtxC(void) {
    s32 i = _021D94D8.matrixIndex;
    if (!_021D94D8.translateOnly) {
        return &_021D9668[i];
    }
    _021109DC.matrix._20 = *(fx32 *)((u8 *)_021D9678 + i * sizeof(MtxFx32));
    _021109DC.matrix._21 = *(fx32 *)((u8 *)_021D967C + i * sizeof(MtxFx32));
    return &_021109E4;
}

void RndCoreCBFuncBeforeCell_(NNSG2dRndCoreInstance *, const NNSG2dCellData *);

void RndCoreCBFuncBeforeCell_(NNSG2dRndCoreInstance *core, const NNSG2dCellData *cell) {
    NNSG2dRenderSurface *surface;
    const MtxFx32 *m;
    surface = _021D94D8.renderer->pCurrentSurface;
    if (surface->pFuncVisibilityCulling) {
        if (_021D94D8.translateOnly) {
            _021109DC.matrix._20 = *(fx32 *)((u8 *)_021D9678 + _021D94D8.matrixIndex * sizeof(MtxFx32));
            _021109DC.matrix._21 = *(fx32 *)((u8 *)_021D967C + _021D94D8.matrixIndex * sizeof(MtxFx32));
            m = &_021109E4;
        } else {
            m = &_021D9668[_021D94D8.matrixIndex];
        }
        if (!surface->pFuncVisibilityCulling(cell, m, &surface->viewRect)) {
            core->bDrawEnable = FALSE;
            return;
        }
        core->bDrawEnable = TRUE;
    }
    if (surface->pBeforeDrawCellBackFunc) {
        m = GetMtxB();
        surface->pBeforeDrawCellBackFunc(_021D94D8.renderer, surface, cell, m);
    }
}
