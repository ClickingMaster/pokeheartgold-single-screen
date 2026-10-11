#ifndef POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_H
#define POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_H

#include "map_object.h"
#include "script.h"
#include "task.h"
#include "field_effects.h"

typedef struct FieldEffectManager {
    u8 unk0[0x24];
} FieldEffectManager;

FieldEffectManager *FieldEffectManager_New(FieldSystem *fieldSystem, u32 rendererCount, enum HeapID heapID);
void FieldEffectManager_InitAnimManagerList(FieldEffectManager *fieldEffectManager, u32 animManagerCount);
void FieldEffectManager_InitGfx(FieldEffectManager *fieldEffectManager, enum HeapID heapID, int, int, int, int, int, int, int);
void FieldEffectManager_InitRenderers(FieldEffectManager *fieldEffectManager, const u32 *arg1);
void FieldEffectManager_Render(FieldEffectManager *fieldEffectManager);
void FieldEffectManager_Free(FieldEffectManager *fieldEffectManager);

typedef struct {
    u8 unk0[0xc];
    NNSG3dResMdl *unkC;
    u8 unk10[4];
} UnkOv01_021FFECC_sub;

typedef struct {
    void *unk0;
    UnkOv01_021FFECC_sub unk4[4];
    NNSG3dRenderObj unk54[4];
} UnkOv01_021FFECC;

typedef struct {
    int unk0;
    BOOL unk4;
    u32 localMapObjectId;
    u32 mapId;
    UnkOv01_021FFFCC_common unk10;
    NNSG3dRenderObj *unk24;
} UnkOv01_021FFFCD;

typedef BOOL (*UnkOv01_02209280_Cb1)(void *, UnkOv01_021FFFCD *);
typedef void (*UnkOv01_02209280_Cb2)(void *, UnkOv01_021FFFCD *);

typedef struct {
    int unk0;
    UnkOv01_02209280_Cb1 unk4;
    UnkOv01_02209280_Cb1 unk8;
    UnkOv01_02209280_Cb2 unkC;
    UnkOv01_02209280_Cb2 unk10;
} UnkOv01_02209280;

typedef struct {
    int unk0;
    FieldSystem *unk4;
    UnkOv01_021FFECC *unk8;
    LocalMapObject *unkC;
    NNSG3dRenderObj *unk10;
} UnkOv01_021FFF5C;

UnkOv01_021FFECC *FieldEffectManager_HeapAllocAndFill(void *a0, int a1, int a2, int a3);
FieldSystem *FieldEffectManager_GetFromMapObject(LocalMapObject *mapObject);
TaskManager *FieldEffectManager_GetFieldSystem(FieldSystem *fieldSystem);
UnkOv01_021FFECC *FieldEffectManager_GetRendererData(FieldSystem *fieldSystem, int a1);
void FieldEffectManager_CreateEffect(FieldSystem *fieldSystem, const UnkOv01_02209280 *a1, VecFx32 *a2, int a3, UnkOv01_021FFF5C *a4, int a5);
void FieldEffectManager_HeapFree(void *a0); // UnkOv01_021FFECC or NNSG3dResFileHeader?
void ov01_021F19F4(void *a0, UnkOv01_021FFECC_sub *a1, int a2, int a3, int a4);
void FieldEffectManager_DestroyEffect(int a0);

#endif // POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_H
