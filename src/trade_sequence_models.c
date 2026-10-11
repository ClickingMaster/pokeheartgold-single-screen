#include "trade_sequence_models.h"

#include "global.h"

#include "camera.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "model_box_test.h"

void TradeSequence_DrawModel(TradeSequenceModel *model);
void TradeSequence_InitModel(TradeSequenceModel *model);
void TradeSequence_FreeModel(TradeSequenceModel *model);
void TradeSequence_InitCamera(TradeSequenceModels *models, fx32 targetX, fx32 targetY, fx32 targetZ);
void TradeSequence_FreeCamera(TradeSequenceModels *models);

TradeSequenceModels *TradeSequence_CreateModels(u32 count, fx32 targetX, fx32 targetY, fx32 targetZ) {
    TradeSequenceModels *models = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceModels));
    u32 i;

    if (models != NULL) {
        TradeSequence_InitCamera(models, targetX, targetY, targetZ);
        models->models = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceModel) * count);
        models->count = count;
        for (i = 0; i < count; i++) {
            TradeSequence_InitModel(&models->models[i]);
        }
    }
    return models;
}

void TradeSequence_DestroyModels(TradeSequenceModels *models) {
    u32 i;

    for (i = 0; i < models->count; i++) {
        TradeSequence_FreeModel(&models->models[i]);
    }
    Heap_Free(models->models);
    TradeSequence_FreeCamera(models);
    Heap_Free(models);
}

void TradeSequence_DrawModels(TradeSequenceModels *models) {
    u32 i;
    u32 one;

    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    Camera_PushLookAtToNNSGlb();
    for (i = 0; i < models->count; i++) {
        if (models->models[i].visible != FALSE) {
            TradeSequence_DrawModel(&models->models[i]);
        }
    }
    one = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, &one, 1);
}

void TradeSequence_DrawModel(TradeSequenceModel *model) {
    MtxFx33 rotation;
    MtxFx33 temp;

    MTX_Identity33_(&rotation);
    MTX_RotX33_(&temp, FX_SinIdx((u16)model->rotation.x), FX_CosIdx((u16)model->rotation.x));
    MTX_Concat33(&temp, &rotation, &rotation);
    MTX_RotY33_(&temp, FX_SinIdx((u16)model->rotation.y), FX_CosIdx((u16)model->rotation.y));
    MTX_Concat33(&temp, &rotation, &rotation);
    MTX_RotZ33_(&temp, FX_SinIdx((u16)model->rotation.z), FX_CosIdx((u16)model->rotation.z));
    MTX_Concat33(&temp, &rotation, &rotation);
    if (model->alpha != 31) {
        NNS_G3dGlbPolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_ALL, 0, model->alpha, 0);
    }
    GF3dRender_DrawModel(&model->renderObj, &model->translation, &rotation, &model->scale);
}

void TradeSequence_InitModel(TradeSequenceModel *model) {
    model->visible = FALSE;
    model->file = NULL;
    model->rotation.x = model->rotation.y = model->rotation.z = 0;
    model->scale.x = model->scale.y = model->scale.z = FX32_ONE;
    model->translation.x = model->translation.y = model->translation.z = 0;
}

void TradeSequence_FreeModel(TradeSequenceModel *model) {
    if (model->file != NULL) {
        Heap_Free(model->file);
        model->file = NULL;
        model->visible = FALSE;
    }
}

#pragma force_active on

TradeSequenceModel *TradeSequence_LoadModel(TradeSequenceModels *models, int index, NarcId narcId, s32 fileId, fx32 x, fx32 y, fx32 z, BOOL visible) {
    TradeSequenceModel *base = models->models;
    TradeSequenceModel *model = &base[index];

    base[index].file = GfGfxLoader_LoadFromNarc(narcId, fileId, FALSE, HEAP_ID_57, TRUE);
    DC_FlushRange(base[index].file, GetNarcMemberSizeByIdPair(narcId, fileId));
    if (base[index].file) {
        model->mdlSet = NNS_G3dGetMdlSet(base[index].file);
        model->mdl = NNS_G3dGetMdlByIdx(model->mdlSet, 0);
        model->tex = NNS_G3dGetTex(model->file);
        GF3dRender_AllocAndLoadTexResources(model->tex);
        GF3dRender_BindModelSet(model->file, model->tex);
        NNS_G3dRenderObjInit(&model->renderObj, model->mdl);
        model->translation.x = x;
        model->translation.y = y;
        model->translation.z = z;
        model->alpha = 31;
        model->visible = visible;
    }
    return model;
}

#pragma force_active reset

void TradeSequence_GetModelPosition(TradeSequenceModel *model, VecFx32 *position) {
    *position = model->translation;
}

void TradeSequence_SetModelPosition(TradeSequenceModel *model, const VecFx32 *position) {
    model->translation = *position;
}

void TradeSequence_GetModelRotation(TradeSequenceModel *model, TradeSequenceRotation *rotation) {
    // copied as unsigned halfwords, as in the original
    rotation->x = *(u16 *)&model->rotation.x;
    rotation->y = *(u16 *)&model->rotation.y;
    rotation->z = *(u16 *)&model->rotation.z;
}

void TradeSequence_SetModelRotation(TradeSequenceModel *model, const TradeSequenceRotation *rotation) {
    // copied as unsigned halfwords, as in the original
    model->rotation.x = *(const u16 *)&rotation->x;
    model->rotation.y = *(const u16 *)&rotation->y;
    model->rotation.z = *(const u16 *)&rotation->z;
}

void TradeSequence_SetModelVisible(TradeSequenceModel *model, BOOL visible) {
    model->visible = visible;
}

void TradeSequence_SetModelAlpha(TradeSequenceModel *model, u32 alpha) {
    model->alpha = alpha;
    if (alpha != 31) {
        NNSi_G3dModifyPolygonAttrMask(model->mdl, FALSE, REG_G3_POLYGON_ATTR_ALPHA_MASK);
    } else {
        NNSi_G3dModifyPolygonAttrMask(model->mdl, TRUE, REG_G3_POLYGON_ATTR_ALPHA_MASK);
    }
}

void TradeSequence_SetModelScale(TradeSequenceModel *model, fx32 scale) {
    model->scale.x = scale;
    model->scale.y = scale;
    model->scale.z = scale;
}

#pragma force_active on

BOOL TradeSequence_IsModelInView(TradeSequenceModel *model) {
    MtxFx33 matrix;
    MtxFx33 rot;
    int one;
    int result;

    MTX_Identity33(&matrix);
    MTX_RotX33(&rot, FX_SinIdx((u16)model->rotation.x), FX_CosIdx((u16)model->rotation.x));
    MTX_Concat33(&rot, &matrix, &matrix);
    MTX_RotY33(&rot, FX_SinIdx((u16)model->rotation.y), FX_CosIdx((u16)model->rotation.y));
    MTX_Concat33(&rot, &matrix, &matrix);
    MTX_RotZ33(&rot, FX_SinIdx((u16)model->rotation.z), FX_CosIdx((u16)model->rotation.z));
    MTX_Concat33(&rot, &matrix, &matrix);
    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    Camera_PushLookAtToNNSGlb();
    result = GF3d_ModelBoxTest(model->mdl, &model->translation, &matrix, &model->scale);
    one = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, (u32 *)&one, 1);
    return result;
}

#pragma force_active reset

void TradeSequence_GetCameraPos(TradeSequenceModels *models, VecFx32 *pos) {
    VecFx32 camPos;
    camPos = Camera_GetLookAtCamPos(models->camera);
    *pos = camPos;
}

void TradeSequence_SetCameraAngle(TradeSequenceModels *models, const CameraAngle *angle) {
    Camera_SetAnglePos(angle, models->camera);
}

void TradeSequence_AdjustCameraAngle(TradeSequenceModels *models, const CameraAngle *delta) {
    Camera_AdjustAngleTarget(delta, models->camera);
}

void TradeSequence_SetCameraPerspectiveType(TradeSequenceModels *models, u8 perspectiveType) {
    Camera_ApplyPerspectiveType(perspectiveType, models->camera);
}

void TradeSequence_SetCameraPerspectiveAngle(TradeSequenceModels *models, u16 perspectiveAngle) {
    Camera_SetPerspectiveAngle(perspectiveAngle, models->camera);
}

#pragma force_active on

void TradeSequence_InitCamera(TradeSequenceModels *models, fx32 targetX, fx32 targetY, fx32 targetZ) {
    VecFx32 up;

    models->camera = Camera_New(HEAP_ID_57);
    models->cameraTarget.x = targetX;
    models->cameraTarget.y = targetY;
    models->cameraTarget.z = targetZ;
    models->cameraAngle.x = 0;
    models->cameraAngle.y = 0;
    models->cameraAngle.z = 0;
    Camera_Init_FromTargetDistanceAndAngle(&models->cameraTarget, FX32_CONST(300), &models->cameraAngle, 4004, 0, TRUE, models->camera);
    up.x = 0;
    up.y = FX32_ONE;
    up.z = 0;
    Camera_SetLookAtCamUp(&up, models->camera);
    Camera_SetStaticPtr(models->camera);
    Camera_SetPerspectiveClippingPlane(0, FX32_CONST(1000), models->camera);
}

#pragma force_active reset

void TradeSequence_FreeCamera(TradeSequenceModels *models) {
    Camera_UnsetStaticPtr();
    Camera_Delete(models->camera);
}
