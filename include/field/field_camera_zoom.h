#ifndef POKEHEARTGOLD_FIELD_CAMERA_ZOOM_H
#define POKEHEARTGOLD_FIELD_CAMERA_ZOOM_H

#include "camera.h"
#include "field_system.h"
#include "sys_task.h"

// A task that moves the field camera's distance linearly (dig, escape rope, starter animations).
enum FieldCameraZoomMode {
    FIELD_CAMERA_ZOOM_IDLE,
    FIELD_CAMERA_ZOOM_BY,   // change the distance by an amount
    FIELD_CAMERA_ZOOM_BACK, // go back to the distance the task started with
};

typedef struct FieldCameraZoom {
    enum HeapID heapID;
    int state;
    BOOL done;
    int mode;
    fx32 initialDistance;
    fx32 amount;
    fx32 targetDistance;
    u32 duration;
    fx32 distance;
    u32 elapsed;
    fx32 step;
    FieldSystem *fieldSystem;
    Camera *camera;
} FieldCameraZoom;

SysTask *FieldCameraZoom_Start(FieldSystem *fieldSystem, enum HeapID heapID);
BOOL FieldCameraZoom_IsDone(SysTask *task);
void FieldCameraZoom_End(SysTask *task);
void FieldCameraZoom_Set(SysTask *task, int mode, fx32 amount, u32 duration);
void Task_FieldCameraZoom(SysTask *task, FieldCameraZoom *zoom);
void FieldCameraZoom_Idle(FieldCameraZoom *zoom);
void FieldCameraZoom_ZoomBy(FieldCameraZoom *zoom);
void FieldCameraZoom_ZoomBack(FieldCameraZoom *zoom);
void FieldCameraZoom_ApplyDistance(FieldCameraZoom *zoom);
void FieldCameraZoom_InitZoomBy(FieldCameraZoom *zoom);
void FieldCameraZoom_InitZoomBack(FieldCameraZoom *zoom);
BOOL FieldCameraZoom_Step(FieldCameraZoom *zoom);

#endif // POKEHEARTGOLD_FIELD_CAMERA_ZOOM_H
