#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"

#include "billboard_lists.h"
#include "map_object.h"
#include "field_effects.h"

// The very tall grass a map object stands in (field effect renderer 12): like the tall grass, a billboard
// over the tile that rustles (12 frames) when the object walks in, then holds its last frame until the
// object leaves the tile (or turns away, for grass shown ahead of it).

#define VERY_TALL_GRASS_RECORD   9
#define VERY_TALL_GRASS_RESOURCE 7
#define VERY_TALL_GRASS_SEQUENCE 7
#define VERY_TALL_GRASS_TEXTURE  8

// Puts position->y on the ground of the tile at position->x/z; FALSE while that tile has no ground yet.
// (unk_0205FD20.h declares it returning void.)
BOOL FieldSystem_SnapToTerrainHeight(FieldSystem *fieldSystem, VecFx32 *position);

// Renderer data, from FieldEffectManager_GetRendererData(manager, 12).
typedef struct VeryTallGrass {
    FieldEffectManager *manager;
} VeryTallGrass;

typedef struct VeryTallGrassEffectArgs {
    int x;
    int y;
    int z;
    FieldSystem *fieldSystem;
    FieldEffectManager *manager;
    VeryTallGrass *grass;
    LocalMapObject *mapObject;
    s16 dx; // tile offset of the map object from the grass tile
    s16 dz;
    s8 direction; // -1: any facing
} VeryTallGrassEffectArgs;

typedef struct VeryTallGrassEffect {
    int state;
    u32 spriteId;
    u32 objectId;
    u32 mapId;
    u8 unk10[4];
    BOOL onGround;
    VeryTallGrassEffectArgs args;
    void *billboard;
} VeryTallGrassEffect;

VeryTallGrass *VeryTallGrass_New(FieldEffectManager *manager);
void VeryTallGrass_Free(VeryTallGrass *grass);
void VeryTallGrass_LoadGfx(VeryTallGrass *grass);
void VeryTallGrass_FreeGfx(VeryTallGrass *grass);
void VeryTallGrass_Spawn(LocalMapObject *mapObject, BOOL animate);
void VeryTallGrass_SpawnAt(LocalMapObject *mapObject, BOOL animate, int x, int z, BOOL checkFacing);
BOOL VeryTallGrassEffect_Init(FieldEffect *effect, VeryTallGrassEffect *work);
void VeryTallGrassEffect_Destroy(FieldEffect *effect, VeryTallGrassEffect *work);
void VeryTallGrassEffect_Update(FieldEffect *effect, VeryTallGrassEffect *work);
void VeryTallGrassEffect_Draw(FieldEffect *effect, VeryTallGrassEffect *work);

// In asm/overlay_01_021FF854_data_local.s.
extern const FieldEffectBasicTemplate sVeryTallGrassEffectTemplate; // very tall grass effect
extern const u32 sVeryTallGrassRecordParams[6];                   // very tall grass record params

VeryTallGrass *VeryTallGrass_New(FieldEffectManager *manager) {
    VeryTallGrass *grass = FieldEffectManager_HeapAllocAndFill(manager, sizeof(VeryTallGrass), FALSE, 0);
    grass->manager = manager;
    VeryTallGrass_LoadGfx(grass);
    return grass;
}

void VeryTallGrass_Free(VeryTallGrass *grass) {
    VeryTallGrass_FreeGfx(grass);
    FieldEffectManager_HeapFree(grass);
}

void VeryTallGrass_LoadGfx(VeryTallGrass *grass) {
    FieldEffectManager_LoadBillboardResource(grass->manager, VERY_TALL_GRASS_RESOURCE, 0x7A);
    FieldEffectManager_LoadBillboardSequence(grass->manager, VERY_TALL_GRASS_SEQUENCE, 0x92);
    FieldEffectManager_LoadTexResource(grass->manager, VERY_TALL_GRASS_TEXTURE, 0x15, TRUE);
    FieldEffectManager_AddRecord(grass->manager, VERY_TALL_GRASS_RECORD, VERY_TALL_GRASS_RESOURCE, VERY_TALL_GRASS_SEQUENCE, VERY_TALL_GRASS_TEXTURE, 0, sVeryTallGrassRecordParams);
}

void VeryTallGrass_FreeGfx(VeryTallGrass *grass) {
    FieldEffectManager_FreeBillboardResource(grass->manager, VERY_TALL_GRASS_RESOURCE);
    FieldEffectManager_FreeBillboardSequence(grass->manager, VERY_TALL_GRASS_SEQUENCE);
    FieldEffectManager_FreeTexResource(grass->manager, VERY_TALL_GRASS_TEXTURE);
    FieldEffectManager_ReleaseRecord(grass->manager, VERY_TALL_GRASS_RECORD);
}

void VeryTallGrass_Spawn(LocalMapObject *mapObject, BOOL animate) {
    VecFx32 position;
    VeryTallGrassEffectArgs args;
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);

    args.x = MapObject_GetXCoord(mapObject);
    args.y = MapObject_GetYCoord(mapObject);
    args.z = MapObject_GetZCoord(mapObject);
    args.dx = 0;
    args.dz = 0;
    args.direction = -1;
    args.manager = manager;
    args.fieldSystem = FieldEffectManager_GetFieldSystem(manager);
    args.grass = FieldEffectManager_GetRendererData(manager, 12);
    args.mapObject = mapObject;
    MapObject_CopyPositionVector(mapObject, &position);
    FieldEffectManager_CreateEffect(manager, &sVeryTallGrassEffectTemplate, &position, animate, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
}

void VeryTallGrass_SpawnAt(LocalMapObject *mapObject, BOOL animate, int x, int z, BOOL checkFacing) {
    VecFx32 position;
    VeryTallGrassEffectArgs args;
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
    args.grass = FieldEffectManager_GetRendererData(manager, 12);
    args.mapObject = mapObject;
    MapObject_CopyPositionVector(mapObject, &position);
    FieldEffectManager_CreateEffect(manager, &sVeryTallGrassEffectTemplate, &position, animate, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
}

BOOL VeryTallGrassEffect_Init(FieldEffect *effect, VeryTallGrassEffect *work) {
    VecFx32 position;

    work->args = *(VeryTallGrassEffectArgs *)FieldEffect_GetUserData(effect);
    work->spriteId = MapObject_GetSpriteID(work->args.mapObject);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    position.x = work->args.x << 16;
    position.z = work->args.z << 16;
    position.y = MapObject_GetPositionVectorYCoord(work->args.mapObject);
    work->onGround = FieldSystem_SnapToTerrainHeight(work->args.fieldSystem, &position);
    position.x += 2 << 14;
    position.z += 0x12 << 12;
    FieldEffect_SetPosition(effect, &position);
    work->billboard = FieldEffectManager_CreateBillboardFromRecord(work->args.manager, VERY_TALL_GRASS_RECORD, &position);
    if (MapObject_TestFlagsBits(work->args.mapObject, MAPOBJECTFLAG_VISIBLE) == TRUE) {
        BillboardObject_SetVisible(work->billboard, FALSE);
    }
    if (FieldEffect_GetParam(effect) == FALSE) {
        BillboardObject_SetFrame(work->billboard, 12 * FX32_ONE);
        BillboardObject_AdvanceFrame(work->billboard, 0);
        work->state = 2;
    }
    return TRUE;
}

void VeryTallGrassEffect_Destroy(FieldEffect *effect, VeryTallGrassEffect *work) {
    BillboardObject_Delete(work->billboard);
}

void VeryTallGrassEffect_Update(FieldEffect *effect, VeryTallGrassEffect *work) {
    VecFx32 current;
    VecFx32 position;
    LocalMapObject *mapObject = work->args.mapObject;

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
        BillboardObject_AdvanceFrame(work->billboard, FX32_ONE);
        if (BillboardObject_GetSequenceFrame(work->billboard) / FX32_ONE < 12) {
            return;
        }
        work->state = 1;
        return;
    case 1:
        BillboardObject_SetFrame(work->billboard, 12 * FX32_ONE);
        BillboardObject_AdvanceFrame(work->billboard, 0);
        work->state = 2;
        // fallthrough
    case 2: {
        int x = MapObject_GetXCoord(mapObject) - work->args.dx;
        int z = MapObject_GetZCoord(mapObject) - work->args.dz;
        if (work->args.x != x || work->args.z != z) {
            FieldEffectManager_DestroyEffect((int)effect);
            return;
        }
        if (work->args.direction != -1 && work->args.direction != MapObject_GetFacingDirection(mapObject)) {
            FieldEffectManager_DestroyEffect((int)effect);
        }
    }
        return;
    }
}

void VeryTallGrassEffect_Draw(FieldEffect *effect, VeryTallGrassEffect *work) {
    VecFx32 position;

    if (!MapObject_CheckSpriteIDAndMap(work->args.mapObject, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    FieldEffect_GetPosition(effect, &position);
    BillboardObject_SetPosition(work->billboard, &position);
}
