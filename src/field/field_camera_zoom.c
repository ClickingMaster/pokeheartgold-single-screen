#include "field/field_camera_zoom.h"

#include "global.h"

#include "heap.h"
#include "sys_task_api.h"

static void (*const sFieldCameraZoomFuncs[])(FieldCameraZoom *zoom) = {
    [FIELD_CAMERA_ZOOM_IDLE] = FieldCameraZoom_Idle,
    [FIELD_CAMERA_ZOOM_BY] = FieldCameraZoom_ZoomBy,
    [FIELD_CAMERA_ZOOM_BACK] = FieldCameraZoom_ZoomBack,
    NULL,
};

SysTask *FieldCameraZoom_Start(FieldSystem *fieldSystem, enum HeapID heapID) {
    FieldCameraZoom *zoom = Heap_AllocAtEnd(heapID, sizeof(FieldCameraZoom));

    memset(zoom, 0, sizeof(FieldCameraZoom));
    zoom->heapID = heapID;
    zoom->mode = FIELD_CAMERA_ZOOM_IDLE;
    zoom->fieldSystem = fieldSystem;
    zoom->camera = fieldSystem->camera;
    zoom->initialDistance = Camera_GetDistance(zoom->camera);
    zoom->distance = zoom->initialDistance;
    return SysTask_CreateOnMainQueue((SysTaskFunc)Task_FieldCameraZoom, zoom, 0xFFFF);
}

BOOL FieldCameraZoom_IsDone(SysTask *task) {
    return ((FieldCameraZoom *)SysTask_GetData(task))->done;
}

void FieldCameraZoom_End(SysTask *task) {
    Heap_Free(SysTask_GetData(task));
    SysTask_Destroy(task);
}

void FieldCameraZoom_Set(SysTask *task, int mode, fx32 amount, u32 duration) {
    FieldCameraZoom *zoom = SysTask_GetData(task);

    zoom->state = 0;
    zoom->done = FALSE;
    zoom->mode = mode;
    zoom->amount = amount;
    zoom->duration = duration;
    zoom->elapsed = 0;
}

void Task_FieldCameraZoom(SysTask *task, FieldCameraZoom *zoom) {
    sFieldCameraZoomFuncs[zoom->mode](zoom);
}

void FieldCameraZoom_Idle(FieldCameraZoom *zoom) {
    zoom->done = TRUE;
}

void FieldCameraZoom_ZoomBy(FieldCameraZoom *zoom) {
    switch (zoom->state) {
    case 0:
        FieldCameraZoom_InitZoomBy(zoom);
        zoom->state++;
    case 1:
        if (FieldCameraZoom_Step(zoom) == TRUE) {
            zoom->state++;
            zoom->done = TRUE;
        }
        break;
    default:
        return;
    }
    FieldCameraZoom_ApplyDistance(zoom);
}

void FieldCameraZoom_ZoomBack(FieldCameraZoom *zoom) {
    switch (zoom->state) {
    case 0:
        FieldCameraZoom_InitZoomBack(zoom);
        zoom->state++;
    case 1:
        if (FieldCameraZoom_Step(zoom) == TRUE) {
            zoom->state++;
            zoom->done = TRUE;
        }
        break;
    default:
        return;
    }
    FieldCameraZoom_ApplyDistance(zoom);
}

void FieldCameraZoom_ApplyDistance(FieldCameraZoom *zoom) {
    Camera_SetDistance(zoom->distance, zoom->camera);
}

void FieldCameraZoom_InitZoomBy(FieldCameraZoom *zoom) {
    zoom->step = zoom->amount / (s32)zoom->duration;
    zoom->targetDistance = zoom->distance + zoom->amount;
}

void FieldCameraZoom_InitZoomBack(FieldCameraZoom *zoom) {
    zoom->step = (zoom->initialDistance - zoom->distance) / (s32)zoom->duration;
    zoom->targetDistance = zoom->initialDistance;
}

BOOL FieldCameraZoom_Step(FieldCameraZoom *zoom) {
    zoom->distance += zoom->step;
    zoom->elapsed++;
    if (zoom->elapsed >= zoom->duration) {
        zoom->elapsed = zoom->duration;
        zoom->distance = zoom->targetDistance;
        return TRUE;
    }
    return FALSE;
}
