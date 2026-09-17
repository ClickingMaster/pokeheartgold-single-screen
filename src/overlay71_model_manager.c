#include "global.h"

#include "camera.h"
#include "gf_3d_render.h"
#include "heap.h"
#include "overlay71_model_manager_private.h"

Ov71ModelManager *ov71_0224744C(u32 count, void *a1, void *a2, void *a3) {
    Ov71ModelManager *manager = Heap_Alloc((enum HeapID)0x39, sizeof(Ov71ModelManager));
    u32 i;

    if (manager != NULL) {
        ov71_0224784C(manager, a1, a2, a3);
        manager->models = Heap_Alloc((enum HeapID)0x39, sizeof(Ov71Model) * count);
        manager->count = count;
        for (i = 0; i < count; i++) {
            ov71_022475C4(&manager->models[i]);
        }
    }
    return manager;
}

void ov71_02247498(Ov71ModelManager *manager) {
    u32 i;

    for (i = 0; i < manager->count; i++) {
        ov71_022475F8(&manager->models[i]);
    }
    Heap_Free(manager->models);
    ov71_022478B8(manager);
    Heap_Free(manager);
}

void ov71_022474CC(Ov71ModelManager *manager) {
    u32 i;
    u32 one;

    NNS_G3dGeBufferOP_N(0x11, NULL, 0);
    Camera_PushLookAtToNNSGlb();
    for (i = 0; i < manager->count; i++) {
        if (manager->models[i].active != 0) {
            ov71_02247514(&manager->models[i]);
        }
    }
    one = 1;
    NNS_G3dGeBufferOP_N(0x12, &one, 1);
}

void ov71_02247514(Ov71Model *model) {
    MtxFx33 rotation;
    MtxFx33 temp;

    MTX_Identity33_(&rotation);
    MTX_RotX33_(&temp, FX_SinIdx((u16)model->rotationX), FX_CosIdx((u16)model->rotationX));
    MTX_Concat33(&temp, &rotation, &rotation);
    MTX_RotY33_(&temp, FX_SinIdx((u16)model->rotationY), FX_CosIdx((u16)model->rotationY));
    MTX_Concat33(&temp, &rotation, &rotation);
    MTX_RotZ33_(&temp, FX_SinIdx((u16)model->rotationZ), FX_CosIdx((u16)model->rotationZ));
    MTX_Concat33(&temp, &rotation, &rotation);
    if (model->polygonId != 0x1f) {
        NNS_G3dGlbPolygonAttr(0, (GXPolygonMode)0, (GXCull)0, 0, model->polygonId, 0);
    }
    GF3dRender_DrawModel((NNSG3dRenderObj *)model->renderObj, &model->translation, &rotation, &model->scale);
}

void ov71_022475C4(Ov71Model *model) {
    model->active = 0;
    model->unk00 = 0;
    model->rotationX = model->rotationY = model->rotationZ = 0;
    model->scale.x = model->scale.y = model->scale.z = FX32_ONE;
    model->translation.x = model->translation.y = model->translation.z = 0;
}
