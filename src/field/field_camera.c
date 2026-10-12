#include "field/field_camera.h"

#include "global.h"

#include "camera.h"
#include "field_system.h"
#include "heap.h"

#define FIELD_CAMERA_PRESET_COUNT 17

typedef struct FieldCameraPreset {
    fx32 distance;
    CameraAngle angle;
    u16 perspectiveType;
    u16 perspectiveAngle;
    fx32 clipNear;
    fx32 clipFar;
    VecFx32 lookAtOffset;
} FieldCameraPreset;

typedef struct FieldCameraShiftMotion {
    u16 angle;
    u16 padding;
    VecFx32 lookAtOffset;
} FieldCameraShiftMotion;

typedef struct FieldCameraShiftPreset {
    FieldCameraShiftMotion motion;
    u32 duration;
} FieldCameraShiftPreset;

// A timed camera shift (script command 610): the camera's x angle moves to the preset's angle and the look-at
// point by the preset's offset; starting the same shift again (or while it runs) moves the camera back.
struct FieldCameraShift {
    Camera *camera;
    u8 duration;                   // 0x04
    u8 step;                       // 0x05
    u8 padding6;                   // 0x06
    u8 mode;                       // 0x07: preset number + 1
    FieldCameraShiftMotion motion; // 0x08: target angle, offset still to apply
    u8 filler_18[0x10];            // 0x18
    BOOL active;                   // 0x28
    u16 startAngle;                // 0x2C
    u16 reverse;                   // 0x2E
    u32 presetNo;                  // 0x30
};

// The shift mode that is in effect; it survives the camera being rebuilt (FieldCamera_Create reapplies it).
extern u32 gFieldCameraShiftMode;
extern const FieldCameraShiftPreset gFieldCameraShiftPresets[];
extern const FieldCameraPreset gFieldCameraPresets[];

void FieldCamera_Create(const VecFx32 *target, FieldSystem *fieldSystem, u32 presetNo, BOOL withHistory) {
    const FieldCameraPreset *preset = &gFieldCameraPresets[presetNo];
    if (presetNo >= FIELD_CAMERA_PRESET_COUNT) {
        GF_AssertFail();
    }
    fieldSystem->camera = Camera_New(HEAP_ID_FIELD1);
    Camera_Init_FromTargetDistanceAndAngle((VecFx32 *)target, preset->distance, &preset->angle, preset->perspectiveAngle, (u8)preset->perspectiveType, TRUE, fieldSystem->camera);
    Camera_SetStaticPtr(fieldSystem->camera);
    Camera_SetPerspectiveClippingPlane(preset->clipNear, preset->clipFar, fieldSystem->camera);
    Camera_OffsetLookAtPosAndTarget(&preset->lookAtOffset, fieldSystem->camera);
    if (withHistory) {
        Camera_History_New(7, 6, 2, HEAP_ID_FIELD1, fieldSystem->camera);
    }
    fieldSystem->unk28 = FieldCameraShift_New(HEAP_ID_FIELD1);
    FieldCamera_InitShift(fieldSystem, presetNo);
}

void FieldCamera_Delete(FieldSystem *fieldSystem) {
    FieldCameraShift_Free(fieldSystem->unk28);
    Camera_UnsetStaticPtr();
    Camera_History_Delete(fieldSystem->camera);
    Camera_Delete(fieldSystem->camera);
}

FieldCameraShift *FieldCameraShift_New(enum HeapID heapID) {
    FieldCameraShift *shift = Heap_Alloc(heapID, sizeof(FieldCameraShift));
    MI_CpuFill8(shift, 0, sizeof(FieldCameraShift));
    return shift;
}

void FieldCameraShift_Free(FieldCameraShift *shift) {
    Heap_Free(shift);
}

void FieldCamera_InitShift(FieldSystem *fieldSystem, u32 presetNo) {
    FieldCameraShift *shift = fieldSystem->unk28;
    u8 mode;

    shift->camera = fieldSystem->camera;
    shift->presetNo = presetNo;
    mode = gFieldCameraShiftMode;
    if (mode != 0) {
        CameraAngle angle = { 0, 0, 0, 0 };
        const FieldCameraShiftPreset *preset = &gFieldCameraShiftPresets[mode - 1];
        angle.x = preset->motion.angle;
        shift->mode = mode;
        Camera_SetAnglePos(&angle, shift->camera);
        Camera_OffsetLookAtPosAndTarget(&preset->motion.lookAtOffset, shift->camera);
    }
}

// force_active only sets a section flag; it is kept so the objects stay identical to the matched ones
#pragma force_active on
void FieldCameraShift_Start(FieldCameraShift *shift, u8 mode) {
    int presetIdx;
    const FieldCameraPreset *cameraPreset;

    if (!mode) {
        return;
    }
    if (shift->active) {
        if (shift->mode == mode) {
            shift->reverse = ((int)shift->reverse + 1) % 2;
        } else {
            GF_AssertFail();
        }
        return;
    }
    cameraPreset = &gFieldCameraPresets[shift->presetNo];
    shift->mode = mode;
    shift->active = TRUE;
    if (gFieldCameraShiftMode == 0) {
        shift->startAngle = cameraPreset->angle.x;
        presetIdx = mode - 1;
        shift->motion = gFieldCameraShiftPresets[presetIdx].motion;
        gFieldCameraShiftMode = shift->mode;
    } else {
        presetIdx = mode - 1;
        shift->startAngle = gFieldCameraShiftPresets[presetIdx].motion.angle;
        shift->motion.angle = cameraPreset->angle.x;
        shift->motion.padding = 0;
        shift->motion.lookAtOffset = gFieldCameraShiftPresets[presetIdx].motion.lookAtOffset;
        shift->motion.lookAtOffset.x *= -1;
        shift->motion.lookAtOffset.y *= -1;
        shift->motion.lookAtOffset.z *= -1;
        gFieldCameraShiftMode = 0;
    }
    shift->duration = gFieldCameraShiftPresets[presetIdx].duration;
    shift->step = 0;
}

void FieldCameraShift_Update(FieldCameraShift *shift) {
    if (shift->active) {
        if (!shift->reverse) {
            shift->step++;
            FieldCameraShift_SetAngle(shift->camera, &shift->startAngle, &shift->motion.angle, shift->step, shift->duration);
            FieldCameraShift_MoveLookAt(shift->camera, &shift->motion.lookAtOffset, shift->step, shift->duration);
            if (shift->step >= shift->duration) {
                shift->active = FALSE;
            }
        } else {
            VecFx32 offset = shift->motion.lookAtOffset;
            offset.x *= -1;
            offset.y *= -1;
            offset.z *= -1;
            FieldCameraShift_SetAngle(shift->camera, &shift->motion.angle, &shift->startAngle, (u8)(shift->duration - shift->step), shift->duration);
            FieldCameraShift_MoveLookAt(shift->camera, &offset, shift->step, shift->duration);
            shift->step--;
            if (shift->step == 0) {
                if (gFieldCameraShiftMode) {
                    gFieldCameraShiftMode = 0;
                } else {
                    gFieldCameraShiftMode = shift->mode;
                }
                shift->reverse = FALSE;
                shift->active = FALSE;
            }
        }
    }
}
#pragma force_active reset

// Sets the camera's x angle to the point step/total of the way from start to end.
void FieldCameraShift_SetAngle(Camera *camera, const u16 *start, const u16 *end, u32 step, u8 total) {
    CameraAngle angle = { 0, 0, 0, 0 };
    s32 change;
    u16 initial = *start;
    if (*end >= initial) {
        u16 difference = *end - initial;
        change = ((s32)difference * (s32)step) / (s32)total;
    } else {
        u16 difference = initial - *end;
        change = -(((s32)difference * (s32)step) / (s32)total);
    }
    angle.x = initial + change;
    Camera_SetAnglePos(&angle, camera);
}

// Moves the look-at point by this step's part of offset (x and z only).
void FieldCameraShift_MoveLookAt(Camera *camera, const VecFx32 *offset, u32 step, u32 total) {
    VecFx32 delta = { 0, 0, 0 };
    delta.x = FieldCameraShift_GetStepDelta(offset->x, step, total);
    delta.z = FieldCameraShift_GetStepDelta(offset->z, step, total);
    Camera_OffsetLookAtPosAndTarget(&delta, camera);
}

s32 FieldCameraShift_GetStepDelta(s32 value, s32 step, s32 total) {
    s32 current = value * step / total;
    s32 previous = value * (step - 1) / total;
    return current - previous;
}
