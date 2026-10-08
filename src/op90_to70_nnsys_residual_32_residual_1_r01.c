#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <nnsys/g2d/g2d_RendererCore.h>

extern NNSG2dRndCoreInstance *_021DA498;
extern const MtxFx32 _021093CC;
extern const u16 _02109310[4][4];
extern const u16 _021092F8[4][4];

BOOL NNS_G2dIsRndCoreFlipH(const NNSG2dRndCoreInstance *pRnd);
BOOL NNS_G2dIsRndCoreFlipV(const NNSG2dRndCoreInstance *pRnd);
void DoAffineTransforme_(const MtxFx22 *mtx, GXOamAttr *oam, const NNSG2dFVec2 *pos);

static inline BOOL IsAffineEnable(const NNSG2dRndCoreInstance *pRnd) {
    if (pRnd->flipFlag == 0 && pRnd->pCurrentMtxCacheFor2D != NULL) {
        return TRUE;
    }
    return FALSE;
}

void DrawCellToSurface2D_(const NNSG2dRndCoreSurface *pSurface, const NNSG2dCellData *pCell);

// @fn DrawCellToSurface2D_
void DrawCellToSurface2D_(const NNSG2dRndCoreSurface *pSurface, const NNSG2dCellData *pCell) {
    NNSG2dRndCoreInstance *pRnd;
    BOOL bAffine;
    u16 i;
    u32 baseOffset;
    NNSG2dFVec2 pos;
    const MtxFx32 *pMtx;

    pRnd = _021DA498;
    if (pRnd->pCurrentMxt != NULL) {
        pMtx = pRnd->pCurrentMxt;
    } else {
        pMtx = &_021093CC;
    }

    pos.x = pMtx->_20;
    pos.y = pMtx->_21;
    pos.x -= pSurface->viewRect.posTopLeft.x;
    pos.y -= pSurface->viewRect.posTopLeft.y;

    baseOffset = _021DA498->base2DCharOffset;
    bAffine = IsAffineEnable(_021DA498);

    for (i = 0; i < pCell->numOAMAttrs; i++) {
        u16 affineIndex;

        const NNSG2dCellOAMAttrData *pSrc;

        _021DA498->bDrawEnable = TRUE;
        pSrc = &pCell->pOamAttrArray[i];
        pRnd->currentOam.attr0 = pSrc->attr0;
        pRnd->currentOam.attr1 = pSrc->attr1;
        pRnd->currentOam.attr2 = pSrc->attr2;

        if (pSurface->pBeforeDrawOamBackFunc != NULL) {
            pSurface->pBeforeDrawOamBackFunc(_021DA498, pCell, i);
        }
        if (_021DA498->bDrawEnable) {
            pRnd->currentOam.charNo += baseOffset;
            if (bAffine) {
                NNSG2dRndCore2DMtxCache *pCache;
                MtxFx22 mtx22;
                u32 flip = (pRnd->currentOam.flipV << 1) | pRnd->currentOam.flipH;
                NNSG2dRndCoreInstance *pCore = _021DA498;
                pCache = pCore->pCurrentMtxCacheFor2D;

                affineIndex = pCache->affineIndex[flip][pSurface->type - 1];
                if (affineIndex == MtxCache_NOT_AVAILABLE) {
                    if (flip == 0) {
                        affineIndex = pCore->pFuncOamAffineRegister(&pCache->m22);
                    } else {
                        MtxFx22 mtx = pCache->m22;
                        if (flip & 1) {
                            mtx._00 = -mtx._00;
                            mtx._01 = -mtx._01;
                        }
                        if (flip & 2) {
                            mtx._10 = -mtx._10;
                            mtx._11 = -mtx._11;
                        }
                        affineIndex = pCore->pFuncOamAffineRegister(&mtx);
                    }
                    pCache->affineIndex[flip][pSurface->type - 1] = affineIndex;
                }
                mtx22._00 = pMtx->_00;
                mtx22._01 = pMtx->_01;
                mtx22._10 = pMtx->_10;
                mtx22._11 = pMtx->_11;
                DoAffineTransforme_(&mtx22, &pRnd->currentOam, &pos);
            } else {
                affineIndex = 0xFFFE;
                if (_021DA498->flipFlag) {
                    u32 shape = pRnd->currentOam.attr01 & 0xC000C000;
                    u32 h, v;
                    h = pRnd->currentOam.flipH;
                    h = NNS_G2dIsRndCoreFlipH(_021DA498) ^ h;
                    v = pRnd->currentOam.flipV;
                    v = NNS_G2dIsRndCoreFlipV(_021DA498) ^ v;
                    pRnd->currentOam.attr01 = (((pRnd->currentOam.attr01 & ~0x10000000) | (h << 28)) & ~0x20000000) | (v << 29);
                    if (NNS_G2dIsRndCoreFlipH(_021DA498)) {
                        pRnd->currentOam.attr01 = (pRnd->currentOam.attr01 & 0xFE00FFFF) | (((u32)(-(_02109310[(s32)(shape & 0xC000) >> 14][(shape & 0xC0000000) >> 30] + pRnd->currentOam.x)) & 0x1FF) << 16);
                    }
                    if (NNS_G2dIsRndCoreFlipV(_021DA498)) {
                        pRnd->currentOam.attr01 = (pRnd->currentOam.attr01 & ~0xFF) | ((u32)(-(_021092F8[(s32)(shape & 0xC000) >> 14][(shape & 0xC0000000) >> 30] + pRnd->currentOam.y)) & 0xFF);
                    }
                }
                pRnd->currentOam.x = (pos.x >> FX32_SHIFT) + pRnd->currentOam.x;
                pRnd->currentOam.y = (pos.y >> FX32_SHIFT) + pRnd->currentOam.y;
            }
            if (_021DA498->pFuncOamRegister(&pRnd->currentOam, affineIndex, FALSE) != TRUE) {
                return;
            }
            if (pSurface->pAfterDrawOamBackFunc != NULL) {
                pSurface->pAfterDrawOamBackFunc(_021DA498, pCell, i);
            }
        }
    }
}
