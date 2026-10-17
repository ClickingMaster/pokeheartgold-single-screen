#include "global.h"

#include "camera.h"

// The leading fields of the Aprijuice scene
typedef struct AprijuiceSceneHeader {
    enum HeapID heapId; // 0x00
    void *vramMan;      // 0x04
    Camera *camera;     // 0x08
} AprijuiceSceneHeader;

extern VecFx32 gAprijuiceSceneCameraPosition; // camera position

void AprijuiceScene_InitCamera(AprijuiceSceneHeader *scene, enum HeapID heapId);

void AprijuiceScene_InitCamera(AprijuiceSceneHeader *scene, enum HeapID heapId) {
    VecFx32 pos = gAprijuiceSceneCameraPosition;
    CameraAngle angle = { 0, 0, 0, 0 };
    scene->camera = Camera_New(heapId);
    Camera_Init_FromPosDistanceAndAngle(&pos, FX32_CONST(16), &angle, 1473, 1, scene->camera);
    Camera_SetPerspectiveClippingPlane(0, FX32_CONST(100), scene->camera);
    Camera_ClearFixedTarget(scene->camera);
    Camera_SetStaticPtr(scene->camera);
}
