#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on

typedef NNSG2dRenderSurface Surface;
typedef NNSG2dRendererInstance Renderer;
typedef NNSG2dRndCore2DMtxCache MtxCache;

typedef struct RendererState {
    u16 cacheCount, reserved;
    BOOL translateOnly;
    Renderer *renderer;
    s32 matrixIndex;
    u8 gap[0xf94 - 0x10];
    BOOL cacheEnabled;
} RendererState;
typedef struct RendererContext {
    u16 transferIndex, reserved;
    u32 transferHandle;
    MtxFx32 matrix;
} RendererContext;
typedef struct IndexRecord {
    u16 cache, id, state, pad;
} IndexRecord;
typedef struct McmState {
    u8 pad[0x90];
    u16 index;
} McmState;
typedef struct McmTable {
    u16 index, pad;
    MtxCache *values[256];
} McmTable;

extern RendererState _021D94D8;
extern RendererContext _021109DC;
extern MtxFx32 _021109E4;
extern McmState _021D9FD8;
extern MtxCache *_021DA06C[256];
extern McmTable _021DA068;
extern IndexRecord _021D9568[32];
extern MtxCache _021D9C68[32];
extern MtxFx32 _021D9668[32];
extern fx32 _021D9678[], _021D967C[];
extern fx32 _021D94E8[32];

void NNSi_G2dMCMStoreCurrentMtxToMtxCache(void);
void NNS_G2dSetRndCoreCurrentMtx2D(const MtxFx32 *, MtxCache *);
void NNS_G2dSetRndCoreCurrentMtx3D(const MtxFx32 *);
void NNS_G2dRndCoreDrawCellVramTransfer(const NNSG2dCellData *, u32);
void NNS_G2dRndCoreDrawCell(const NNSG2dCellData *);
void NNS_G2dSetRndCore3DSoftSpriteZvalue(NNSG2dRndCoreInstance *, fx32);
void NNS_G2dSetRndCoreSurface(NNSG2dRndCoreInstance *, NNSG2dRndCoreSurface *);
void NNS_G2dRndCoreBeginRendering(NNSG2dRndCoreInstance *);
void NNS_G2dRndCoreEndRendering(void);

static inline BOOL IsCacheUsable(void) {
    if (_021D94D8.translateOnly == 0 && _021109DC.transferIndex != 0xFFFE) {
        return TRUE;
    }
    return FALSE;
}
static inline BOOL IsValidHandle(u32 handle) {
    return handle != (u32)-1;
}
static inline void DrawCoreCell(const NNSG2dCellData *cell) {
    switch (IsValidHandle(_021109DC.transferHandle)) {
    default:
        NNS_G2dRndCoreDrawCellVramTransfer(cell, _021109DC.transferHandle);
        break;
    case FALSE:
        NNS_G2dRndCoreDrawCell(cell);
        break;
    }
}
static inline MtxCache *GetCacheA(void) {
    MtxCache *cache = NULL;
    if (IsCacheUsable()) {
        if (_021D94D8.cacheEnabled) {
            cache = _021DA06C[_021D9FD8.index];
            if (cache == NULL) {
                NNSi_G2dMCMStoreCurrentMtxToMtxCache();
                cache = &_021D9C68[_021D9568[(u16)_021D94D8.matrixIndex].cache];
                _021DA06C[_021D9FD8.index] = cache;
            }
        } else {
            NNSi_G2dMCMStoreCurrentMtxToMtxCache();
            cache = &_021D9C68[_021D9568[(u16)_021D94D8.matrixIndex].cache];
        }
    }
    return cache;
}
static inline MtxCache *GetCacheB(void) {
    MtxCache *cache = NULL;
    if (IsCacheUsable()) {
        if (_021D94D8.cacheEnabled) {
            cache = _021DA068.values[_021D9FD8.index];
            if (cache == NULL) {
                NNSi_G2dMCMStoreCurrentMtxToMtxCache();
                cache = &_021D9C68[_021D9568[(u16)_021D94D8.matrixIndex].cache];
                _021DA068.values[_021D9FD8.index] = cache;
            }
        } else {
            NNSi_G2dMCMStoreCurrentMtxToMtxCache();
            cache = &_021D9C68[_021D9568[(u16)_021D94D8.matrixIndex].cache];
        }
    }
    return cache;
}
static inline const MtxFx32 *GetMtxA(void) {
    if (_021D94D8.translateOnly == 0) {
        return &_021D9668[_021D94D8.matrixIndex];
    }
    _021109DC.matrix._20 = *(fx32 *)((u8 *)_021D9678 + _021D94D8.matrixIndex * sizeof(MtxFx32));
    _021109DC.matrix._21 = *(fx32 *)((u8 *)_021D967C + _021D94D8.matrixIndex * sizeof(MtxFx32));
    return &_021109E4;
}
static inline const MtxFx32 *GetMtxB(void) {
    if (_021D94D8.translateOnly) {
        _021109DC.matrix._20 = _021D9668[_021D94D8.matrixIndex]._20;
        _021109DC.matrix._21 = _021D9668[_021D94D8.matrixIndex]._21;
        return &_021109E4;
    }
    return &_021D9668[_021D94D8.matrixIndex];
}

static inline void Draw2DA(const NNSG2dCellData *cell) {
    MtxCache *cache = GetCacheA();
    NNS_G2dSetRndCoreCurrentMtx2D(GetMtxA(), cache);
    DrawCoreCell(cell);
}
static inline void Draw3DA(Renderer *renderer, const NNSG2dCellData *cell) {
    NNS_G2dSetRndCore3DSoftSpriteZvalue(&renderer->rendererCore, _021D94E8[_021D94D8.matrixIndex]);
    NNS_G2dSetRndCoreCurrentMtx3D(GetMtxA());
    DrawCoreCell(cell);
}
static inline void Draw2DB(const NNSG2dCellData *cell) {
    MtxCache *cache = GetCacheB();
    NNS_G2dSetRndCoreCurrentMtx2D(GetMtxB(), cache);
    DrawCoreCell(cell);
}
static inline void Draw3DB(Renderer *renderer, const NNSG2dCellData *cell) {
    NNS_G2dSetRndCore3DSoftSpriteZvalue(&renderer->rendererCore, _021D94E8[_021D94D8.matrixIndex]);
    NNS_G2dSetRndCoreCurrentMtx3D(GetMtxB());
    DrawCoreCell(cell);
}
static inline void EndSurface(void) {
    _021D94D8.renderer->pCurrentSurface = NULL;
    NNS_G2dRndCoreEndRendering();
}

void DrawCellImpl_(const NNSG2dCellData *cell);

// @fn DrawCellImpl_
void DrawCellImpl_(const NNSG2dCellData *cell) {
    Renderer *renderer = _021D94D8.renderer;
    Surface *surface = renderer->pTargetSurfaceList;
    if (renderer->opzHint & 2) {
        if (surface->type != NNS_G2D_SURFACETYPE_MAIN3D) {
            Draw2DA(cell);
        } else {
            Draw3DA(renderer, cell);
        }
        return;
    }
    for (; surface != NULL; surface = surface->pNextSurface) {
        if (surface->bActive) {
            if (surface->type != NNS_G2D_SURFACETYPE_MAIN3D) {
                Renderer *r = _021D94D8.renderer;
                r->pCurrentSurface = surface;
                NNS_G2dSetRndCoreSurface(&r->rendererCore, &surface->coreSurface);
                NNS_G2dSetRndCoreOamRegisterFunc(&r->rendererCore, surface->pFuncOamRegister, surface->pFuncOamAffineRegister);
                NNS_G2dRndCoreBeginRendering(&r->rendererCore);
                Draw2DB(cell);
                EndSurface();
            } else {
                Renderer *r = _021D94D8.renderer;
                r->pCurrentSurface = surface;
                NNS_G2dSetRndCoreSurface(&r->rendererCore, &surface->coreSurface);
                NNS_G2dRndCoreBeginRendering(&r->rendererCore);
                Draw3DB(renderer, cell);
                EndSurface();
            }
        }
    }
}
