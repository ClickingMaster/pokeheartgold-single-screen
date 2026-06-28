#include "overlay_01.h"
#include "overlay_01_021E6880.h"
#include "overlay_2/event_cutscene.h"
#include "overlay_2/overlay_02_gear_phone.h"

#include "follow_mon.h"
#include "map_events.h"
#include "map_object.h"
#include "menu_input_state.h"
#include "player_avatar.h"
#include "script_pokemon_util.h"
#include "start_menu.h"
#include "sys_flags.h"
#include "unk_02005D10.h"
#include "unk_0205CB48.h"

#include "constants/movements.h"
#include "constants/moves.h"
#include "constants/std_script.h"

#include "global.h"

static void ov01_021E6880(FieldInput *fieldInput) {
    fieldInput->unk0_0 = 0;
    fieldInput->unk0_1 = 0;
    fieldInput->unk0_2 = 0;
    fieldInput->unk0_3 = 0;
    fieldInput->unk0_4 = 0;
    fieldInput->unk0_5 = 0;
    fieldInput->unk0_6 = 0;
    
    fieldInput->unk0_7 = 0;
    fieldInput->unk0_8 = 0;
    fieldInput->unk0_9 = 0;
    fieldInput->unk0_A = 0;
    fieldInput->unk0_B = 0;
    fieldInput->unk0_C = 0;
    
    fieldInput->unk4 = -1;
    fieldInput->unk5 = -1;
}

BOOL ov01_021E690C(FieldSystem *fieldSystem) {
    return ov01_021F6B10(fieldSystem) == TRUE;
}

void ov01_021F6B64_thunk(FieldSystem *fieldSystem, int arg1) { // ov01_021E6920
    ov01_021F6B64(fieldSystem, arg1);
}

void FieldInput_Update(FieldInput *fieldInput, FieldSystem *fieldSystem, u16 newKeys, u16 heldKeys) {
    ov01_021E6880(fieldInput);
    if (PlayerAvatar_CheckRunningShoesLock(FieldSystem_GetPlayerAvatar(fieldSystem))) {
        heldKeys |= PAD_BUTTON_B;
    }
    u32 avatarUnk14 = PlayerAvatar_GetUnk14(fieldSystem->playerAvatar);
    u32 avatarUnk10 = PlayerAvatar_GetUnk10(fieldSystem->playerAvatar);
    u32 facingDirection = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    fieldInput->newKeys = newKeys;
    fieldInput->heldKeys = heldKeys;
    BOOL shouldDrawBagIcon = FieldSystem_ShouldDrawStartMenuIcon(fieldSystem, START_MENU_ICON_BAG);
    if (avatarUnk14 == 3 || avatarUnk14 == 0) {
        if (shouldDrawBagIcon && (newKeys & PAD_BUTTON_Y) || fieldSystem->lastTouchMenuInput == 9) {
            if (ov01_021E690C(fieldSystem) == TRUE) {
                fieldInput->unk0_3 = 1;
                fieldSystem->lastTouchMenuInput = 0;
            }
        } else if (fieldSystem->lastTouchMenuInput == 10) {
            if (ov01_021E690C(fieldSystem) == TRUE) {
                fieldInput->unk0_3 = 2;
                fieldSystem->lastTouchMenuInput = 0;
            }
        } else if (fieldSystem->lastTouchMenuInput == 11) {
            fieldInput->unk0_8 = TRUE;
            fieldSystem->lastTouchMenuInput = 0;
        } else if (newKeys & PAD_BUTTON_X || fieldSystem->lastTouchMenuInput != 0) {
            if (ov01_021F6B00(fieldSystem) == 4) {
                MenuInputStateMgr_SetState(&fieldSystem->menuInputState, MENU_INPUT_STATE_BUTTONS);
                fieldInput->unk0_8 = TRUE;
                fieldInput->unk0_2 = TRUE;
            } else if (shouldDrawBagIcon) {
                fieldInput->unk0_2 = TRUE;
                if (fieldSystem->lastTouchMenuInput == 1) {
                    fieldSystem->lastTouchMenuInput = 0;
                }
            }
        } else if (newKeys & 1) {
            fieldInput->unk0_0 = TRUE;
        }
        if (PAD_PLUS_KEY_MASK & heldKeys) {
            fieldInput->unk0_4 = TRUE;
            fieldInput->unk0_5 = TRUE;
        }
        fieldInput->unk0_7 = TRUE;
    } else {
        fieldSystem->lastTouchMenuInput = 0;
    }
    
    if (avatarUnk14 == 3 && avatarUnk10 == 1) {
        fieldInput->unk0_6 = TRUE;
    }
    if (avatarUnk14 == 3) {
        fieldInput->unk0_1 = TRUE;
    }

    if (facingDirection == MOVEMENT_FACE_UP && (PAD_KEY_UP & heldKeys) || facingDirection == MOVEMENT_FACE_DOWN && (PAD_KEY_DOWN & heldKeys) || facingDirection == MOVEMENT_FACE_LEFT && (PAD_KEY_LEFT & heldKeys) || facingDirection == MOVEMENT_FACE_RIGHT && (PAD_KEY_RIGHT & heldKeys)) {
        fieldInput->unk5 = facingDirection;
    } else {
        fieldInput->unk5 = -1;
    }
    
    fieldInput->unk4 = sub_0205DD94(fieldSystem->playerAvatar, newKeys, heldKeys);
}

int FieldInput_Process(FieldInput *fieldInput, FieldSystem *fieldSystem) {
    if (fieldInput->unk0_C == FALSE && TryStartMapScriptByType(fieldSystem, 1) == TRUE) {
        return 1;
    }
    
    if (fieldInput->unk0_C == FALSE) {
        BOOL doublesEligible = HasEnoughAlivePokemonForDoubleBattle(SaveArray_Party_Get(fieldSystem->saveData));
        if (Save_VarsFlags_CheckHaveFollower(Save_VarsFlags_Get(fieldSystem->saveData)) == TRUE) { // Never true in HGSS, but the capability was not removed.
            doublesEligible = TRUE;
        }
        
        if (TryGetSeenByNpcTrainers(fieldSystem, doublesEligible) == TRUE) {
            sub_0205CF44(fieldSystem->playerAvatar);
            MapObjectManager_PauseAllMovement(fieldSystem->mapObjectManager);
            if (FollowMon_IsActive(fieldSystem)) {
                MapObject_UnpauseMovement(FollowMon_GetMapObject(fieldSystem));
            }
            return 1;
        }
    }
    
    if (fieldInput->unk0_6) { // fieldInput->movement?
        ClearFlag965(Save_VarsFlags_Get(fieldSystem->saveData)); // VarsFlags_ClearStep?
        if (ov01_021E7628(fieldSystem) == TRUE) { // FieldSystem_ProcessStep?
            return 1;
        }
    }
    
    if (BugContestTimeoutCheck(fieldSystem)) {
        return 1;
    }
    
    int playerEvent = 0;
    int direction = sub_0205DD94(fieldSystem->playerAvatar, fieldInput->newKeys, fieldInput->heldKeys);
    
    if (StrengthFlagAction(Save_VarsFlags_Get(fieldSystem->saveData), 2)) {
        playerEvent |= 1; // PLAYER_EVENT_USED_STRENGTH?
    }
    
    if (GetIdxOfFirstPartyMonWithMove(SaveArray_Party_Get(fieldSystem->saveData), MOVE_WATERFALL) != 0xFF) {
        playerEvent |= 2; // PLAYER_EVENT_USED_WATERFALL?
    }
    
    if (ov01_021F1D94(fieldSystem, fieldSystem->playerAvatar, direction, playerEvent) == TRUE) {
        return 1;
    }
    
    if (fieldInput->unk0_1) { // fieldInput->endMovement?
        if (ov01_021E7114(fieldSystem)) { // FieldSystem_CheckWildEncounter?
            return 1;
        }
        if (ov01_021E6DC4(fieldSystem) == TRUE) { // FieldSystem_CheckSign?
            return 1;
        }
    }

    if (fieldInput->unk0_4 // fieldInput->sign?
        && fieldInput->unk4 == PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar) // fieldInput->playerDir?
        && ov01_021E6DC4(fieldSystem) == TRUE) {
        return 1;
    }
    
    if (fieldInput->unk0_0) { // fieldInput->interact?
        LocalMapObject* mapObject;
        if (sub_0203DC64(fieldSystem, &mapObject) == TRUE) { // IsInteractionValid?
            if (sub_0205CF60(fieldSystem->playerAvatar) == TRUE) {
                sub_0205CFBC(fieldSystem->playerAvatar, PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar));
            }
            if (MapObject_GetType(mapObject) != 9) {
                StartMapSceneScript(fieldSystem, MapObject_GetScriptID(mapObject), mapObject);
            } else {
                StartMapSceneScript(fieldSystem, 0, mapObject);
            }
            fieldSystem->lastTouchMenuInput = 0;
            return 1;
        }
        u32 bgEventScript = GetInteractedBackgroundEventScript(fieldSystem, Field_GetBgEvents(fieldSystem), Field_GetNumBgEvents(fieldSystem));
        if (bgEventScript != 0xFFFF) {
            StartMapSceneScript(fieldSystem, bgEventScript, 0);
            fieldSystem->lastTouchMenuInput = 0;
            return 1;
        }
        u32 metatileScript = GetInteractedMetatileScript(fieldSystem, ov01_021E7B54(fieldSystem));
        if (metatileScript != 0xFFFF) {
            StartMapSceneScript(fieldSystem, metatileScript, 0);
            fieldSystem->lastTouchMenuInput = 0;
            return 1;
        }
        u32 unkScript = ov01_021E7F38(fieldSystem);
        if (unkScript != 0xFFFF) {
            StartMapSceneScript(fieldSystem, unkScript, 0);
            fieldSystem->lastTouchMenuInput = 0;
            return 1;
        }
    }
    
    if (fieldInput->unk0_5 && ov01_021E7198(fieldSystem, fieldInput) == TRUE) {
        ov01_021E7C70(fieldSystem);
        return 1;
    }
    
    if (fieldInput->unk0_3) {
        u32 registeredItemResult = UseRegisteredItemButtonInField(fieldSystem, fieldInput->unk0_3);
        if (registeredItemResult) {
            ov01_021F6B64_thunk(fieldSystem, fieldInput->unk0_3);
            return registeredItemResult;
        }
    }
    
    if (fieldInput->unk0_2 && FieldSystem_MapIsNotMysteryZone(fieldSystem) == TRUE) {
        PlaySE(SEQ_SE_DP_WIN_OPEN);
        StartMenu_Init(fieldSystem);
        return 2;
    }
    
    if (ov02_02252334(fieldSystem)) {
        StartMapSceneScript(fieldSystem, std_revert_shaymin, 0);
        return 1;
    }

    if (fieldInput->unk0_8) {
        ov01_021F6A9C(fieldSystem, 0, 0);
        return 2;
    }
    
    if (fieldInput->unk0_7 && ov02_02251F20(FieldSystem_GetGearPhoneRingManager(fieldSystem))) {
        return 2;
    }
    return 0;
}
