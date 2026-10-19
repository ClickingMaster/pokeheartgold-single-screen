#ifndef POKEHEARTGOLD_GF_G2D_RENDERER_H
#define POKEHEARTGOLD_GF_G2D_RENDERER_H

#include "global.h"

void GF_InitG2dRenderer(NNSG2dRendererInstance *instance, fx32 zOffset);
void GF_InitG2dRenderSurface(NNSG2dRenderSurface *, NNSG2dViewRect *, NNSG2dOamRegisterFunction, NNSG2dAffineRegisterFunction, NNSG2dRndCellCullingFunction, NNSG2dSurfaceType, NNSG2dRendererInstance *);
void GF_SetG2dRendererSurface(NNSG2dRenderSurface *surface, NNSG2dViewRect *rect);

#endif // POKEHEARTGOLD_GF_G2D_RENDERER_H
