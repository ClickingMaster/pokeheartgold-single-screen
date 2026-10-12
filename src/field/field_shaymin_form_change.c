#include "global.h"

#include "constants/sndseq.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"
#include "field/field_shaymin_form_change.h"
#include "field/overlay_01_021FB878.h"

#include "field_effects.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "sound_playback.h"

// The burst played on the following Shaymin when it changes back to its Land Forme (field effect renderer
// 22, used by the Shaymin event cutscene): one 3D model with two animations, played once at the
// follower's position. The model stays loaded while the renderer exists; an effect only shows it.

#define SHAYMIN_FORM_CHANGE_ANIMATION_COUNT 2

// Renderer data, from FieldEffectManager_GetRendererData(manager, 22).
typedef struct ShayminFormChange {
    FieldEffectManager *manager;
    NNSFndAllocator allocator;
    Field3dModel model;
    Field3DModelAnimation animations[SHAYMIN_FORM_CHANGE_ANIMATION_COUNT];
    Field3dObject object;
} ShayminFormChange;

typedef struct ShayminFormChangeEffectArgs {
    FieldEffectManager *manager;
    ShayminFormChange *formChange;
} ShayminFormChangeEffectArgs;

typedef struct ShayminFormChangeEffect {
    BOOL done;
    ShayminFormChange *formChange;
} ShayminFormChangeEffect;

ShayminFormChange *ShayminFormChange_New(FieldEffectManager *manager);
void ShayminFormChange_Free(ShayminFormChange *formChange);
void ShayminFormChange_LoadModel(ShayminFormChange *formChange);
void ShayminFormChange_FreeModel(ShayminFormChange *formChange);
BOOL ShayminFormChangeEffect_Init(FieldEffect *effect, ShayminFormChangeEffect *work);
void ShayminFormChangeEffect_Update(void);
void ShayminFormChangeEffect_Destroy(FieldEffect *effect, ShayminFormChangeEffect *work);
void ShayminFormChangeEffect_Draw(FieldEffect *effect, ShayminFormChangeEffect *work);
BOOL ShayminFormChange_AdvanceAnimations(Field3DModelAnimation *animations, u32 count);
void ShayminFormChange_SetAnimationsFrame(Field3DModelAnimation *animations, u32 count, fx32 frame);

// The effect template stays in asm (asm/overlay_01_022051EC_residual_1.s, linked after this file).
extern const FieldEffectBasicTemplate ov01_022096CC;

ShayminFormChange *ShayminFormChange_New(FieldEffectManager *manager) {
    ShayminFormChange *formChange = FieldEffectManager_HeapAllocAndFill(manager, sizeof(ShayminFormChange), FALSE, 0);
    formChange->manager = manager;
    ShayminFormChange_LoadModel(formChange);
    return formChange;
}

void ShayminFormChange_Free(ShayminFormChange *formChange) {
    ShayminFormChange_FreeModel(formChange);
    FieldEffectManager_HeapFree(formChange);
}

void ShayminFormChange_LoadModel(ShayminFormChange *formChange) {
    HeapExp_FndInitAllocator(&formChange->allocator, HEAP_ID_FIELD1, 32);
    Field3dModel_LoadFromFilesystem(&formChange->model, NARC_a_1_0_3, 0x84, HEAP_ID_FIELD1);
    Field3dModelAnimation_LoadFromFilesystem(&formChange->animations[0], &formChange->model, NARC_a_1_0_3, 0xA8, HEAP_ID_FIELD1, &formChange->allocator);
    Field3dModelAnimation_LoadFromFilesystem(&formChange->animations[1], &formChange->model, NARC_a_1_0_3, 0xA6, HEAP_ID_FIELD1, &formChange->allocator);
    Field3dObject_InitFromModel(&formChange->object, &formChange->model);
    Field3dObject_AddAnimation(&formChange->object, &formChange->animations[0]);
    Field3dObject_AddAnimation(&formChange->object, &formChange->animations[1]);
    ShayminFormChange_SetAnimationsFrame(formChange->animations, SHAYMIN_FORM_CHANGE_ANIMATION_COUNT, 0);
    Field3dObject_SetActiveFlag(&formChange->object, FALSE);
}

void ShayminFormChange_FreeModel(ShayminFormChange *formChange) {
    Field3dModelAnimation_Unload(&formChange->animations[1], &formChange->allocator);
    Field3dModelAnimation_Unload(&formChange->animations[0], &formChange->allocator);
    Field3dModel_Unload(&formChange->model);
}

FieldEffect *ShayminFormChange_Spawn(FieldEffectManager *manager, VecFx32 *position) {
    ShayminFormChangeEffectArgs args;
    args.manager = manager;
    args.formChange = FieldEffectManager_GetRendererData(manager, 22);
    return FieldEffectManager_CreateEffect(manager, &ov01_022096CC, position, 0, &args, 0xFF);
}

BOOL ShayminFormChange_IsDone(FieldEffect *effect) {
    ShayminFormChangeEffect *work = FieldEffect_GetState(effect);
    return work->done;
}

BOOL ShayminFormChangeEffect_Init(FieldEffect *effect, ShayminFormChangeEffect *work) {
    ShayminFormChange *formChange;
    VecFx32 position;
    ShayminFormChangeEffectArgs *args = (ShayminFormChangeEffectArgs *)FieldEffect_GetUserData(effect);
    work->formChange = args->formChange;
    work->done = FALSE;
    formChange = args->formChange;
    ShayminFormChange_SetAnimationsFrame(formChange->animations, SHAYMIN_FORM_CHANGE_ANIMATION_COUNT, 0);
    Field3dObject_SetActiveFlag(&formChange->object, TRUE);
    FieldEffect_GetPosition(effect, &position);
    Field3dObject_SetPosEx(&formChange->object, position.x, position.y, position.z);
    PlaySE(SEQ_SE_GS_TUREARUKI);
    return TRUE;
}

void ShayminFormChangeEffect_Update(void) {
}

void ShayminFormChangeEffect_Destroy(FieldEffect *effect, ShayminFormChangeEffect *work) {
    Field3dObject_SetActiveFlag(&work->formChange->object, FALSE);
}

void ShayminFormChangeEffect_Draw(FieldEffect *effect, ShayminFormChangeEffect *work) {
    ShayminFormChange *formChange = work->formChange;
    if (ShayminFormChange_AdvanceAnimations(formChange->animations, SHAYMIN_FORM_CHANGE_ANIMATION_COUNT)) {
        work->done = TRUE;
    }
    Field3dObject_Draw(&formChange->object);
}

BOOL ShayminFormChange_AdvanceAnimations(Field3DModelAnimation *animations, u32 count) {
    u8 i;
    u8 done = 0;
    for (i = 0; i < count; i++) {
        if (Field3dModelAnimation_FrameAdvanceAndCheck(&animations[i], FX32_ONE)) {
            done++;
        }
    }
    return done == count;
}

void ShayminFormChange_SetAnimationsFrame(Field3DModelAnimation *animations, u32 count, fx32 frame) {
    u8 i;
    for (i = 0; i < count; i++) {
        Field3dModelAnimation_FrameSet(&animations[i], frame);
    }
}
