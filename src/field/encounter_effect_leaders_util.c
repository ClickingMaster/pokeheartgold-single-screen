#include "global.h"

#include "constants/trainer_class.h"
#include "constants/trainers.h"

#include "field/encounter_effect_leaders.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0189.h"

#include "message_format.h"
#include "msgdata.h"
#include "sprite.h"

const GymLeaderIntroParams gEncounterRivalIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_RIVAL_SILVER_6,
    .trainerClass = TRAINERCLASS_RIVAL,
    .unk_0A = 1,
    .spriteMembers = { 127, 128, 129, 130 },
    .bgMembers = { 45, 44, 46 },
};

void ScaleSprites_Init(ScaleSprites *scaleSprites, EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *resources, fx32 x, fx32 y, enum HeapID heapID) {
    int i;

    scaleSprites->timer = 0;
    scaleSprites->count = 0;
    for (i = 0; i < SCALE_SPRITE_COUNT; i++) {
        scaleSprites->sprites[i] = EncounterEffect_CreateSprite(spriteManager, resources, x, y, 0, 0);
        Sprite_SetDrawFlag(scaleSprites->sprites[i], FALSE);
        Sprite_SetAnimActiveFlag(scaleSprites->sprites[i], TRUE);
        if (i != SCALE_SPRITE_COUNT - 1) {
            Sprite_SetAffineOverwriteMode(scaleSprites->sprites[i], NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
            Sprite_SetAnimCtrlSeq(scaleSprites->sprites[i], 1);
            EncounterEffectInterp_InitFx(&scaleSprites->scale[i], FX32_CONST(2), FX32_CONST(1), 6);
        } else {
            EncounterEffectInterp_InitFx(&scaleSprites->scale[i], FX32_CONST(1), FX32_CONST(1), 6);
        }
    }
}

void ScaleSprites_Delete(ScaleSprites *scaleSprites) {
    int i;

    for (i = 0; i < SCALE_SPRITE_COUNT; i++) {
        Sprite_Delete(scaleSprites->sprites[i]);
    }
}

BOOL ScaleSprites_Update(ScaleSprites *scaleSprites) {
    BOOL done = TRUE;
    VecFx32 scale;
    int i;
    BOOL finished;

    if (scaleSprites->count < SCALE_SPRITE_COUNT) {
        done = FALSE;
        scaleSprites->timer--;
        if (scaleSprites->timer <= 0) {
            scaleSprites->timer = 3;
            scaleSprites->count++;
        }
    }
    for (i = 0; i < scaleSprites->count; i++) {
        finished = EncounterEffectInterp_UpdateFx(&scaleSprites->scale[i]);
        EncounterEffect_SetVecFx32(&scale, scaleSprites->scale[i].value, scaleSprites->scale[i].value, scaleSprites->scale[i].value);
        Sprite_SetAffineScale(scaleSprites->sprites[i], &scale);
        Sprite_SetDrawFlag(scaleSprites->sprites[i], TRUE);
        if (finished == FALSE) {
            done = FALSE;
        } else if (i != SCALE_SPRITE_COUNT - 1) {
            Sprite_SetDrawFlag(scaleSprites->sprites[i], FALSE);
        }
    }
    return done;
}

String *LeadersEffect_GetTrainerNameString(u32 trainerId, enum HeapID heapID) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0189_bin, heapID);
    MessageFormat *msgFormat = MessageFormat_New(heapID);
    String *string = String_New(128, heapID);
    String *template = String_New(128, heapID);

    ReadMsgDataIntoString(msgData, msg_0189_00000, template);
    BufferTrainerName(msgFormat, 0, trainerId);
    StringExpandPlaceholders(msgFormat, string, template);
    DestroyMsgData(msgData);
    MessageFormat_Delete(msgFormat);
    String_Delete(template);
    return string;
}

String *LeadersEffect_GetRivalNameString(SaveData *saveData, enum HeapID heapID) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0189_bin, heapID);
    MessageFormat *msgFormat = MessageFormat_New(heapID);
    String *string = String_New(128, heapID);
    String *template = String_New(128, heapID);

    ReadMsgDataIntoString(msgData, msg_0189_00000, template);
    BufferRivalsName(msgFormat, 0, saveData);
    StringExpandPlaceholders(msgFormat, string, template);
    DestroyMsgData(msgData);
    MessageFormat_Delete(msgFormat);
    String_Delete(template);
    return string;
}
