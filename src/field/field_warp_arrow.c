#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"

#include "field_effects.h"
#include "field_system.h"
#include "geometry_math.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "player_avatar.h"
#include "render_animation.h"
#include "render_model.h"
#include "map_object_movement_utilities.h"

// The arrow shown on the tile ahead of the player while it stands on a warp entrance (door mat, cave exit,
// ...) facing the way out (field effect renderer 3, created with the player avatar): a model turned to the
// exit direction that steps 2 units towards it and back every 16 frames.

#define WARP_ARROW_FRAME_TIME 16

// Renderer data, from FieldEffectManager_GetRendererData(manager, 3).
typedef struct WarpArrow {
    FieldEffectManager *manager;
    UnkOv01_021FFECC_sub model;
    NNSG3dRenderObj renderObj;
} WarpArrow;

typedef struct WarpArrowEffectArgs {
    FieldEffectManager *manager;
    WarpArrow *arrow;
    PlayerAvatar *playerAvatar;
} WarpArrowEffectArgs;

typedef struct WarpArrowEffect {
    BOOL hidden;
    int direction; // of the warp the player stands on, -1 if none
    u8 behavior;
    u8 unk09[0xB];
    u32 objectId;
    u32 mapId;
    int timer;
    int frame;
    WarpArrowEffectArgs args;
} WarpArrowEffect;

WarpArrow *WarpArrow_New(FieldEffectManager *manager);
void WarpArrow_Free(WarpArrow *arrow);
void WarpArrow_LoadModel(WarpArrow *arrow);
void WarpArrow_FreeModel(WarpArrow *arrow);
void WarpArrow_Spawn(PlayerAvatar *playerAvatar);
BOOL WarpArrowEffect_Init(FieldEffect *effect, WarpArrowEffect *work);
void WarpArrowEffect_Destroy(FieldEffect *effect, WarpArrowEffect *work);
void WarpArrowEffect_Update(FieldEffect *effect, WarpArrowEffect *work);
void WarpArrowEffect_Draw(FieldEffect *effect, WarpArrowEffect *work);
int WarpArrow_GetWarpDirection(u8 behavior);

// Data stays in asm (asm/field_warp_arrow_data.s, linked after this file): the model scale (1, 1, 1),
// the effect template and the arrow's two offsets per direction (none, 2 units towards the exit).
extern const VecFx32 sWarpArrowScale;
extern const FieldEffectBasicTemplate sWarpArrowTemplate;
extern const VecFx32 sWarpArrowOffsets[4][2];

WarpArrow *WarpArrow_New(FieldEffectManager *manager) {
    WarpArrow *arrow = FieldEffectManager_HeapAllocAndFill(manager, sizeof(WarpArrow), FALSE, 0);
    arrow->manager = manager;
    WarpArrow_LoadModel(arrow);
    return arrow;
}

void WarpArrow_Free(WarpArrow *arrow) {
    WarpArrow_FreeModel(arrow);
    FieldEffectManager_HeapFree(arrow);
}

void WarpArrow_LoadModel(WarpArrow *arrow) {
    FieldEffectManager_LoadRenderModel(arrow->manager, &arrow->model, 0, 0x55, FALSE);
    RenderObj_InitFromModel(&arrow->renderObj, &arrow->model);
}

void WarpArrow_FreeModel(WarpArrow *arrow) {
    RenderModel_Free(&arrow->model);
}

void WarpArrow_Spawn(PlayerAvatar *playerAvatar) {
    WarpArrowEffectArgs args;
    VecFx32 position = { 0, 0, 0 };

    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(playerAvatar);
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);
    args.manager = manager;
    args.arrow = FieldEffectManager_GetRendererData(manager, 3);
    args.playerAvatar = playerAvatar;
    FieldEffectManager_CreateEffect(manager, &sWarpArrowTemplate, &position, 0, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
}

BOOL WarpArrowEffect_Init(FieldEffect *effect, WarpArrowEffect *work) {
    WarpArrowEffectArgs *args = (WarpArrowEffectArgs *)FieldEffect_GetUserData(effect);
    work->args = *args;
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(args->playerAvatar);
    work->direction = -1;
    work->objectId = MapObject_GetID(mapObject);
    work->mapId = MapObject_GetMapID(mapObject);
    return TRUE;
}

void WarpArrowEffect_Destroy(FieldEffect *effect, WarpArrowEffect *work) {
}

void WarpArrowEffect_Update(FieldEffect *effect, WarpArrowEffect *work) {
    VecFx32 objectPos;
    VecFx32 position;

    PlayerAvatar *playerAvatar = work->args.playerAvatar;
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(playerAvatar);
    if (!sub_0205F0A8(mapObject, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    int prevDirection = work->direction;
    work->behavior = sub_0205F504(mapObject);
    work->direction = WarpArrow_GetWarpDirection(work->behavior);
    work->hidden = FALSE;
    if (work->direction == -1 || work->direction != (int)PlayerAvatar_GetFacingDirection(playerAvatar)) {
        work->timer = 0;
        work->frame = 0;
        work->hidden = TRUE;
        return;
    }
    if (prevDirection != work->direction) {
        work->timer = 0;
        work->frame = 0;
    }
    int x = MapObject_GetXCoord(mapObject);
    int dx = GetDeltaXByFacingDirection(work->direction);
    int z = MapObject_GetZCoord(mapObject);
    int dz = GetDeltaYByFacingDirection(work->direction);
    SetPositionToTileCenter(x + dx, z + dz, &position);
    MapObject_CopyPositionVector(mapObject, &objectPos);
    position.y = objectPos.y;
    FieldEffect_SetPosition(effect, &position);
    work->timer++;
    if (work->timer > WARP_ARROW_FRAME_TIME - 1) {
        work->timer = 0;
        work->frame = (work->frame + 1) % 2;
    }
}

#pragma force_active on

void WarpArrowEffect_Draw(FieldEffect *effect, WarpArrowEffect *work) {
    MtxFx33 rotation;
    VecFx32 position;
    VecFx32 scale;
    const VecFx32 *offset;
    NNSG3dRenderObj *renderObj;
    int angle;

    if (work->hidden == TRUE || work->direction == -1) {
        return;
    }
    angle = 0;
    scale = sWarpArrowScale;
    renderObj = &work->args.arrow->renderObj;
    offset = &sWarpArrowOffsets[work->direction][work->frame];
    switch (work->direction) {
    case 0:
        angle = 180;
        break;
    case 1:
        break;
    case 2:
        angle = 270;
        break;
    case 3:
        angle = 90;
        break;
    }
    Math_CalcRotationMatrixXYZDeg(&rotation, 0, angle, 0);
    FieldEffect_GetPosition(effect, &position);
    position.x += offset->x;
    position.y += offset->y;
    position.z += offset->z;
    RenderObj_Draw(renderObj, &position, &scale, &rotation);
}

#pragma force_active reset

// The way out of a warp entrance tile (DIR_NORTH..DIR_EAST), -1 for other tiles.
int WarpArrow_GetWarpDirection(u8 behavior) {
    if (MetatileBehavior_IsWarpEntranceNorth(behavior)) {
        return 0;
    }
    if (MetatileBehavior_IsWarpEntranceSouth(behavior)) {
        return 1;
    }
    if (MetatileBehavior_IsWarpEntranceWest(behavior)) {
        return 2;
    }
    if (MetatileBehavior_IsWarpEntranceEast(behavior)) {
        return 3;
    }
    return -1;
}
