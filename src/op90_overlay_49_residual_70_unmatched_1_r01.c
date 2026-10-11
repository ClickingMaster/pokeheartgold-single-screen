#pragma force_active on
#include "global.h"

typedef struct Ov49Model {
    u8 unk0[8];
    void *mdl;
    NNSG3dResTex *tex;
} Ov49Model;

typedef struct Ov49Anim {
    u8 unk0[0x14];
} Ov49Anim;

typedef struct Ov49Global {
    u8 unk0[0x10550];
    Ov49Model models[15];
    NNSG3dResFileHeader *texRes[39];
    Ov49Anim anims[17];
} Ov49Global;

typedef struct Ov49Def {
    u8 model;
    u8 tex;
    u8 anims[2];
} Ov49Def;

typedef struct Ov49Render {
    u8 unk0[0x78];
} Ov49Render;

typedef struct Ov49Local {
    u8 unk0[4];
    u32 polygonId;
    u8 unk8[4];
    Ov49Render renders[18];
    Ov49Def *defs[18];
    void *animObjs[18][2];
} Ov49Local;

BOOL ModelInstance_IsVisible(Ov49Render *render);
BOOL NNS_G3dForceBindMdlTex(void *mdl, NNSG3dResTex *tex, u32 a, u32 b);
BOOL NNS_G3dForceBindMdlPltt(void *mdl, NNSG3dResTex *tex, u32 a, u32 b);
void ModelInstance_AddAnimation(Ov49Render *render, Ov49Anim *anim);
void ModelAnimation_SetFrame(Ov49Anim *anim, void *obj);
void NNS_G3dMdlSetMdlPolygonIDAll(void *mdl, u32 id);
void ModelInstance_Draw(Ov49Render *render);
void ModelInstance_RemoveAnimation(Ov49Render *render, Ov49Anim *anim);
void NNS_G3dReleaseMdlTex(void *mdl);
void NNS_G3dReleaseMdlPltt(void *mdl);
void ov49_022659D0(Ov49Global *global, Ov49Local *local, u32 index);

// @fn ov49_022659D0
void ov49_022659D0(Ov49Global *global, Ov49Local *local, u32 index) {
    Ov49Model *model;
    int i;
    u8 n;

    GF_ASSERT(local->defs[index] != NULL);
    if (ModelInstance_IsVisible(&local->renders[index])) {
        model = &global->models[local->defs[index]->model];
        model->tex = NNS_G3dGetTex(global->texRes[local->defs[index]->tex]);
        GF_ASSERT(NNS_G3dForceBindMdlTex(model->mdl, model->tex, 0, 0));
        GF_ASSERT(NNS_G3dForceBindMdlPltt(model->mdl, model->tex, 0, 0));
        for (i = 0; i < 2; i++) {
            n = local->defs[index]->anims[i];
            if (n == 17) {
                break;
            }
            ModelInstance_AddAnimation((Ov49Render *)((u8 *)local + 0xC + index * 0x78), &global->anims[n]);
            ModelAnimation_SetFrame(&global->anims[n], local->animObjs[index][i]);
        }
        NNS_G3dMdlSetMdlPolygonIDAll(model->mdl, local->polygonId);
        ModelInstance_Draw((Ov49Render *)((u8 *)local + 0xC + index * 0x78));
        for (i = 0; i < 2; i++) {
            n = local->defs[index]->anims[i];
            if (n == 17) {
                break;
            }
            ModelInstance_RemoveAnimation((Ov49Render *)((u8 *)local + 0xC + index * 0x78), &global->anims[n]);
        }
        NNS_G3dReleaseMdlTex(model->mdl);
        NNS_G3dReleaseMdlPltt(model->mdl);
        model->tex = NULL;
    }
}
