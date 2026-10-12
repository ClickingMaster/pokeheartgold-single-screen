#ifndef POKEHEARTGOLD_FIELD_FIELD_CAMERA_H
#define POKEHEARTGOLD_FIELD_FIELD_CAMERA_H

#include "camera.h"
#include "field_system.h"
#include "heap.h"

typedef struct FieldCameraShift FieldCameraShift;

void FieldCamera_Create(const VecFx32 *target, FieldSystem *fieldSystem, u32 presetNo, BOOL withHistory);
void FieldCamera_Delete(FieldSystem *fieldSystem);
FieldCameraShift *FieldCameraShift_New(enum HeapID heapID);
void FieldCameraShift_Free(FieldCameraShift *shift);
void FieldCamera_InitShift(FieldSystem *fieldSystem, u32 presetNo);
void FieldCameraShift_Start(FieldCameraShift *shift, u8 mode);
void FieldCameraShift_Update(FieldCameraShift *shift);
void FieldCameraShift_SetAngle(Camera *camera, const u16 *start, const u16 *end, u32 step, u8 total);
void FieldCameraShift_MoveLookAt(Camera *camera, const VecFx32 *offset, u32 step, u32 total);
s32 FieldCameraShift_GetStepDelta(s32 value, s32 step, s32 total);

#endif // POKEHEARTGOLD_FIELD_FIELD_CAMERA_H
