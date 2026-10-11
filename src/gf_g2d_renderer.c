#include "gf_g2d_renderer.h"

#include "global.h"

// NitroSystem renderer functions not declared in lib/include
void NNS_G2dInitRenderSurface(NNSG2dRenderSurface *surface);
void NNS_G2dAddRendererTargetSurface(NNSG2dRendererInstance *renderer, NNSG2dRenderSurface *surface);

void GF_InitG2dRenderSurface(NNSG2dRenderSurface *surface, NNSG2dViewRect *rect, NNSG2dOamRegisterFunction oam, NNSG2dAffineRegisterFunction affine, NNSG2dRndCellCullingFunction culling, NNSG2dSurfaceType type, NNSG2dRendererInstance *renderer);

void GF_InitG2dRenderer(NNSG2dRendererInstance *instance, fx32 zOffset) {
    NNS_G2dInitRenderer(instance);
    instance->spriteZoffsetStep = zOffset;
}

void GF_InitG2dRenderSurface(NNSG2dRenderSurface *surface, NNSG2dViewRect *rect, NNSG2dOamRegisterFunction oam, NNSG2dAffineRegisterFunction affine, NNSG2dRndCellCullingFunction culling, NNSG2dSurfaceType type, NNSG2dRendererInstance *renderer) {
    NNS_G2dInitRenderSurface(surface);
    surface->viewRect = *rect;
    surface->pFuncOamRegister = oam;
    surface->pFuncOamAffineRegister = affine;
    surface->pFuncVisibilityCulling = culling;
    surface->type = type;
    if (renderer != NULL) {
        NNS_G2dAddRendererTargetSurface(renderer, surface);
    }
}

void GF_SetG2dRendererSurface(NNSG2dRenderSurface *surface, NNSG2dViewRect *rect) {
    surface->viewRect = *rect;
}
