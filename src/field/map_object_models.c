#include "field/map_object_models.h"

#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_follow_mon.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"

// The work of a following Pokemon's billboard (the map object's draw work, 0x18 bytes).
typedef struct FollowMonBillboardWork {
    Sprite *billboard;
    VecFx32 savedState;
    s8 direction;
    u8 unk11;
    s8 shadowOffsetX;      // in tiles, while the shadow is anchored
    s8 shadowOffsetZ;
    s8 height;             // extra height in units, while the shadow is anchored
    u8 bobbing;
    u8 unk16;
    u8 paletteApplied : 1; // cleared to reload the sepia / alternate palette on the next update
    u8 unk17_1 : 1;
    u8 shadowAnchored : 1; // the shadow stays at shadowOffsetX/Z while the Pokemon hops (follow mon interactions)
    u8 : 5;
} FollowMonBillboardWork;

// The work of the Ho-Oh / Lugia objects (SPRITE_HOU_OBJ01, SPRITE_LUG_OBJ01): two looping sequences picked by the
// object's animation number.
typedef struct LegendaryBirdObject {
    Sprite *billboard;
    VecFx32 savedState;
    u32 sequence;
} LegendaryBirdObject;

typedef void (*LegendaryBirdAnimateFunc)(LocalMapObject *object, Sprite *billboard, LegendaryBirdObject *work);

// Map object render (MapObjectRender_Init group, no header yet).
void MapObjectRender_Init(void *render, MapObjectManager *manager, u32 count, u32 priority, int arg1, int objectsToPreload, const int *objectIDs, int arg4);
void MapObjectRender_Free(void *render);
void MapObject_CreateBillboard(LocalMapObject *object, void *work);
void MapObject_DeleteBillboard(LocalMapObject *object, void *work);
void BillboardObject_SaveState(Sprite *billboard, VecFx32 *state);
void BillboardObject_RestoreState(Sprite *billboard, VecFx32 *state);
u32 ov01_021FA298(u32 spriteId);
BOOL MapObject_IsBillboardPending(LocalMapObject *object);
void MapObject_UpdateBillboardPosition(LocalMapObject *object, Sprite *billboard);
int Facing_GetBillboardStandSequence(u32 direction);
// field_shadow.c
void FieldShadow_OffsetByFacing(u32 direction, VecFx32 *offset);
// billboard_lists.h declares an int (not included); this caller narrows to u8.
void BillboardObject_SetVisible(Sprite *billboard, u8 visible);

BOOL FollowMonBillboard_IsBobFrame(u8 direction, LocalMapObject *object, Sprite *billboard);
void LegendaryBirdObject_CreateBillboard(LocalMapObject *object);
void LegendaryBirdObject_AnimateIdle(LocalMapObject *object, Sprite *billboard, LegendaryBirdObject *work);
void LegendaryBirdObject_AnimateFlap(LocalMapObject *object, Sprite *billboard, LegendaryBirdObject *work);
void LegendaryBirdObject_UpdateVisibility(LocalMapObject *object, Sprite *billboard);
void LegendaryBirdObject_DeleteBillboard(LocalMapObject *object);

// Per sprite id, from overlay_01_sprite_data.s: the graphics info (terminated by spriteId 0xFFFF) and the effect flags
// its unk4_0 field selects.
extern ObjectEventGraphicsInfo ov01_022074A8[];
extern u16 ov01_02206D00[];

static const LegendaryBirdAnimateFunc sLegendaryBirdAnimateFuncs[] = {
    LegendaryBirdObject_AnimateIdle,
    LegendaryBirdObject_AnimateFlap,
};

void FollowMonBillboard_Update(LocalMapObject *object, Sprite *billboard, void *work, u32 direction, BOOL addFacingOffset) {
    FollowMonBillboardWork *billboardWork = work;
    BOOL flag4;
    int hasFollowMonSprite;
    BOOL paused;
    VecFx32 offset;

    paused = MapObject_IsBillboardAnimPaused(object);
    flag4 = MapObject_CheckFlag4(object);
    hasFollowMonSprite = MapObject_HasFollowMonSprite(object);
    if (billboardWork->shadowAnchored) {
        MapObject_CopyFacingVector(object, &offset);
        offset.y = billboardWork->height << 12;
    } else {
        if (paused != 0 || flag4 != 0 || hasFollowMonSprite != 0) {
            MapObject_CopyFacingVector(object, &offset);
            offset.x = 0;
            offset.z = 0;
        } else {
            offset.x = 0;
            offset.y = 0;
            offset.z = 0;
        }
        if (addFacingOffset) {
            FollowMonBillboard_AddFacingOffset(object, direction, &offset);
        }
        offset.y += ov01_022054E0(object);
    }
    if (direction != billboardWork->direction) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(direction));
        BillboardObject_SetSequenceFrame(billboard, 0);
        billboardWork->bobbing = 0;
    }
    if (paused == 0 && flag4 == 0) {
        BillboardObject_AdvanceFrame(billboard, FX32_ONE);
        if (FollowMonBillboard_IsBobFrame((u8)direction, object, billboard)) {
            offset.y -= 2 * FX32_ONE;
            billboardWork->bobbing = 1;
        } else {
            billboardWork->bobbing = 0;
        }
    }
    if (flag4) {
        BillboardObject_AdvanceFrame(billboard, FX32_ONE);
    }
    MapObject_SetFacingVector(object, &offset);
}

// Large billboards (billboard kind 10) sit further out to the side and slightly forward / back.
void FollowMonBillboard_AddFacingOffset(LocalMapObject *object, u32 direction, VecFx32 *offset) {
    VecFx32 unused;

    MapObject_CopyFacingVector(object, &unused);
    if (ov01_021FA298(MapObject_GetSpriteID(object)) == 10) {
        switch (direction) {
        case DIR_NORTH:
            offset->z += FX32_ONE;
            break;
        case DIR_SOUTH:
            offset->z -= FX32_ONE;
            break;
        case DIR_WEST:
            offset->x += 10 * FX32_ONE;
            break;
        case DIR_EAST:
            offset->x -= 10 * FX32_ONE;
            break;
        }
    } else {
        switch (direction) {
        case DIR_WEST:
            offset->x += 2 * FX32_ONE;
            break;
        case DIR_EAST:
            offset->x -= 2 * FX32_ONE;
            break;
        }
    }
}

void FollowMonBillboard_SetShadowAnchored(LocalMapObject *object, BOOL anchored) {
    FollowMonBillboardWork *work;
    VecFx32 offset;
    fx32 *p;

    work = (FollowMonBillboardWork *)MapObject_GetDrawData(object);
    if (anchored) {
        p = (fx32 *)&offset;
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
        work->shadowAnchored = TRUE;
        FieldShadow_OffsetByFacing(MapObject_GetFacingDirection(object), &offset);
        work->shadowOffsetX = offset.x / FX32_ONE;
        work->shadowOffsetZ = offset.z / FX32_ONE;
    } else {
        work->shadowAnchored = FALSE;
        work->shadowOffsetX = 0;
        work->shadowOffsetZ = 0;
    }
}

void FollowMonBillboard_SetHeight(LocalMapObject *object, s8 height) {
    ((FollowMonBillboardWork *)MapObject_GetDrawData(object))->height = height;
}

void FollowMonBillboard_AddHeight(LocalMapObject *object, s8 delta) {
    FollowMonBillboardWork *work = (FollowMonBillboardWork *)MapObject_GetDrawData(object);
    work->height = work->height + delta;
}

BOOL FollowMonBillboard_IsShadowFollowing(LocalMapObject *object) {
    FollowMonBillboardWork *work = (FollowMonBillboardWork *)MapObject_GetDrawData(object);
    if (work->shadowAnchored) {
        return FALSE;
    }
    return TRUE;
}

void FollowMonBillboard_GetShadowOffset(LocalMapObject *object, VecFx32 *offset) {
    FollowMonBillboardWork *work = (FollowMonBillboardWork *)MapObject_GetDrawData(object);
    offset->x = work->shadowOffsetX << 12;
    offset->z = work->shadowOffsetZ << 12;
    offset->y = 0;
}

// Each facing has a 20-frame walk sequence; the billboard dips on some of its frames.
BOOL FollowMonBillboard_IsBobFrame(u8 direction, LocalMapObject *object, Sprite *billboard) {
    int frame = BillboardObject_GetFrame(billboard) / FX32_ONE;

    switch (direction) {
    case DIR_NORTH:
        break;
    case DIR_SOUTH:
        frame -= 20;
        break;
    case DIR_WEST:
        frame -= 40;
        break;
    case DIR_EAST:
        frame -= 60;
        break;
    }
    if (ov01_022055B0(object)) {
        if (direction == DIR_SOUTH) {
            if (frame < 5 || frame >= 15) {
                return TRUE;
            }
        } else if (frame < 10) {
            return TRUE;
        }
    } else if ((frame >= 5 && frame < 10) || frame >= 15) {
        return TRUE;
    }
    return FALSE;
}

void FollowMonBillboard_LoadSequence(BOOL alternate, LocalMapObject *object) {
    FollowMonBillboardWork *work = (FollowMonBillboardWork *)MapObject_GetDrawData(object);
    ov01_02205808(alternate, object, work->billboard);
}

void FollowMonBillboard_ResetPalette(LocalMapObject *object) {
    FollowMonBillboardWork *work = (FollowMonBillboardWork *)MapObject_GetDrawData(object);
    work->paletteApplied = FALSE;
}

void LegendaryBirdObject_Init(LocalMapObject *object) {
    VecFx32 offset;

    LegendaryBirdObject_CreateBillboard(object);
    offset.x = 0;
    offset.y = 0;
    offset.z = 0;
    MapObject_SetDrawOffset(object, &offset);
}

void LegendaryBirdObject_Update(LocalMapObject *object) {
    LegendaryBirdObject *work = (LegendaryBirdObject *)MapObject_GetDrawData(object);
    Sprite *billboard = work->billboard;

    if (MapObject_IsBillboardPending(object) == TRUE) {
        return;
    }
    if (billboard == NULL) {
        return;
    }
    sLegendaryBirdAnimateFuncs[MapObject_GetAnimation(object)](object, billboard, work);
    work->sequence = MapObject_GetAnimation(object);
    MapObject_UpdateBillboardPosition(object, billboard);
    LegendaryBirdObject_UpdateVisibility(object, billboard);
}

void LegendaryBirdObject_Delete(LocalMapObject *object) {
    LegendaryBirdObject_DeleteBillboard(object);
}

void LegendaryBirdObject_Pause(LocalMapObject *object) {
    LegendaryBirdObject *work = (LegendaryBirdObject *)MapObject_GetDrawData(object);

    if (work->billboard != NULL) {
        BillboardObject_SaveState(work->billboard, &work->savedState);
    }
    MapObject_DeleteBillboard(object, work);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK21);
}

void LegendaryBirdObject_Resume(LocalMapObject *object) {
    LegendaryBirdObject *work = (LegendaryBirdObject *)MapObject_GetDrawData(object);

    if (MapObject_IsBillboardPending(object) == TRUE) {
        return;
    }
    if (work->billboard == NULL) {
        MapObject_CreateBillboard(object, work);
    }
    if (work->billboard != NULL) {
        BillboardObject_RestoreState(work->billboard, &work->savedState);
        MapObject_UpdateBillboardPosition(object, work->billboard);
        MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK21);
    }
}

void LegendaryBirdObject_CreateBillboard(LocalMapObject *object) {
    LegendaryBirdObject *work = (LegendaryBirdObject *)MapObject_InitDrawData(object, sizeof(LegendaryBirdObject));
    MapObject_CreateBillboard(object, work);
}

void LegendaryBirdObject_AnimateIdle(LocalMapObject *object, Sprite *billboard, LegendaryBirdObject *work) {
    if (work->sequence != 0) {
        BillboardObject_SetSequence(billboard, 0);
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    BillboardObject_AdvanceFrame(billboard, FX32_ONE);
}

void LegendaryBirdObject_AnimateFlap(LocalMapObject *object, Sprite *billboard, LegendaryBirdObject *work) {
    if (work->sequence != 1) {
        BillboardObject_SetSequence(billboard, 1);
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    BillboardObject_AdvanceFrame(billboard, FX32_ONE);
}

void LegendaryBirdObject_UpdateVisibility(LocalMapObject *object, Sprite *billboard) {
    int visible = TRUE;

    if (MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) == TRUE) {
        visible = FALSE;
    }
    if (MapObject_TestFlagsBits(object, MAPOBJECTFLAG_UNK12) == TRUE && MapObject_TestFlagsBits(object, MAPOBJECTFLAG_UNK13) == FALSE) {
        visible = FALSE;
    }
    BillboardObject_SetVisible(billboard, visible);
}

void LegendaryBirdObject_DeleteBillboard(LocalMapObject *object) {
    LegendaryBirdObject *work = (LegendaryBirdObject *)MapObject_GetDrawData(object);
    MapObject_DeleteBillboard(object, work);
}

// Manager flag 1: the render data exists.
void MapObjectManager_InitRender(MapObjectManager *mapObjectManager, int arg1, int objectsToPreload, const int *objectIDs, int arg4) {
    u32 count;
    u32 priority;

    if (MapObjectManager_GetFlagsBitsMask(mapObjectManager, (MapObjectFlagBits)1) != 0) {
        GF_AssertFail();
    }
    FldObjSys_OpenMModelNarc(mapObjectManager);
    count = MapObjectManager_GetObjectCount(mapObjectManager);
    priority = MapObjectManager_GetPriority(mapObjectManager) - 1;
    MapObjectRender_Init(MapObjectManager_GetRender(mapObjectManager), mapObjectManager, count, priority, arg1, objectsToPreload, objectIDs, arg4);
    MapObjectManager_SetFlagsBits(mapObjectManager, (MapObjectFlagBits)1);
}

void MapObjectManager_FreeRender(MapObjectManager *mapObjectManager) {
    if (MapObjectManager_IsRenderInitialized(mapObjectManager) != TRUE) {
        GF_AssertFail();
    }
    MapObjectRender_Free(MapObjectManager_GetRender(mapObjectManager));
    MapObjectManager_ClearFlagsBits(mapObjectManager, (MapObjectFlagBits)1);
    FldObjSys_CloseMModelNarc(mapObjectManager);
}

void FldObjSys_OpenMModelNarc(MapObjectManager *manager) {
    MapObjectManager_SetMapModelNarc(manager, NARC_New(NARC_data_mmodel_mmodel, HEAP_ID_FIELD1));
}

void FldObjSys_CloseMModelNarc(MapObjectManager *manager) {
    NARC_Delete(MapObjectManager_GetMapModelNarc(manager));
}

void MapObject_TryUpdateDraw(LocalMapObject *object) {
    if (MapObjectManager_GetFlagsBitsMask(MapObject_GetManager(object), (MapObjectFlagBits)4) != 0) {
        return;
    }
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK14) == 0) {
        return;
    }
    if (MapObject_CheckMovementPaused(object) == FALSE || MapObject_CheckFlag4(object) != FALSE) {
        MapObject_CallDrawUpdate(object);
    }
}

ObjectEventGraphicsInfo *ObjectEvent_GetGraphicsInfo(u32 spriteId) {
    ObjectEventGraphicsInfo *info = ov01_022074A8;

    do {
        if (info->spriteId == spriteId) {
            return info;
        }
        info++;
    } while (info->spriteId != 0xFFFF);
    GF_AssertFail();
    return NULL;
}

int GetMoveModelNoBySpriteId(u32 spriteId) {
    ObjectEventGraphicsInfo *info = ObjectEvent_GetGraphicsInfo(spriteId);
    if (info == NULL) {
        return -1;
    }
    return info->mapModelId;
}

const u16 *MapObject_GetEffectFlags(LocalMapObject *object) {
    return SpriteId_GetEffectFlags(MapObject_GetSpriteID(object));
}

const u16 *SpriteId_GetEffectFlags(u32 spriteId) {
    ObjectEventGraphicsInfo *info = ObjectEvent_GetGraphicsInfo(spriteId);
    if (info == NULL) {
        return NULL;
    }
    return &ov01_02206D00[info->unk4_0];
}

BOOL MapObject_IsBillboardAnimPaused(LocalMapObject *object) {
    if (MapObject_CheckMovementPaused(object) == TRUE && MapObject_CheckFlag4(object) == FALSE) {
        return TRUE;
    }
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK8) != 0) {
        return TRUE;
    }
    return FALSE;
}

void *ReadMModelFromNarcInternal(MapObjectManager *manager, u32 memberIdx, BOOL atStart) {
    void *dest;
    NARC *narc = MapObjectManager_GetMapModelNarc(manager);
    u32 size = NARC_GetMemberSize(narc, memberIdx);

    if (atStart == TRUE) {
        dest = Heap_Alloc(HEAP_ID_FIELD1, size);
    } else {
        dest = Heap_AllocAtEnd(HEAP_ID_FIELD1, size);
    }
    NARC_ReadWholeMember(narc, memberIdx, dest);
    return dest;
}

void MapObject_GetDrawPosition(LocalMapObject *object, VecFx32 *dest) {
    VecFx32 position;
    VecFx32 facing;
    VecFx32 offset2;
    VecFx32 offset3;

    MapObject_CopyPositionVector(object, &position);
    MapObject_CopyFacingVector(object, &facing);
    MapObject_CopyDrawOffset(object, &offset2);
    sub_0205F9B0(object, &offset3);
    dest->x = position.x + facing.x + offset2.x + offset3.x;
    dest->y = position.y + facing.y + offset2.y + offset3.y;
    dest->z = position.z + facing.z + offset2.z + offset3.z;
}

void MapObject_SetFacingAndUpdateDraw(LocalMapObject *object, u32 direction) {
    MapObject_SetFacingDirection(object, direction);
    if (MapObject_CheckFlag14(object) == TRUE) {
        MapObject_CallDrawUpdate(object);
    }
}

void CameraFocusObject_Init(LocalMapObject *object) {
    MapObject_SetVisible(object, TRUE);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK20);
}

void CameraFocusObject_Update(LocalMapObject *object) {
}

void CameraFocusObject_Delete(LocalMapObject *object) {
}

void CameraFocusObject_Pause(LocalMapObject *object) {
}

void CameraFocusObject_Resume(LocalMapObject *object) {
}
