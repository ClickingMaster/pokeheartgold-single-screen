#ifndef POKEHEARTGOLD_WIFI_PLAZA_CAMERA_H
#define POKEHEARTGOLD_WIFI_PLAZA_CAMERA_H

#include "global.h"

#include "camera.h"
#include "heap.h"

// Camera of the Wi-Fi Plaza room; follows an avatar object when one is set.
typedef struct WiFiPlazaCamera {
    Camera *camera;
    void *followObject; // WiFiPlazaObj; its position (offset like WiFiPlazaCamera_SetTargetWithOffset) becomes the target
    VecFx32 target;
} WiFiPlazaCamera;

WiFiPlazaCamera *WiFiPlazaCamera_Create(enum HeapID heapID);
void WiFiPlazaCamera_Delete(WiFiPlazaCamera *camera);
void WiFiPlazaCamera_Update(WiFiPlazaCamera *camera);
void WiFiPlazaCamera_SetTarget(WiFiPlazaCamera *camera, fx32 x, fx32 y, fx32 z);
void WiFiPlazaCamera_SetTargetWithOffset(WiFiPlazaCamera *camera, fx32 x, fx32 y, fx32 z);
void WiFiPlazaCamera_SetFollowObject(WiFiPlazaCamera *camera, void *object);
void WiFiPlazaCamera_ClearFollowObject(WiFiPlazaCamera *camera);

#endif // POKEHEARTGOLD_WIFI_PLAZA_CAMERA_H
