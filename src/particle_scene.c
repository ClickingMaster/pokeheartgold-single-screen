#include "global.h"

#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "particle_system.h"
#include "unk_02026E30.h"
typedef struct ParticleScene {
    enum HeapID heapID;
    int resource;
    u32 unused;
    SPLEmitter *context;
} ParticleScene;
void sub_020773AC(void);
void sub_020773D4(void);
GF3DVramMan *sub_02077400(enum HeapID heapID);
void sub_0207741C(void);
void sub_020774A0(void);
void sub_020774E0(void);
u32 sub_02077504(u32 size, BOOL compressed);
u32 sub_02077520(u32 size, BOOL palette4);
SPLEmitter *sub_0207753C(enum HeapID heapID);
SPLEmitter *sub_02077584(enum HeapID heapID, NarcId narc, int file);
void sub_020775AC(SPLEmitter *context);
void sub_020775C4(SPLEmitter *emitter);
ParticleScene *sub_02077604(ParticleScene *settings);
void sub_02077634(ParticleScene *scene, int resource);
BOOL sub_02077650(ParticleScene *scene);
void sub_02077664(ParticleScene *scene);
void sub_020773AC(void) {
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}
void sub_020773D4(void) {
    G2_SetBlendAlpha(0, 14, 11, 7);
    G2S_SetBlendAlpha(0, 14, 7, 8);
}
GF3DVramMan *sub_02077400(enum HeapID heapID) {
    return GF_3DVramMan_Create(heapID, 0, 2, 0, 2, sub_0207741C);
}
void sub_0207741C(void) {
    GfGfx_EngineATogglePlanes(1, 1);
    G2_SetBG0Priority(1);
    G3X_SetShading((GXShading)0);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, (GXFogBlend)0, (GXFogSlope)0, 0);
    G3X_SetClearColor(0, 0, 0x7FFF, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
}
void sub_020774A0(void) {
    NNSGfdTexKey texture = NNS_GfdAllocTexVram(0x8000, FALSE, 0);
    NNSGfdPlttKey palette = NNS_GfdAllocPlttVram(0xA0, FALSE, 0);
    GF_ASSERT(texture != 0);
    GF_ASSERT(palette != 0);
    ParticleSystem_ZeroAll();
}
void sub_020774E0(void) {
    Thunk_G3X_Reset();
    if (ParticleSystem_DrawAll() > 0) {
        Thunk_G3X_Reset();
        NNS_G2dSetupSoftwareSpriteCamera();
    }
    ParticleSystem_CalcAll();
    RequestSwap3DBuffers((GXSortMode)1, (GXBufferMode)0);
}
u32 sub_02077504(u32 size, BOOL compressed) {
    NNSGfdTexKey key = NNS_GfdAllocTexVram(size, compressed, 0);
    ParticleSystem_RegisterTexKey(key);
    return (key << 16) >> 13;
}
u32 sub_02077520(u32 size, BOOL palette4) {
    NNSGfdPlttKey key = NNS_GfdAllocPlttVram(size, palette4, 0);
    ParticleSystem_RegisterPlttKey(key);
    return (key << 16) >> 13;
}
SPLEmitter *sub_0207753C(enum HeapID heapID) {
    void *heap = Heap_Alloc(heapID, 0x4800);
    SPLEmitter *context = ParticleSystem_New(sub_02077504, sub_02077520, heap, 0x4800, TRUE, heapID);
    Camera *camera = ParticleSystem_GetCamera(context);
    if (camera) {
        Camera_SetPerspectiveClippingPlane(FX32_ONE, 0x384000, camera);
    }
    return context;
}
SPLEmitter *sub_02077584(enum HeapID heapID, NarcId narc, int file) {
    SPLEmitter *context = sub_0207753C(heapID);
    void *resource = ParticleSystem_LoadResourceFile(narc, file, heapID);
    ParticleSystem_SetResource(context, resource, 10, TRUE);
    return context;
}
void sub_020775AC(SPLEmitter *context) {
    void *heap = ParticleSystem_GetHeap(context);
    ParticleSystem_Free(context);
    Heap_Free(heap);
}
void sub_020775C4(SPLEmitter *emitter) {
    VecFx32 position = { 0, 0, 0 };
    position.x = 0;
    position.y = 0x560;
    position.z = 0;
    SPL_SetEmitterPosition(emitter, &position);
}
ParticleScene *sub_02077604(ParticleScene *settings) {
    ParticleScene *scene = Heap_Alloc(settings->heapID, sizeof(*scene));
    GF_ASSERT(scene != NULL); /* Preserve the original settings-read order before initializing the scene. */
    {
        enum HeapID heapID = ((volatile ParticleScene *)settings)->heapID;
        int resource = ((volatile ParticleScene *)settings)->resource;
        scene->heapID = heapID;
        scene->resource = resource;
    }
    scene->context = sub_02077584(scene->heapID, (NarcId)119, scene->resource);
    ParticleSystem_SetPerspectiveType(scene->context, 1);
    return scene;
}
void sub_02077634(ParticleScene *scene, int resource) {
    ParticleSystem_CreateEmitterWithParam(scene->context, resource, sub_020775C4, scene);
    ParticleSystem_SetPerspectiveType(scene->context, 1);
}
BOOL sub_02077650(ParticleScene *scene) {
    if (ParticleSystem_GetActiveEmitterCount(scene->context)) {
        return TRUE;
    }
    return FALSE;
}
void sub_02077664(ParticleScene *scene) {
    sub_020775AC(scene->context);
    Heap_Free(scene);
}
