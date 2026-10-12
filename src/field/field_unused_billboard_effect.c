#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"

#include "billboard_lists.h"
#include "map_object.h"
#include "field_effects.h"

// Field effect renderer 13: a billboard over a map object's tile in the style of the grass effects (two
// frames, then held while the object stays on the tile). Nothing creates it: its template, ov01_02209244,
// is referenced from nowhere.

#define OV01_021FFC0C_RECORD   10
#define OV01_021FFC0C_RESOURCE 8
#define OV01_021FFC0C_SEQUENCE 8
#define OV01_021FFC0C_TEXTURE  9

// Puts position->y on the ground of the tile at position->x/z; FALSE while that tile has no ground yet.
// (unk_0205FD20.h declares it returning void.)
BOOL FieldSystem_SnapToTerrainHeight(FieldSystem *fieldSystem, VecFx32 *position);

// Renderer data, from FieldEffectManager_GetRendererData(manager, 13).
typedef struct UnkOv01_021FFC0C {
    FieldEffectManager *manager;
} UnkOv01_021FFC0C;

typedef struct UnkOv01_021FFC0C_Args {
    int x;
    int y;
    int z;
    FieldSystem *fieldSystem;
    FieldEffectManager *manager;
    UnkOv01_021FFC0C *renderer;
    LocalMapObject *mapObject;
    s16 dx;
    s16 dz;
    s8 direction; // -1: any facing
} UnkOv01_021FFC0C_Args;

typedef struct UnkOv01_021FFC0C_Effect {
    int state;
    u32 spriteId;
    u32 objectId;
    u32 mapId;
    u8 unk10[4];
    BOOL onGround;
    UnkOv01_021FFC0C_Args args;
    void *billboard;
} UnkOv01_021FFC0C_Effect;

UnkOv01_021FFC0C *ov01_021FFC0C(FieldEffectManager *manager);
void ov01_021FFC28(UnkOv01_021FFC0C *renderer);
void ov01_021FFC38(UnkOv01_021FFC0C *renderer);
void ov01_021FFC80(UnkOv01_021FFC0C *renderer);
BOOL ov01_021FFCA8(FieldEffect *effect, UnkOv01_021FFC0C_Effect *work);
void ov01_021FFD64(FieldEffect *effect, UnkOv01_021FFC0C_Effect *work);
void ov01_021FFD70(FieldEffect *effect, UnkOv01_021FFC0C_Effect *work);
void ov01_021FFE98(FieldEffect *effect, UnkOv01_021FFC0C_Effect *work);

static const u32 ov01_02209258[6] = { 0, 2, 0, 0, 0, 2 };

UnkOv01_021FFC0C *ov01_021FFC0C(FieldEffectManager *manager) {
    UnkOv01_021FFC0C *renderer = FieldEffectManager_HeapAllocAndFill(manager, sizeof(UnkOv01_021FFC0C), FALSE, 0);
    renderer->manager = manager;
    ov01_021FFC38(renderer);
    return renderer;
}

void ov01_021FFC28(UnkOv01_021FFC0C *renderer) {
    ov01_021FFC80(renderer);
    FieldEffectManager_HeapFree(renderer);
}

void ov01_021FFC38(UnkOv01_021FFC0C *renderer) {
    FieldEffectManager_LoadBillboardResource(renderer->manager, OV01_021FFC0C_RESOURCE, 0x7B);
    FieldEffectManager_LoadBillboardSequence(renderer->manager, OV01_021FFC0C_SEQUENCE, 0x93);
    FieldEffectManager_LoadTexResource(renderer->manager, OV01_021FFC0C_TEXTURE, 0x16, TRUE);
    FieldEffectManager_AddRecord(renderer->manager, OV01_021FFC0C_RECORD, OV01_021FFC0C_RESOURCE, OV01_021FFC0C_SEQUENCE, OV01_021FFC0C_TEXTURE, 0, ov01_02209258);
}

void ov01_021FFC80(UnkOv01_021FFC0C *renderer) {
    FieldEffectManager_FreeBillboardResource(renderer->manager, OV01_021FFC0C_RESOURCE);
    FieldEffectManager_FreeBillboardSequence(renderer->manager, OV01_021FFC0C_SEQUENCE);
    FieldEffectManager_FreeTexResource(renderer->manager, OV01_021FFC0C_TEXTURE);
    FieldEffectManager_ReleaseRecord(renderer->manager, OV01_021FFC0C_RECORD);
}

BOOL ov01_021FFCA8(FieldEffect *effect, UnkOv01_021FFC0C_Effect *work) {
    VecFx32 position;

    work->args = *(UnkOv01_021FFC0C_Args *)FieldEffect_GetUserData(effect);
    work->spriteId = MapObject_GetSpriteID(work->args.mapObject);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    position.x = work->args.x << 16;
    position.z = work->args.z << 16;
    position.y = MapObject_GetPositionVectorYCoord(work->args.mapObject);
    work->onGround = FieldSystem_SnapToTerrainHeight(work->args.fieldSystem, &position);
    position.x += 0x1E << 10;
    position.y -= 2 << 14;
    position.z += 0xD << 12;
    FieldEffect_SetPosition(effect, &position);
    work->billboard = FieldEffectManager_CreateBillboardFromRecord(work->args.manager, OV01_021FFC0C_RECORD, &position);
    if (MapObject_TestFlagsBits(work->args.mapObject, MAPOBJECTFLAG_VISIBLE) == TRUE) {
        BillboardObject_SetVisible(work->billboard, FALSE);
    }
    if (FieldEffect_GetParam(effect) == 0) {
        BillboardObject_SetFrame(work->billboard, 2 * FX32_ONE);
        work->state = 2;
    }
    return TRUE;
}

void ov01_021FFD64(FieldEffect *effect, UnkOv01_021FFC0C_Effect *work) {
    BillboardObject_Delete(work->billboard);
}

void ov01_021FFD70(FieldEffect *effect, UnkOv01_021FFC0C_Effect *work) {
    struct {
        VecFx32 target;
        VecFx32 current;
    } vecs;
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
        FieldEffect_GetPosition(effect, &vecs.current);
        vecs.target.x = work->args.x << 16;
        vecs.target.z = work->args.z << 16;
        vecs.target.y = vecs.current.y;
        work->onGround = FieldSystem_SnapToTerrainHeight(work->args.fieldSystem, &vecs.target);
        if (work->onGround == TRUE) {
            vecs.current.y = vecs.target.y;
            FieldEffect_SetPosition(effect, &vecs.current);
        }
    }
    switch (work->state) {
    case 0:
        BillboardObject_AdvanceFrame(work->billboard, FX32_ONE);
        if (BillboardObject_GetSequenceFrame(work->billboard) / FX32_ONE >= 2) {
            work->state = 1;
        }
        break;
    case 1:
        BillboardObject_SetFrame(work->billboard, 2 * FX32_ONE);
        BillboardObject_AdvanceFrame(work->billboard, 0);
        work->state = 2;
        // fallthrough
    case 2:
        if (!MapObject_CheckSpriteIDAndMap(mapObject, work->spriteId, work->objectId, work->mapId)) {
            FieldEffectManager_DestroyEffect((int)effect);
            return;
        }
        x = MapObject_GetXCoord(mapObject);
        z = MapObject_GetZCoord(mapObject);
        if (work->args.x != x || work->args.z != z) {
            FieldEffectManager_DestroyEffect((int)effect);
            return;
        }
        if (work->args.direction != -1 && work->args.direction != MapObject_GetFacingDirection(mapObject)) {
            FieldEffectManager_DestroyEffect((int)effect);
        }
        break;
    }
}

void ov01_021FFE98(FieldEffect *effect, UnkOv01_021FFC0C_Effect *work) {
    VecFx32 position;

    if (!MapObject_CheckIDAndMap(work->args.mapObject, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    FieldEffect_GetPosition(effect, &position);
    BillboardObject_SetPosition(work->billboard, &position);
}
