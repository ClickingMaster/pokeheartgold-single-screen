#include "global.h"

#include "camera.h"

// The leading fields of the Aprijuice scene
typedef struct AprijuiceSceneHeader {
    enum HeapID heapId;
    void *vramMan;
    Camera *camera;
} AprijuiceSceneHeader;

void AprijuiceScene_InitCamera(AprijuiceSceneHeader *scene, enum HeapID heapId);

static const VecFx32 sAprijuiceSceneCameraPosition = {
    .x = 0,
    .y = 0,
    .z = FX32_CONST(16),
};

void AprijuiceScene_InitCamera(AprijuiceSceneHeader *scene, enum HeapID heapId) {
    VecFx32 pos = sAprijuiceSceneCameraPosition;
    CameraAngle angle = { 0, 0, 0, 0 };
    scene->camera = Camera_New(heapId);
    Camera_Init_FromPosDistanceAndAngle(&pos, FX32_CONST(16), &angle, 1473, 1, scene->camera);
    Camera_SetPerspectiveClippingPlane(0, FX32_CONST(100), scene->camera);
    Camera_ClearFixedTarget(scene->camera);
    Camera_SetStaticPtr(scene->camera);
}
