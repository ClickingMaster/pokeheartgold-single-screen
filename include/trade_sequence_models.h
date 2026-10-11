#ifndef POKEHEARTGOLD_TRADE_SEQUENCE_MODELS_H
#define POKEHEARTGOLD_TRADE_SEQUENCE_MODELS_H

#include "camera.h"
#include "filesystem.h"

// 3D models of the trade animation (OVY_71) with the camera they are drawn through.

typedef struct TradeSequenceRotation {
    s16 x;               // 0x0
    s16 y;               // 0x2
    s16 z;               // 0x4
} TradeSequenceRotation; // size: 0x6

typedef struct TradeSequenceModel {
    NNSG3dResFileHeader *file;      // 0x00
    NNSG3dRenderObj renderObj;      // 0x04
    NNSG3dResMdlSet *mdlSet;        // 0x58
    NNSG3dResMdl *mdl;              // 0x5C
    NNSG3dResTex *tex;              // 0x60
    BOOL visible;                   // 0x64
    VecFx32 translation;            // 0x68
    VecFx32 scale;                  // 0x74
    TradeSequenceRotation rotation; // 0x80
    u16 padding_86;                 // 0x86
    u32 alpha;                      // 0x88, 31: opaque
} TradeSequenceModel;               // size: 0x8C

typedef struct TradeSequenceModels {
    Camera *camera;             // 0x00
    VecFx32 cameraTarget;       // 0x04
    CameraAngle cameraAngle;    // 0x10
    TradeSequenceModel *models; // 0x18
    u32 count;                  // 0x1C
} TradeSequenceModels;          // size: 0x20

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

#endif // POKEHEARTGOLD_TRADE_SEQUENCE_MODELS_H
