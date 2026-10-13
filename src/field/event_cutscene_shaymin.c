#include "constants/sprites.h"

#include "overlay_2/event_cutscene.h"

#include "follow_mon.h"
#include "math_util.h"
#include "field/field_motion_blur.h"
#include "field/map_object_render.h"
#include "field/field_follow_mon.h"
#include "field/field_shaymin_form_change.h"
#include "screen_fade.h"
#include "script_manager.h"
#include "script_pokemon_util.h"
#include "task.h"
#include "map_object_movement_commands.h"
#include "field_effects.h"

typedef struct ShayminRevertEnv {
    int timer;
    FieldEffect *formChange;
    u32 followMonVisible;
} ShayminRevertEnv;

static BOOL Task_RevertShayminToLandForm(TaskManager *taskMan);

BOOL FieldSystem_FollowMonShayminNeedsRevert(FieldSystem *fieldSystem) {
    if (!MapObject_AreBitsSetForMovementScriptInit(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar))) {
        return FALSE;
    }
    if (FollowMon_IsActive(fieldSystem) && fieldSystem->followMon.species == SPECIES_SHAYMIN) {
        Pokemon *mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
        u32 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        u32 form = GetMonData(mon, MON_DATA_FORM, NULL);
        GF_ASSERT(species == fieldSystem->followMon.species);
        // yes, this is explicitly checked twice
        if (species != fieldSystem->followMon.species) {
            return FALSE;
        }
        if (fieldSystem->followMon.form == SHAYMIN_SKY && form == SHAYMIN_LAND) {
            return TRUE;
        }
    }
    return FALSE;
}

void StartShayminRevertToLandForm(TaskManager *taskMan) {
    ShayminRevertEnv *data = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(ShayminRevertEnv));
    TaskManager_Call(taskMan, Task_RevertShayminToLandForm, data);
}

static BOOL Task_RevertShayminToLandForm(TaskManager *taskMan) {
    int *state = TaskManager_GetStatePtr(taskMan);
    VecFx32 pos;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    ShayminRevertEnv *env = TaskManager_GetEnvironment(taskMan);
    switch (*state) {
    case 0:
        MapObject_UnpauseMovement(FollowMon_GetMapObject(fieldSystem));
        TaskManager_Call(fieldSystem->taskman, Task_WaitFollowMonMovement, NULL);
        (*state)++;
        break;
    case 1:
        MapObject_CopyPositionVector(FollowMon_GetMapObject(fieldSystem), &pos);
        u32 res;
        if (FollowMon_IsVisible(fieldSystem)) {
            env->formChange = ShayminFormChange_Spawn(fieldSystem->fieldEffectManager, &pos);
            res = 1;
        } else {
            env->formChange = NULL;
            res = 0;
        }
        env->followMonVisible = res;
        env->timer = 0;
        FollowMon_SetObjectParams(fieldSystem->followMon.mapObject, SPECIES_SHAYMIN, 0, fieldSystem->followMon.shiny);
        (*state)++;
        break;
    case 2:
        if (env->followMonVisible != 0) {
            if (++env->timer >= 4) {
                MapObject_ChangeBillboardSprite(fieldSystem->followMon.mapObject, SPRITE_FOLLOWER_MON_SHAYMIN);
                (*state)++;
            }
            break;
        }
        MapObjectManager *mapObjectMan = MapObject_GetManager(fieldSystem->followMon.mapObject);
        u32 spriteId = MapObject_GetSpriteID(fieldSystem->followMon.mapObject);
        MapObject_ClearSprite(fieldSystem->followMon.mapObject);
        MapObjectRender_ReleaseSpriteResources(mapObjectMan, spriteId, fieldSystem->followMon.mapObject);
        MapObject_SetSpriteAndInitDraw(fieldSystem->followMon.mapObject, SPRITE_FOLLOWER_MON_SHAYMIN);
        (*state)++;
        break;
    case 3:
        if (MapObject_IsBillboardPending(fieldSystem->followMon.mapObject) == 0) {
            fieldSystem->followMon.form = 0;
            (*state)++;
        }
        break;
    case 4:
        u32 var = 0;
        if (env->formChange == NULL) {
            var = 1;
        } else if (ShayminFormChange_IsDone(env->formChange)) {
            FieldEffect_Destroy((int)env->formChange);
            var = 1;
        }
        if (var != 0) {
            Heap_Free(env);
            MapObject_PauseMovement(FollowMon_GetMapObject(fieldSystem));
            return TRUE;
        }
        break;
    }
    return FALSE;
}
