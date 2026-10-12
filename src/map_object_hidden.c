#include "global.h"

#include "field/field_effect_manager.h"

#include "map_object.h"
#include "map_object_follow_internal.h"

void HiddenObjectMovement_Init(LocalMapObject *object, int coverType) {
    HiddenObjectMovementState *state = (HiddenObjectMovementState *)MapObject_InitMovementData(object, sizeof(HiddenObjectMovementState));
    state->coverType = coverType;
    MapObject_SetAnimation(object, 0);
    MapObject_ClearSingleMovement(object);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK20);
    VecFx32 offset = gHiddenObjectOffset;
    MapObject_SetFacingVector(object, &offset);
}

void sub_0206630C(LocalMapObject *object) {
    HiddenObjectMovement_Init(object, 0);
}

void sub_02066318(LocalMapObject *object) {
    HiddenObjectMovement_Init(object, 1);
}

void sub_02066324(LocalMapObject *object) {
    HiddenObjectMovement_Init(object, 2);
}

void sub_02066330(LocalMapObject *object) {
    HiddenObjectMovement_Init(object, 3);
}

void HiddenObjectMovement_Update(LocalMapObject *object) {
    HiddenObjectMovementState *state = (HiddenObjectMovementState *)MapObject_GetMovementData(object);

    while (gHiddenObjectStates[state->state](object, state) == TRUE) {
    }
}

void HiddenObjectMovement_Delete(LocalMapObject *object) {
    int effect = HiddenObjectMovement_GetCoverEffect(object);
    if (effect != 0) {
        FieldEffectManager_DestroyEffect(effect);
    }
}

void HiddenObjectMovement_Resume(LocalMapObject *object) {
    HiddenObjectMovementState *state = (HiddenObjectMovementState *)MapObject_GetMovementData(object);

    state->state = 0;
    HiddenObjectMovement_SetCoverEffect(object, 0);
    if (state->revealed == FALSE) {
        VecFx32 offset = gHiddenObjectResumeOffset;
        MapObject_SetFacingVector(object, &offset);
        MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK20);
    }
}

BOOL HiddenObjectState_CreateCover(LocalMapObject *object, HiddenObjectMovementState *state) {
    if (state->revealed == FALSE) {
        HiddenObjectMovement_SetCoverEffect(object, ov01_021FFF5C(object, state->coverType));
    }
    MapObject_ClearSingleMovement(object);
    MapObject_ClearEndMovement(object);
    state->state++;
    return FALSE;
}

BOOL HiddenObjectState_Wait(LocalMapObject *object, HiddenObjectMovementState *state) {
    if (state->revealed == FALSE) {
        if (HiddenObjectMovement_GetCoverEffect(object) == 0 && MapObject_IsDrawInitialized(object) == TRUE) {
            HiddenObjectMovement_SetCoverEffect(object, ov01_021FFF5C(object, state->coverType));
        }
        MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK20);
    }
    return FALSE;
}

int HiddenObjectMovement_SetCoverEffect(LocalMapObject *object, int effect) {
    HiddenObjectMovementState *state = (HiddenObjectMovementState *)MapObject_GetMovementData(object);
    state->coverEffect = effect;
    return (int)state;
}

int HiddenObjectMovement_GetCoverEffect(LocalMapObject *object) {
    return ((HiddenObjectMovementState *)MapObject_GetMovementData(object))->coverEffect;
}

void HiddenObjectMovement_SetRevealed(LocalMapObject *object) {
    ((HiddenObjectMovementState *)MapObject_GetMovementData(object))->revealed = TRUE;
}
