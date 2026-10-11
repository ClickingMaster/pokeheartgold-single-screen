#include "global.h"

#include "constants/global_fieldmap.h"
#include "constants/scrcmd.h"
#include "constants/sprites.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"

#include "billboard_lists.h"
#include "field_effects.h"
#include "field_system.h"
#include "map_object.h"
#include "overlay_01_021F72DC.h"
#include "overlay_01_021F944C.h"
#include "overlay_01_022053EC.h"
#include "player_avatar.h"

// Map object reflections (field effect renderer 1): an upside-down copy of the object's billboard below it
// while it stands on a reflective tile, showing the object's current sequence and frame. Kinds 0-2: the
// tile is a puddle, other water, or a reflective floor (whose reflection does not ripple); kinds 3-5: the
// same for a large following Pokemon. The rippling is a horizontal scale swinging between 0.875 and 1.125.

#define REFLECTION_RESOURCE       2
#define REFLECTION_RESOURCE_LARGE 13

#define REFLECTION_KIND_FLOOR       2
#define REFLECTION_KIND_LARGE_FLOOR 5
#define REFLECTION_KIND_COUNT       6

#define REFLECTION_SCALE_STEP 0x40
#define REFLECTION_SCALE_MIN  0xE00
#define REFLECTION_SCALE_MAX  0x1200

// Puts position->y on the ground of the tile at position->x/z; FALSE while that tile has no ground yet.
// (unk_0205FD20.h declares it returning void.)
BOOL sub_0206121C(FieldSystem *fieldSystem, VecFx32 *position);

// A billboard record (struct FieldEffectRecord, 40 bytes) whose first word is the billboard resource.
typedef struct ReflectionRecord {
    void *resource;
    u32 unk04[9];
} ReflectionRecord;

// Copies the billboard record of the sprite into record; FALSE if the sprite has none.
// (field object models, still in asm)
BOOL ov01_021F9744(MapObjectManager *manager, u32 spriteId, ReflectionRecord *record);
// field_effect_dispatch_3.c: swaps the record's sequence for a following Pokemon's alternate one.
void ov01_02205870(BOOL select, LocalMapObject *mapObject, void *billboard, ReflectionRecord *record);

// Renderer data, from FieldEffectManager_GetRendererData(manager, 1).
typedef struct Reflection {
    FieldEffectManager *manager;
} Reflection;

typedef struct ReflectionEffectArgs {
    FieldSystem *fieldSystem;
    FieldEffectManager *manager;
    Reflection *reflection;
    LocalMapObject *mapObject;
} ReflectionEffectArgs;

typedef struct ReflectionEffect {
    u32 objectId;
    u32 mapId;
    u32 spriteId;
    BOOL hasBillboard;
    u32 kind;
    ReflectionEffectArgs args;
    void *billboard;
    VecFx32 scale;
    fx32 scaleStep;
} ReflectionEffect;

// The reflection of a billboard that is not (or no longer) drawn by a map object, at a fixed position.
typedef struct BillboardReflectionArgs {
    FieldSystem *fieldSystem;
    FieldEffectManager *manager;
    Reflection *reflection;
    ReflectionRecord record;
    void *source;
} BillboardReflectionArgs;

// Only the first sizeof(ReflectionEffect) bytes are cleared: the template is shared in size with the
// map object reflection.
typedef struct BillboardReflectionEffect {
    int kind;
    BillboardReflectionArgs args;
    void *billboard;
    VecFx32 scale;
    fx32 scaleStep;
    VecFx32 position;
} BillboardReflectionEffect;

// How far below the object's ground the reflection is drawn, per kind.
typedef struct ReflectionDepths {
    fx32 depth[REFLECTION_KIND_COUNT];
} ReflectionDepths;

Reflection *Reflection_New(FieldEffectManager *manager);
void Reflection_Free(Reflection *reflection);
void Reflection_LoadGfx(Reflection *reflection);
void Reflection_FreeGfx(Reflection *reflection);
void Reflection_Spawn(LocalMapObject *mapObject, int kind);
BOOL ReflectionEffect_Init(FieldEffect *effect, ReflectionEffect *work);
void ReflectionEffect_Destroy(FieldEffect *effect, ReflectionEffect *work);
void ReflectionEffect_Update(FieldEffect *effect, ReflectionEffect *work);
void ReflectionEffect_Draw(FieldEffect *effect, ReflectionEffect *work);
void ReflectionEffect_CalcPosition(ReflectionEffect *work, LocalMapObject *mapObject, VecFx32 *position);
void ReflectionEffect_CreateBillboard(FieldEffect *effect, ReflectionEffect *work);
void BillboardReflectionEffect_CreateBillboard(FieldEffect *effect, BillboardReflectionEffect *work);
void BillboardReflectionEffect_CalcPosition(BillboardReflectionEffect *work, VecFx32 *position);
FieldEffect *Reflection_SpawnForBillboard(FieldEffectManager *manager, const ReflectionRecord *record, void *source, const VecFx32 *position, int kind, u32 priority);
BOOL BillboardReflectionEffect_Init(FieldEffect *effect, BillboardReflectionEffect *work);
void BillboardReflectionEffect_Destroy(FieldEffect *effect, BillboardReflectionEffect *work);
void BillboardReflectionEffect_Update(FieldEffect *effect, BillboardReflectionEffect *work);
void BillboardReflectionEffect_Draw(FieldEffect *effect, BillboardReflectionEffect *work);
void Reflection_SpawnForFollower(LocalMapObject *mapObject, int kind);
BOOL FollowerReflectionEffect_Init(FieldEffect *effect, ReflectionEffect *work);
void FollowerReflectionEffect_Update(FieldEffect *effect, ReflectionEffect *work);
void FollowerReflectionEffect_Draw(FieldEffect *effect, ReflectionEffect *work);
void FollowerReflectionEffect_CreateBillboard(FieldEffect *effect, ReflectionEffect *work);

// The three effect templates stay in asm (asm/overlay_01_021FDA14_residual_1.s, linked ahead of this file).
extern const FieldEffectBasicTemplate ov01_02208F74; // follower
extern const FieldEffectBasicTemplate ov01_02208F88; // map object
extern const FieldEffectBasicTemplate ov01_02208F9C; // billboard

static const ReflectionDepths sReflectionDepths = {
    { 14 * FX32_ONE, 20 * FX32_ONE, 14 * FX32_ONE, 14 * FX32_ONE, 20 * FX32_ONE, 14 * FX32_ONE }
};

static inline void Reflection_SpawnWithTemplate(LocalMapObject *mapObject, int kind, const FieldEffectBasicTemplate *template) {
    ReflectionEffectArgs args;
    VecFx32 position;
    args.fieldSystem = MapObject_GetFieldSystem(mapObject);
    args.manager = FieldEffectManager_GetFromMapObject(mapObject);
    args.reflection = FieldEffectManager_GetRendererData(args.manager, 1);
    args.mapObject = mapObject;
    MapObject_CopyPositionVector(mapObject, &position);
    FieldEffectManager_CreateEffect(args.manager, template, &position, kind, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
}

static inline BOOL ReflectionEffect_IsObjectGone(ReflectionEffect *work, LocalMapObject *mapObject) {
    return work->spriteId != MapObject_GetSpriteID(mapObject) || !sub_0205F0A8(mapObject, work->objectId, work->mapId) || !MapObject_CheckFlag24(mapObject);
}

Reflection *Reflection_New(FieldEffectManager *manager) {
    Reflection *reflection = FieldEffectManager_HeapAllocAndFill(manager, sizeof(Reflection), FALSE, 0);
    reflection->manager = manager;
    Reflection_LoadGfx(reflection);
    return reflection;
}

void Reflection_Free(Reflection *reflection) {
    Reflection_FreeGfx(reflection);
    FieldEffectManager_HeapFree(reflection);
}

void Reflection_LoadGfx(Reflection *reflection) {
    FieldEffectManager_LoadBillboardResource(reflection->manager, REFLECTION_RESOURCE, 0x23);
    FieldEffectManager_LoadBillboardResource(reflection->manager, REFLECTION_RESOURCE_LARGE, 0x69);
}

void Reflection_FreeGfx(Reflection *reflection) {
    FieldEffectManager_FreeBillboardResource(reflection->manager, REFLECTION_RESOURCE);
    FieldEffectManager_FreeBillboardResource(reflection->manager, REFLECTION_RESOURCE_LARGE);
}

void Reflection_Spawn(LocalMapObject *mapObject, int kind) {
    Reflection_SpawnWithTemplate(mapObject, kind, &ov01_02208F88);
}

BOOL ReflectionEffect_Init(FieldEffect *effect, ReflectionEffect *work) {
    VecFx32 position;
    work->args = *(ReflectionEffectArgs *)FieldEffect_GetUserData(effect);
    work->kind = FieldEffect_GetParam(effect);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    work->spriteId = MapObject_GetSpriteID(work->args.mapObject);
    work->scale.x = FX32_ONE;
    work->scale.y = FX32_ONE;
    work->scale.z = FX32_ONE;
    work->scaleStep = REFLECTION_SCALE_STEP;
    if (work->kind == REFLECTION_KIND_FLOOR) {
        work->scaleStep = 0;
    }
    ReflectionEffect_CalcPosition(work, work->args.mapObject, &position);
    FieldEffect_SetPosition(effect, &position);
    ReflectionEffect_CreateBillboard(effect, work);
    return TRUE;
}

void ReflectionEffect_Destroy(FieldEffect *effect, ReflectionEffect *work) {
    if (work->hasBillboard == TRUE) {
        BillboardObject_Delete(work->billboard);
    }
}

void ReflectionEffect_Update(FieldEffect *effect, ReflectionEffect *work) {
    VecFx32 position;
    LocalMapObject *mapObject = work->args.mapObject;
    if (ReflectionEffect_IsObjectGone(work, mapObject)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    work->scale.x += work->scaleStep;
    if (work->scale.x >= REFLECTION_SCALE_MAX) {
        work->scale.x = REFLECTION_SCALE_MAX;
        work->scaleStep = -work->scaleStep;
    } else if (work->scale.x <= REFLECTION_SCALE_MIN) {
        work->scale.x = REFLECTION_SCALE_MIN;
        work->scaleStep = -work->scaleStep;
    }
    ReflectionEffect_CalcPosition(work, mapObject, &position);
    FieldEffect_SetPosition(effect, &position);
    if (work->hasBillboard == FALSE) {
        ReflectionEffect_CreateBillboard(effect, work);
    }
}

void ReflectionEffect_Draw(FieldEffect *effect, ReflectionEffect *work) {
    VecFx32 facing;
    VecFx32 position;
    LocalMapObject *mapObject = work->args.mapObject;
    Sprite *source;
    BOOL hidden = FALSE;

    if (ReflectionEffect_IsObjectGone(work, mapObject)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    if (work->hasBillboard == FALSE) {
        return;
    }
    MapObject_CopyFacingVector(work->args.mapObject, &facing);
    if (facing.y != 0) {
        hidden = TRUE;
    }
    if (MapObject_CheckVisible(mapObject) == TRUE) {
        hidden = TRUE;
    }
    if (hidden) {
        BillboardObject_SetVisible(work->billboard, FALSE);
    } else {
        BillboardObject_SetVisible(work->billboard, TRUE);
    }
    FieldEffect_GetPosition(effect, &position);
    BillboardObject_SetPosition(work->billboard, &position);
    BillboardObject_SetScale(work->billboard, &work->scale);
    source = ov01_021F72DC(work->args.mapObject);
    BillboardObject_SetSequence(work->billboard, BillboardObject_GetSequence(source));
    BillboardObject_SetFrame(work->billboard, BillboardObject_GetFrame(source));
}

void ReflectionEffect_CalcPosition(ReflectionEffect *work, LocalMapObject *mapObject, VecFx32 *position) {
    const ReflectionDepths depths = {
        { 14 * FX32_ONE, 20 * FX32_ONE, 14 * FX32_ONE, 14 * FX32_ONE, 20 * FX32_ONE, 14 * FX32_ONE }
    };
    VecFx32 facing;
    ReflectionDepths depthTable;
    fx32 xOffset;
    fx32 zOffset;
    fx32 yOffset;
    BOOL onGround;

    depthTable = depths;
    MapObject_CopyFacingVector(mapObject, &facing);
    xOffset = facing.x;
    if (MapObject_GetID(mapObject) == obj_player && MapObject_GetSpriteID(mapObject) == SPRITE_FISHINGHERO) {
        fx32 base;
        int direction;
        direction = MapObject_GetFacingDirection(mapObject);
        base = 2;
        base <<= 10;
        if (direction == DIR_SOUTH) {
            zOffset = base;
            zOffset += base << 1;
            zOffset -= facing.z;
        } else {
            zOffset = facing.z;
        }
    } else {
        zOffset = facing.z;
    }
    yOffset = -(facing.y / 6);
    if (MapObject_GetID(mapObject) == obj_partner_poke && MapObject_GetSpriteID(PlayerAvatar_GetMapObject(work->args.fieldSystem->playerAvatar)) == SPRITE_FISHINGHERO) {
        yOffset = 0;
    }
    MapObject_CopyPositionVector(mapObject, position);
    onGround = sub_0206121C(work->args.fieldSystem, position);
    position->x += xOffset;
    position->z += zOffset - 7 * FX32_ONE;
    if (onGround == FALSE) {
        position->y = 0;
    } else {
        position->y -= depthTable.depth[work->kind];
    }
    position->y += yOffset;
}

void ReflectionEffect_CreateBillboard(FieldEffect *effect, ReflectionEffect *work) {
    VecFx32 position;
    ReflectionRecord record;
    if (ov01_021F9744(MapObject_GetManager(work->args.mapObject), work->spriteId, &record) && ov01_021FA2D4(work->args.mapObject) != TRUE) {
        record.resource = FieldEffectManager_GetBillboardResource(work->args.manager, REFLECTION_RESOURCE);
        FieldEffect_GetPosition(effect, &position);
        work->billboard = FieldEffectManager_CreateBillboard(work->args.manager, (struct FieldEffectRecord *)&record, &position);
        work->hasBillboard = TRUE;
    }
}

void BillboardReflectionEffect_CreateBillboard(FieldEffect *effect, BillboardReflectionEffect *work) {
    VecFx32 position;
    ReflectionRecord record;
    void *source;

    record = work->args.record;
    record.resource = FieldEffectManager_GetBillboardResource(work->args.manager, REFLECTION_RESOURCE);
    FieldEffect_GetPosition(effect, &position);
    work->billboard = FieldEffectManager_CreateBillboard(work->args.manager, (struct FieldEffectRecord *)&record, &position);
    source = work->args.source;
    BillboardObject_SetSequence(work->billboard, BillboardObject_GetSequence(source));
    BillboardObject_SetSequenceFrame(work->billboard, BillboardObject_GetSequenceFrame(source));
    BillboardObject_SetFrame(work->billboard, BillboardObject_GetFrame(source));
    BillboardObject_AdvanceFrame(work->billboard, 0);
    BillboardObject_UpdateTextureAnimation(work->billboard);
}

void BillboardReflectionEffect_CalcPosition(BillboardReflectionEffect *work, VecFx32 *position) {
    ReflectionDepths depths = sReflectionDepths;
    BOOL onGround;
    *position = work->position;
    onGround = sub_0206121C(work->args.fieldSystem, position);
    position->z -= 7 * FX32_ONE;
    if (onGround == FALSE) {
        position->y = 0;
    } else {
        position->y -= depths.depth[work->kind];
    }
}

FieldEffect *Reflection_SpawnForBillboard(FieldEffectManager *manager, const ReflectionRecord *record, void *source, const VecFx32 *position, int kind, u32 priority) {
    BillboardReflectionArgs args;
    args.fieldSystem = FieldEffectManager_GetFieldSystem(manager);
    args.manager = manager;
    args.reflection = FieldEffectManager_GetRendererData(manager, 1);
    args.record = *record;
    args.source = source;
    return FieldEffectManager_CreateEffect(manager, &ov01_02208F9C, position, kind, &args, priority);
}

BOOL BillboardReflectionEffect_Init(FieldEffect *effect, BillboardReflectionEffect *work) {
    VecFx32 position;
    work->args = *(BillboardReflectionArgs *)FieldEffect_GetUserData(effect);
    work->kind = FieldEffect_GetParam(effect);
    work->scale.x = FX32_ONE;
    work->scale.y = FX32_ONE;
    work->scale.z = FX32_ONE;
    work->scaleStep = REFLECTION_SCALE_STEP;
    if (work->kind == REFLECTION_KIND_FLOOR) {
        work->scaleStep = 0;
    }
    FieldEffect_GetPosition(effect, &work->position);
    BillboardReflectionEffect_CalcPosition(work, &position);
    FieldEffect_SetPosition(effect, &position);
    BillboardReflectionEffect_CreateBillboard(effect, work);
    FieldEffect_RunUpdate(effect);
    return TRUE;
}

void BillboardReflectionEffect_Destroy(FieldEffect *effect, BillboardReflectionEffect *work) {
    BillboardObject_Delete(work->billboard);
}

void BillboardReflectionEffect_Update(FieldEffect *effect, BillboardReflectionEffect *work) {
    VecFx32 position;
    work->scale.x += work->scaleStep;
    if (work->scale.x >= REFLECTION_SCALE_MAX) {
        work->scale.x = REFLECTION_SCALE_MAX;
        work->scaleStep = -work->scaleStep;
    } else if (work->scale.x <= REFLECTION_SCALE_MIN) {
        work->scale.x = REFLECTION_SCALE_MIN;
        work->scaleStep = -work->scaleStep;
    }
    BillboardReflectionEffect_CalcPosition(work, &position);
    FieldEffect_SetPosition(effect, &position);
}

void BillboardReflectionEffect_Draw(FieldEffect *effect, BillboardReflectionEffect *work) {
    VecFx32 position;
    FieldEffect_GetPosition(effect, &position);
    BillboardObject_SetPosition(work->billboard, &position);
    BillboardObject_SetScale(work->billboard, &work->scale);
}

void Reflection_SpawnForFollower(LocalMapObject *mapObject, int kind) {
    Reflection_SpawnWithTemplate(mapObject, kind, &ov01_02208F74);
}

BOOL FollowerReflectionEffect_Init(FieldEffect *effect, ReflectionEffect *work) {
    VecFx32 position;
    work->args = *(ReflectionEffectArgs *)FieldEffect_GetUserData(effect);
    work->kind = FieldEffect_GetParam(effect);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    work->spriteId = MapObject_GetSpriteID(work->args.mapObject);
    work->scale.x = FX32_ONE;
    work->scale.y = FX32_ONE;
    work->scale.z = FX32_ONE;
    work->scaleStep = REFLECTION_SCALE_STEP;
    if (work->kind == REFLECTION_KIND_FLOOR || work->kind == REFLECTION_KIND_LARGE_FLOOR) {
        work->scaleStep = 0;
    }
    ReflectionEffect_CalcPosition(work, work->args.mapObject, &position);
    position.y -= 0x514;
    FieldEffect_SetPosition(effect, &position);
    FollowerReflectionEffect_CreateBillboard(effect, work);
    return TRUE;
}

void FollowerReflectionEffect_Update(FieldEffect *effect, ReflectionEffect *work) {
    VecFx32 position;
    LocalMapObject *mapObject = work->args.mapObject;
    if (ReflectionEffect_IsObjectGone(work, mapObject)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    work->scale.x += work->scaleStep;
    if (work->scale.x >= REFLECTION_SCALE_MAX) {
        work->scale.x = REFLECTION_SCALE_MAX;
        work->scaleStep = -work->scaleStep;
    } else if (work->scale.x <= REFLECTION_SCALE_MIN) {
        work->scale.x = REFLECTION_SCALE_MIN;
        work->scaleStep = -work->scaleStep;
    }
    ReflectionEffect_CalcPosition(work, mapObject, &position);
    position.y -= 0x514;
    FieldEffect_SetPosition(effect, &position);
    if (work->hasBillboard == FALSE) {
        FollowerReflectionEffect_CreateBillboard(effect, work);
    }
}

void FollowerReflectionEffect_Draw(FieldEffect *effect, ReflectionEffect *work) {
    VecFx32 position;
    LocalMapObject *mapObject = work->args.mapObject;
    Sprite *source;

    if (ReflectionEffect_IsObjectGone(work, mapObject)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    if (work->hasBillboard == FALSE) {
        return;
    }
    if (MapObject_CheckVisible(mapObject) == TRUE) {
        BillboardObject_SetVisible(work->billboard, FALSE);
    } else {
        BillboardObject_SetVisible(work->billboard, TRUE);
    }
    FieldEffect_GetPosition(effect, &position);
    position.z -= FX32_ONE;
    BillboardObject_SetPosition(work->billboard, &position);
    BillboardObject_SetScale(work->billboard, &work->scale);
    source = ov01_021F72DC(work->args.mapObject);
    BillboardObject_SetSequence(work->billboard, BillboardObject_GetSequence(source));
    BillboardObject_SetFrame(work->billboard, BillboardObject_GetFrame(source));
}

void FollowerReflectionEffect_CreateBillboard(FieldEffect *effect, ReflectionEffect *work) {
    VecFx32 position;
    ReflectionRecord record;
    void *resource;
    if (ov01_021F9744(MapObject_GetManager(work->args.mapObject), work->spriteId, &record) && ov01_021FA2D4(work->args.mapObject) != TRUE) {
        if (work->kind <= REFLECTION_KIND_FLOOR) {
            resource = FieldEffectManager_GetBillboardResource(work->args.manager, REFLECTION_RESOURCE);
        } else {
            resource = FieldEffectManager_GetBillboardResource(work->args.manager, REFLECTION_RESOURCE_LARGE);
        }
        record.resource = resource;
        if (ov01_0220553C(work->args.mapObject)) {
            ov01_02205870(TRUE, work->args.mapObject, work->billboard, &record);
        }
        FieldEffect_GetPosition(effect, &position);
        work->billboard = FieldEffectManager_CreateBillboard(work->args.manager, (struct FieldEffectRecord *)&record, &position);
        work->hasBillboard = TRUE;
    }
}
