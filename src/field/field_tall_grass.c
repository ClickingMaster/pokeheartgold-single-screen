#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"

#include "billboard_lists.h"
#include "map_object.h"
#include "field_effects.h"

// The tall grass a map object stands in (field effect renderer 8): a billboard over the tile that rustles
// (12 frames) when the object walks in, then holds its last frame until the object leaves the tile (or
// turns away, for grass shown ahead of it). The renderer also loads records 5-7 for a three-kind
// billboard effect (template sTallGrassUnusedEffectTemplate) that nothing in the game creates.

#define TALL_GRASS_RECORD 0

// Puts position->y on the ground of the tile at position->x/z; FALSE while that tile has no ground yet.
// (unk_0205FD20.h declares it returning void.)
BOOL FieldSystem_SnapToTerrainHeight(FieldSystem *fieldSystem, VecFx32 *position);

// Renderer data, from FieldEffectManager_GetRendererData(manager, 8).
typedef struct TallGrass {
    FieldEffectManager *manager;
} TallGrass;

typedef struct TallGrassEffectArgs {
    int x;
    int y;
    int z;
    FieldSystem *fieldSystem;
    FieldEffectManager *manager;
    TallGrass *grass;
    LocalMapObject *mapObject;
    s16 dx; // tile offset of the map object from the grass tile
    s16 dz;
    s8 direction; // -1: any facing
} TallGrassEffectArgs;

typedef struct TallGrassEffect {
    int state;
    u32 spriteId;
    u32 objectId;
    u32 mapId;
    u8 unk10[4];
    BOOL onGround;
    TallGrassEffectArgs args;
    void *billboard;
} TallGrassEffect;

typedef struct UnkOv01_0220914C_Args {
    int x;
    int z;
    FieldSystem *fieldSystem;
    FieldEffectManager *manager;
    u8 unk10[4];
} UnkOv01_0220914C_Args;

typedef struct UnkOv01_0220914C_Effect {
    int state;
    BOOL done;
    u8 unk08[4];
    int kind;
    UnkOv01_0220914C_Args args;
    void *billboard;
} UnkOv01_0220914C_Effect;

TallGrass *TallGrass_New(FieldEffectManager *manager);
void TallGrass_Free(TallGrass *grass);
void TallGrass_LoadGfx(TallGrass *grass);
void TallGrass_FreeGfx(TallGrass *grass);
void TallGrass_Spawn(LocalMapObject *mapObject, BOOL animate);
FieldEffect *TallGrass_SpawnAt(LocalMapObject *mapObject, BOOL animate, int x, int z, BOOL checkFacing);
BOOL TallGrassEffect_Init(FieldEffect *effect, TallGrassEffect *work);
BOOL TallGrassEffect_Destroy(FieldEffect *effect, TallGrassEffect *work);
void TallGrassEffect_Update(FieldEffect *effect, TallGrassEffect *work);
void TallGrassEffect_Draw(FieldEffect *effect, TallGrassEffect *work);
BOOL TallGrassUnusedEffect_Init(FieldEffect *effect, UnkOv01_0220914C_Effect *work);
BOOL TallGrassUnusedEffect_Destroy(FieldEffect *effect, UnkOv01_0220914C_Effect *work);
void TallGrassUnusedEffect_Update(FieldEffect *effect, UnkOv01_0220914C_Effect *work);
void TallGrassUnusedEffect_Draw(FieldEffect *effect, UnkOv01_0220914C_Effect *work);

// In asm/overlay_01_021FEEEC_data_local.s (its order of these objects is not one MWCC reproduces).
extern const FieldEffectBasicTemplate ov01_02209138; // tall grass effect
extern const FieldEffectBasicTemplate sTallGrassUnusedEffectTemplate; // created nowhere
extern const u32 ov01_02209160[6];                   // record 6 params
extern const u32 ov01_02209178[6];                   // tall grass record params
extern const u32 ov01_02209190[6];                   // record 5 params
extern const u32 ov01_022091A8[6];                   // record 7 params

TallGrass *TallGrass_New(FieldEffectManager *manager) {
    TallGrass *grass = FieldEffectManager_HeapAllocAndFill(manager, sizeof(TallGrass), FALSE, 0);
    grass->manager = manager;
    TallGrass_LoadGfx(grass);
    return grass;
}

void TallGrass_Free(TallGrass *grass) {
    TallGrass_FreeGfx(grass);
    FieldEffectManager_HeapFree(grass);
}

void TallGrass_LoadGfx(TallGrass *grass) {
    FieldEffectManager_LoadBillboardResource(grass->manager, 0, 0x7E);
    FieldEffectManager_LoadBillboardResource(grass->manager, 5, 0x7F);
    FieldEffectManager_LoadBillboardSequence(grass->manager, 0, 0x8C);
    FieldEffectManager_LoadBillboardSequence(grass->manager, 3, 0x8D);
    FieldEffectManager_LoadBillboardSequence(grass->manager, 4, 0x8E);
    FieldEffectManager_LoadBillboardSequence(grass->manager, 5, 0x8F);
    FieldEffectManager_LoadTexResource(grass->manager, 0, 0x19, TRUE);
    FieldEffectManager_LoadTexResource(grass->manager, 4, 0x10, TRUE);
    FieldEffectManager_LoadTexResource(grass->manager, 5, 0, TRUE);
    FieldEffectManager_LoadTexResource(grass->manager, 6, 1, TRUE);
    FieldEffectManager_AddRecord(grass->manager, TALL_GRASS_RECORD, 0, 0, 0, 0, ov01_02209178);
    FieldEffectManager_AddRecord(grass->manager, 5, 5, 3, 4, 0, ov01_02209190);
    FieldEffectManager_AddRecord(grass->manager, 6, 5, 4, 5, 0, ov01_02209160);
    FieldEffectManager_AddRecord(grass->manager, 7, 5, 5, 6, 0, ov01_022091A8);
}

void TallGrass_FreeGfx(TallGrass *grass) {
    FieldEffectManager_FreeBillboardResource(grass->manager, 0);
    FieldEffectManager_FreeBillboardResource(grass->manager, 5);
    FieldEffectManager_FreeBillboardSequence(grass->manager, 0);
    FieldEffectManager_FreeBillboardSequence(grass->manager, 3);
    FieldEffectManager_FreeBillboardSequence(grass->manager, 4);
    FieldEffectManager_FreeBillboardSequence(grass->manager, 5);
    FieldEffectManager_FreeTexResource(grass->manager, 0);
    FieldEffectManager_FreeTexResource(grass->manager, 4);
    FieldEffectManager_FreeTexResource(grass->manager, 5);
    FieldEffectManager_FreeTexResource(grass->manager, 6);
    FieldEffectManager_ReleaseRecord(grass->manager, TALL_GRASS_RECORD);
    FieldEffectManager_ReleaseRecord(grass->manager, 5);
    FieldEffectManager_ReleaseRecord(grass->manager, 6);
    FieldEffectManager_ReleaseRecord(grass->manager, 7);
}

void TallGrass_Spawn(LocalMapObject *mapObject, BOOL animate) {
    VecFx32 position;
    TallGrassEffectArgs args;
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);

    args.x = MapObject_GetXCoord(mapObject);
    args.y = MapObject_GetYCoord(mapObject);
    args.z = MapObject_GetZCoord(mapObject);
    args.dx = 0;
    args.dz = 0;
    args.direction = -1;
    args.manager = manager;
    args.fieldSystem = FieldEffectManager_GetFieldSystem(manager);
    args.grass = FieldEffectManager_GetRendererData(manager, 8);
    args.mapObject = mapObject;
    MapObject_CopyPositionVector(mapObject, &position);
    FieldEffectManager_CreateEffect(manager, &ov01_02209138, &position, animate, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
}

#pragma force_active on
FieldEffect *TallGrass_SpawnAt(LocalMapObject *mapObject, BOOL animate, int x, int z, BOOL checkFacing) {
    VecFx32 position;
    TallGrassEffectArgs args;
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);

    args.x = x;
    args.y = MapObject_GetYCoord(mapObject);
    args.z = z;
    args.dx = MapObject_GetXCoord(mapObject) - x;
    args.dz = MapObject_GetZCoord(mapObject) - z;
    if (checkFacing) {
        args.direction = MapObject_GetFacingDirection(mapObject);
    } else {
        args.direction = -1;
    }
    args.manager = manager;
    args.fieldSystem = FieldEffectManager_GetFieldSystem(manager);
    args.grass = FieldEffectManager_GetRendererData(manager, 8);
    args.mapObject = mapObject;
    MapObject_CopyPositionVector(mapObject, &position);
    return FieldEffectManager_CreateEffect(manager, &ov01_02209138, &position, animate, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
}
#pragma force_active reset

BOOL TallGrassEffect_Init(FieldEffect *effect, TallGrassEffect *work) {
    VecFx32 position;

    work->args = *(TallGrassEffectArgs *)FieldEffect_GetUserData(effect);
    work->spriteId = MapObject_GetSpriteID(work->args.mapObject);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    position.x = work->args.x << 16;
    position.z = work->args.z << 16;
    position.y = MapObject_GetPositionVectorYCoord(work->args.mapObject);
    work->onGround = FieldSystem_SnapToTerrainHeight(work->args.fieldSystem, &position);
    position.x = position.x + (2 << 14);
    position.z = position.z + (0x12 << 12);
    FieldEffect_SetPosition(effect, &position);
    work->billboard = FieldEffectManager_CreateBillboardFromRecord(work->args.manager, TALL_GRASS_RECORD, &position);
    if (MapObject_TestFlagsBits(work->args.mapObject, MAPOBJECTFLAG_VISIBLE) == TRUE) {
        BillboardObject_SetVisible(work->billboard, FALSE);
    }
    if (FieldEffect_GetParam(effect) == FALSE) {
        BillboardObject_SetFrame(work->billboard, 12 * FX32_ONE);
        work->state = 2;
    }
    return TRUE;
}

BOOL TallGrassEffect_Destroy(FieldEffect *effect, TallGrassEffect *work) {
    return BillboardObject_Delete(work->billboard);
}

void TallGrassEffect_Update(FieldEffect *effect, TallGrassEffect *work) {
    VecFx32 current;
    VecFx32 position;
    LocalMapObject *mapObject = work->args.mapObject;
    int x;
    int z;

    if (!MapObject_CheckSpriteIDAndMap(mapObject, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    if (MapObject_TestFlagsBits(mapObject, MAPOBJECTFLAG_VISIBLE) == TRUE) {
        BillboardObject_SetVisible(work->billboard, FALSE);
    } else {
        BillboardObject_SetVisible(work->billboard, TRUE);
    }
    if (work->onGround == FALSE) {
        FieldEffect_GetPosition(effect, &current);
        position.x = work->args.x << 16;
        position.z = work->args.z << 16;
        position.y = current.y;
        work->onGround = FieldSystem_SnapToTerrainHeight(work->args.fieldSystem, &position);
        if (work->onGround == TRUE) {
            current.y = position.y;
            FieldEffect_SetPosition(effect, &current);
        }
    }
    switch (work->state) {
    case 0:
        if (BillboardObject_AdvanceFrame(work->billboard, FX32_ONE) == TRUE) {
        }
        if (BillboardObject_GetSequenceFrame(work->billboard) / FX32_ONE >= 12) {
            work->state = 1;
        }
        return;
    case 1:
        BillboardObject_SetFrame(work->billboard, 12 * FX32_ONE);
        work->state = 2;
        // fallthrough
    case 2:
        if (!MapObject_CheckSpriteIDAndMap(mapObject, work->spriteId, work->objectId, work->mapId)) {
            FieldEffectManager_DestroyEffect((int)effect);
            return;
        }
        x = MapObject_GetXCoord(mapObject) - work->args.dx;
        z = MapObject_GetZCoord(mapObject) - work->args.dz;
        if (work->args.x != x || work->args.z != z) {
            FieldEffectManager_DestroyEffect((int)effect);
            return;
        }
        if (work->args.direction == -1) {
            return;
        }
        if (work->args.direction == MapObject_GetFacingDirection(mapObject)) {
            return;
        }
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
}

void TallGrassEffect_Draw(FieldEffect *effect, TallGrassEffect *work) {
    VecFx32 position;

    if (!MapObject_CheckSpriteIDAndMap(work->args.mapObject, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    FieldEffect_GetPosition(effect, &position);
    BillboardObject_SetPosition(work->billboard, &position);
}

#pragma force_active on
BOOL TallGrassUnusedEffect_Init(FieldEffect *effect, UnkOv01_0220914C_Effect *work) {
    VecFx32 position;
    int record;

    work->args = *(UnkOv01_0220914C_Args *)FieldEffect_GetUserData(effect);
    work->kind = FieldEffect_GetParam(effect);
    position.x = (work->args.x << 16) + 0x8000;
    position.z = (work->args.z << 16) + 0x9000;
    position.y = 0;
    FieldSystem_SnapToTerrainHeight(work->args.fieldSystem, &position);
    position.y += FX32_ONE;
    FieldEffect_SetPosition(effect, &position);
    switch (work->kind) {
    case 0:
        record = 5;
        break;
    case 1:
        record = 6;
        break;
    case 2:
    default:
        record = 7;
        break;
    }
    work->billboard = FieldEffectManager_CreateBillboardFromRecord(work->args.manager, record, &position);
    return TRUE;
}
#pragma force_active reset

BOOL TallGrassUnusedEffect_Destroy(FieldEffect *effect, UnkOv01_0220914C_Effect *work) {
    return BillboardObject_Delete(work->billboard);
}

void TallGrassUnusedEffect_Update(FieldEffect *effect, UnkOv01_0220914C_Effect *work) {
    switch (work->state) {
    case 0:
        if (BillboardObject_AdvanceFrame(work->billboard, FX32_ONE) == TRUE) {
            work->done = TRUE;
            work->state++;
        }
        break;
    case 1:
        break;
    }
}

void TallGrassUnusedEffect_Draw(FieldEffect *effect, UnkOv01_0220914C_Effect *work) {
    VecFx32 position;

    FieldEffect_GetPosition(effect, &position);
    BillboardObject_SetPosition(work->billboard, &position);
}
