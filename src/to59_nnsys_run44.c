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

void NNS_G2dInitRndCore(NNSG2dRndCoreInstance *core) {
    MIi_CpuClear16(0, (u16 *)core, 0x70);
    core->pCurrentTargetSurface = NULL;
    core->affineOverwriteMode = NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE;
    core->pImgProxy = NULL;
    core->pPltProxy = NULL;
    core->flipFlag = 0;
    core->bDrawEnable = TRUE;
    core->pCurrentMtxCacheFor2D = NULL;
}

void NNS_G2dSetRndCoreImageProxy(NNSG2dRndCoreInstance *core, const struct NNSG2dImageProxy *img, const struct NNSG2dImagePaletteProxy *pltt) {
    core->pImgProxy = img;
    core->pPltProxy = pltt;
}

void NNS_G2dSetRndCoreOamRegisterFunc(NNSG2dRndCoreInstance *core, NNSG2dOamRegisterFunction oam, NNSG2dAffineRegisterFunction affine) {
    core->pFuncOamRegister = oam;
    core->pFuncOamAffineRegister = affine;
}

void NNS_G2dSetRndCoreAffineOverwriteMode(NNSG2dRndCoreInstance *core, NNSG2dRendererAffineTypeOverwiteMode mode) {
    core->affineOverwriteMode = mode;
}

void NNS_G2dSetRndCoreCurrentMtx3D(const MtxFx32 *matrix) {
    NNSG2dRndCoreInstance *core = _021DA498;
    const fx32 *in = (const fx32 *)matrix;
    fx32 *out = (fx32 *)&core->mtxFor3DGE;
    core->pCurrentMxt = matrix;
    out[0] = in[0];
    out[1] = in[1];
    out[2] = 0;
    out[3] = in[2];
    out[4] = in[3];
    out[5] = 0;
    out[6] = 0;
    out[7] = 0;
    out[8] = FX32_ONE;
    out[9] = in[4];
    out[10] = in[5];
    out[11] = core->zFor3DSoftwareSprite;
}

void NNS_G2dSetRndCoreCurrentMtx2D(const MtxFx32 *matrix, NNSG2dRndCore2DMtxCache *cache) {
    NNSG2dRndCoreInstance *core = _021DA498;
    core->pCurrentMxt = matrix;
    core->pCurrentMtxCacheFor2D = cache;
}

void NNS_G2dSetRndCore3DSoftSpriteZvalue(NNSG2dRndCoreInstance *core, fx32 z) {
    core->zFor3DSoftwareSprite = z;
}

void NNS_G2dSetRndCoreSurface(NNSG2dRndCoreInstance *core, NNSG2dRndCoreSurface *surface) {
    core->pCurrentTargetSurface = surface;
}

BOOL NNS_G2dIsRndCoreFlipH(const NNSG2dRndCoreInstance *core) {
    return (core->flipFlag & 1) != 0;
}

BOOL NNS_G2dIsRndCoreFlipV(const NNSG2dRndCoreInstance *core) {
    return (core->flipFlag & 2) != 0;
}

void NNS_G2dSetRndCoreFlipMode(NNSG2dRndCoreInstance *core, BOOL flipH, BOOL flipV) {
    if (flipH) {
        core->flipFlag |= 1;
    } else {
        core->flipFlag &= ~1;
    }
    if (flipV) {
        core->flipFlag |= 2;
    } else {
        core->flipFlag &= ~2;
    }
}
