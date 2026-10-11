#pragma force_active on
#include "global.h"

#include "field/field_effect_manager_internal.h"

NNSG3dResTex *FieldEffectManager_GetTexture(FieldEffectManager *manager, u32 id) {
    NNSG3dResTex *tex;
    GF_3DGfxRawResObj *texObj = GF3dGfxRawResMan_GetObjById(manager->gfx->resMan, id);
    tex = NULL;
    GF_ASSERT(texObj);
    if (texObj != NULL) {
        tex = GF3dGfxRawResObj_GetTex(texObj);
    }
    return tex;
}
