#include "wifi_plaza_camera.h"

#include "global.h"

#include "camera.h"
#include "heap.h"

extern const CameraAngle sWiFiPlazaCameraAngle;

// Object manager (wifi_plaza_obj): writes the object's position
void ov49_02259154(void *object, VecFx32 *position);

WiFiPlazaCamera *WiFiPlazaCamera_Create(enum HeapID heapID) {
    WiFiPlazaCamera *camera = Heap_Alloc(heapID, sizeof(WiFiPlazaCamera));
    u32 i;
    u8 *p = (u8 *)camera;

    // byte-wise clear written out (the compiled loop)
    i = sizeof(WiFiPlazaCamera);
    do {
        *p = 0;
        p++;
        i--;
    } while (i != 0);
    camera->camera = Camera_New(heapID);
    Camera_Init_FromTargetDistanceAndAngle(&camera->target, 0x29AEC1, &sWiFiPlazaCameraAngle, 0x5C1, 0, TRUE, camera->camera);
    Camera_SetStaticPtr(camera->camera);
    Camera_SetPerspectiveClippingPlane(0x96000, 0x384000, camera->camera);
    return camera;
}

void WiFiPlazaCamera_Delete(WiFiPlazaCamera *camera) {
    Camera_UnsetStaticPtr();
    Camera_Delete(camera->camera);
    Heap_Free(camera);
}

void WiFiPlazaCamera_Update(WiFiPlazaCamera *camera) {
    if (camera->followObject != NULL) {
        ov49_02259154(camera->followObject, &camera->target);
        camera->target.x += 0x8000;
        camera->target.z -= 0x20000;
    }
    Camera_PushLookAtToNNSGlb();
}

void WiFiPlazaCamera_SetTarget(WiFiPlazaCamera *camera, fx32 x, fx32 y, fx32 z) {
    camera->target.x = x;
    camera->target.y = y;
    camera->target.z = z;
}

void WiFiPlazaCamera_SetTargetWithOffset(WiFiPlazaCamera *camera, fx32 x, fx32 y, fx32 z) {
    camera->target.x = x + 0x8000;
    camera->target.y = y;
    camera->target.z = z - 0x20000;
}

void WiFiPlazaCamera_SetFollowObject(WiFiPlazaCamera *camera, void *object) {
    camera->followObject = object;
}

void WiFiPlazaCamera_ClearFollowObject(WiFiPlazaCamera *camera) {
    camera->followObject = NULL;
}
