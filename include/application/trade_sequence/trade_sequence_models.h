#ifndef POKEHEARTGOLD_APPLICATION_TRADE_SEQUENCE_TRADE_SEQUENCE_MODELS_H
#define POKEHEARTGOLD_APPLICATION_TRADE_SEQUENCE_TRADE_SEQUENCE_MODELS_H

#include "camera.h"
#include "filesystem.h"

// 3D models of the trade animation with the camera they are drawn through.

typedef struct TradeSequenceRotation {
    s16 x;
    s16 y;
    s16 z;
} TradeSequenceRotation;

typedef struct TradeSequenceModel {
    NNSG3dResFileHeader *file;
    NNSG3dRenderObj renderObj;
    NNSG3dResMdlSet *mdlSet;
    NNSG3dResMdl *mdl;
    NNSG3dResTex *tex;
    BOOL visible;
    VecFx32 translation;
    VecFx32 scale;
    TradeSequenceRotation rotation;
    u16 padding_86;
    u32 alpha; // 31: opaque
} TradeSequenceModel;

typedef struct TradeSequenceModels {
    Camera *camera;
    VecFx32 cameraTarget;
    CameraAngle cameraAngle;
    TradeSequenceModel *models;
    u32 count;
} TradeSequenceModels;

TradeSequenceModels *TradeSequence_CreateModels(u32 count, fx32 targetX, fx32 targetY, fx32 targetZ);
void TradeSequence_DestroyModels(TradeSequenceModels *models);
void TradeSequence_DrawModels(TradeSequenceModels *models);
TradeSequenceModel *TradeSequence_LoadModel(TradeSequenceModels *models, int index, NarcId narcId, s32 fileId, fx32 x, fx32 y, fx32 z, BOOL visible);
void TradeSequence_GetModelPosition(TradeSequenceModel *model, VecFx32 *position);
void TradeSequence_SetModelPosition(TradeSequenceModel *model, const VecFx32 *position);
void TradeSequence_GetModelRotation(TradeSequenceModel *model, TradeSequenceRotation *rotation);
void TradeSequence_SetModelRotation(TradeSequenceModel *model, const TradeSequenceRotation *rotation);
void TradeSequence_SetModelVisible(TradeSequenceModel *model, BOOL visible);
void TradeSequence_SetModelAlpha(TradeSequenceModel *model, u32 alpha);
void TradeSequence_SetModelScale(TradeSequenceModel *model, fx32 scale);
BOOL TradeSequence_IsModelInView(TradeSequenceModel *model);
void TradeSequence_GetCameraPos(TradeSequenceModels *models, VecFx32 *pos);
void TradeSequence_SetCameraAngle(TradeSequenceModels *models, const CameraAngle *angle);
void TradeSequence_AdjustCameraAngle(TradeSequenceModels *models, const CameraAngle *delta);
void TradeSequence_SetCameraPerspectiveType(TradeSequenceModels *models, u8 perspectiveType);
void TradeSequence_SetCameraPerspectiveAngle(TradeSequenceModels *models, u16 perspectiveAngle);

#endif // POKEHEARTGOLD_APPLICATION_TRADE_SEQUENCE_TRADE_SEQUENCE_MODELS_H
