#ifndef POKEHEARTGOLD_RENDER_MODEL_H
#define POKEHEARTGOLD_RENDER_MODEL_H

#include "field/field_effect_manager.h"

typedef struct RenderModel {
    u32 loaded;
    NNSG3dResFileHeader *file;
    NNSG3dResMdlSet *set;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
} RenderModel;

void RenderModel_LoadFromNarc(RenderModelStorage *, int, NARC *, int, enum HeapID, BOOL);
void RenderModel_BindTexturesOnVBlank(RenderModelStorage *);
void RenderModel_Free(RenderModelStorage *data);

#endif // POKEHEARTGOLD_RENDER_MODEL_H
