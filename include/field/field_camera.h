#ifndef POKEHEARTGOLD_FIELD_FIELD_CAMERA_H
#define POKEHEARTGOLD_FIELD_FIELD_CAMERA_H

#include "camera.h"
#include "field_system.h"
#include "heap.h"

typedef struct FieldCameraShift FieldCameraShift;

void FieldCamera_Create(const VecFx32 *target, FieldSystem *fieldSystem, u32 presetNo, BOOL withHistory);
void FieldCamera_Delete(FieldSystem *fieldSystem);
void FieldCameraShift_Start(FieldCameraShift *shift, u8 mode);
void FieldCameraShift_Update(FieldCameraShift *shift);

#endif // POKEHEARTGOLD_FIELD_FIELD_CAMERA_H
