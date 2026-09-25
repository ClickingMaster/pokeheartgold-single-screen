#include <nitro.h>
#include <nitro/code32.h>
#include <nnsys/g2d/g2d_Image.h>
#include <nnsys/g2d/g2d_RendererCore.h>

extern NNSG2dRndCoreInstance *_021DA498;
extern BOOL NNS_G2dIsImageReadyToUse(const NNSG2dImageProxy *, NNS_G2D_VRAM_TYPE);
extern BOOL NNS_G2dIsImagePaletteReadyToUse(const NNSG2dImagePaletteProxy *, NNS_G2D_VRAM_TYPE);

void NNS_G2dInitRndCore(NNSG2dRndCoreInstance *core);
void NNS_G2dSetRndCoreImageProxy(NNSG2dRndCoreInstance *core, const struct NNSG2dImageProxy *img, const struct NNSG2dImagePaletteProxy *pltt);
void NNS_G2dSetRndCoreOamRegisterFunc(NNSG2dRndCoreInstance *core, NNSG2dOamRegisterFunction oam, NNSG2dAffineRegisterFunction affine);
void NNS_G2dSetRndCoreAffineOverwriteMode(NNSG2dRndCoreInstance *core, NNSG2dRendererAffineTypeOverwiteMode mode);
void NNS_G2dSetRndCoreCurrentMtx3D(const MtxFx32 *matrix);
void NNS_G2dSetRndCoreCurrentMtx2D(const MtxFx32 *matrix, NNSG2dRndCore2DMtxCache *cache);
void NNS_G2dSetRndCore3DSoftSpriteZvalue(NNSG2dRndCoreInstance *core, fx32 z);
void NNS_G2dSetRndCoreSurface(NNSG2dRndCoreInstance *core, NNSG2dRndCoreSurface *surface);
BOOL NNS_G2dIsRndCoreFlipH(const NNSG2dRndCoreInstance *core);
BOOL NNS_G2dIsRndCoreFlipV(const NNSG2dRndCoreInstance *core);

extern void DrawCellToSurface3D_(NNSG2dRndCoreSurface *, const NNSG2dCellData *);
extern void DrawCellToSurface2D_(NNSG2dRndCoreSurface *, const NNSG2dCellData *);
void NNS_G2dRndCoreEndRendering(void);
void NNS_G2dRndCoreDrawCell(const NNSG2dCellData *);
void NNS_G2dRndCoreEndRendering(void) {
    NNSG2dRndCoreInstance *core = _021DA498;
    if (core->pCurrentTargetSurface->type == NNS_G2D_SURFACETYPE_MAIN3D) {
        volatile u32 *reg = (volatile u32 *)0x04000440;
        reg[0] = 0;
        reg[2] = 1;
        reg[0] = 1;
    } else {
        core->base2DCharOffset = 0;
        _021DA498->pCurrentMxt = NULL;
        _021DA498->pCurrentMtxCacheFor2D = NULL;
    }
    _021DA498 = NULL;
}

void NNS_G2dRndCoreDrawCell(const NNSG2dCellData *cell) {
    NNSG2dRndCoreSurface *surface = _021DA498->pCurrentTargetSurface;
    if (!surface->bActive) {
        return;
    }
    _021DA498->bDrawEnable = TRUE;
    if (surface->pBeforeDrawCellBackFunc) {
        surface->pBeforeDrawCellBackFunc(_021DA498, cell);
    }
    if (_021DA498->bDrawEnable) {
        switch (surface->type) {
        case NNS_G2D_SURFACETYPE_MAIN3D:
            DrawCellToSurface3D_(surface, cell);
            break;
        case NNS_G2D_SURFACETYPE_MAIN2D:
        case NNS_G2D_SURFACETYPE_SUB2D:
            DrawCellToSurface2D_(surface, cell);
            break;
        case NNS_G2D_SURFACETYPE_MAX:
            break;
        default:
            break;
        }
    }
    if (surface->pAfterDrawCellBackFunc) {
        surface->pAfterDrawCellBackFunc(_021DA498, cell);
    }
}
