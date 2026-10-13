#include "global.h"

#include "constants/trainer_class.h"

#include "field/encounter_effect.h"
#include "msgdata/msg.naix"

#include "bg_window.h"
#include "brightness.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "palette.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "text.h"
#include "unk_0200FA24.h"
#include "swap_3d_buffers.h"

// Battle intros of the gym leaders and the rival (effects 12-28): the leader's picture slides
// in over a scrolling band with the leader's name. Elite Four and champion (effects 29-33):
// the player and the opponent slide in from both sides and shake.

#define SCALE_SPRITE_COUNT 4

// Sprites that pop in one after another, shrinking from double size.
typedef struct ScaleSprites {
    s16 timer;
    s16 count;
    Sprite *sprites[SCALE_SPRITE_COUNT];
    EncounterEffectInterp scale[SCALE_SPRITE_COUNT];
} ScaleSprites;

typedef struct GymLeaderIntroParams {
    s32 endX;
    u32 trainerId;
    u16 trainerClass;
    u8 padding[2];
    u8 spriteMembers[4]; // character, palette, cell, animation
    u8 bgMembers[3];
} GymLeaderIntroParams;

typedef struct GymLeaderIntroWork {
    EncounterEffectQuadInterp slide;
    EncounterEffectInterp brightness;
    u8 unk_2C[0x14];
    void *band;
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources leaderResources;
    EncounterEffectSpriteResources scaleSpriteResources;
    Sprite *leaderSprite;
    ScaleSprites scaleSprites;
    u8 unk_250[0x2C];
    Window window;
    BOOL scrolling;
    int scroll;
    int timer;
} GymLeaderIntroWork;

typedef struct EliteFourIntroParams {
    u16 opponentMember; // first of the four sprite resource members
    u8 nameplateMember;
    u8 shakeFrames;
    u16 unk_04;
    u16 trainerId;
} EliteFourIntroParams;

typedef struct EliteFourIntroWork {
    EncounterEffectQuadInterp x;
    EncounterEffectQuadInterp y;
    EncounterEffectQuadInterp slide;
    EncounterEffectInterp fade;
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources resources[4];
    Sprite *sprites[4];
    VecFx32 playerPos;
    VecFx32 opponentPos;
    ScaleSprites scaleSprites;
    Window window;
    int playerGender;
    int playerMember;
    u8 unused[8];
} EliteFourIntroWork;

void ScaleSprites_Init(ScaleSprites *scaleSprites, EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *resources, fx32 x, fx32 y, enum HeapID heapID);
void ScaleSprites_Delete(ScaleSprites *scaleSprites);
BOOL ScaleSprites_Update(ScaleSprites *scaleSprites);
String *LeadersEffect_GetTrainerNameString(u32 trainerId, enum HeapID heapID);
String *LeadersEffect_GetRivalNameString(SaveData *saveData, enum HeapID heapID);
BOOL LeadersEffect_RunGymLeaderIntro(EncounterEffectTask *effect, enum HeapID heapID, const GymLeaderIntroParams *params);
u32 LeadersEffect_GetPlayerGender(FieldSystem *fieldSystem);
BOOL LeadersEffect_RunEliteFourIntro(EncounterEffectTask *effect, enum HeapID heapID, const EliteFourIntroParams *params);
void LeadersEffect_LoadBlendedSpritePalette(Sprite *sprite, enum HeapID heapID, s32 member, u8 amount, u16 color);

BOOL ov01_021EFE30(EncounterEffectTask *effect);
void EncounterEffectInterp_InitFx(EncounterEffectInterp *interp, s32 start, s32 end, s32 duration);
BOOL EncounterEffectInterp_UpdateFx(EncounterEffectInterp *interp);
void EncounterEffect_LoadBgGraphics(NARC *narc, int member1, int member2, int member3, int arg4, int arg5, BgConfig *bgConfig, int bgId);
void EncounterEffect_SetVecFx32(VecFx32 *vec, fx32 x, fx32 y, fx32 z);
void *ZigzagWipe_New(void);
void ZigzagWipe_Free(void *band);
void ZigzagWipe_Start(EncounterEffectTask *effect, void *band, int arg2, int arg3, int arg4, int arg5, int arg6);
SysTask *EncounterEffect_ApplyMainBrightness(EncounterEffectInterp *brightness);
void EncounterEffect_StartParticleMode(FieldSystem *fieldSystem);
void EncounterEffect_RequestParticleSetup(void);
u8 EncounterEffect_IsParticleSetupDone(void);
void EncounterEffect_DrawParticles(void);
void EncounterEffect_EndParticleMode(void);
void EncounterEffect_LoadParticles(NARC *narc, int member);
void EncounterEffect_CreateEmitters(u32 arg0);
BOOL EncounterEffect_UpdateParticles(void);
BOOL EncounterEffect_AreParticlesDone(void);
void EncounterEffect_FreeParticles(void);

extern const GymLeaderIntroParams _02260374;
extern const EliteFourIntroParams ov115_02260388;
extern const EliteFourIntroParams ov115_02260390;
extern const EliteFourIntroParams ov115_02260398;
extern const EliteFourIntroParams ov115_022603A0;
extern const EliteFourIntroParams ov115_022603A8;
extern const GymLeaderIntroParams ov115_022603B0;
extern const GymLeaderIntroParams ov115_022603C4;
extern const GymLeaderIntroParams ov115_022603D8;
extern const GymLeaderIntroParams ov115_022603EC;
extern const GymLeaderIntroParams ov115_02260400;
extern const GymLeaderIntroParams ov115_02260414;
extern const GymLeaderIntroParams ov115_02260428;
extern const GymLeaderIntroParams ov115_0226043C;
extern const GymLeaderIntroParams ov115_02260450;
extern const GymLeaderIntroParams ov115_02260464;
extern const GymLeaderIntroParams ov115_02260478;
extern const GymLeaderIntroParams ov115_0226048C;
extern const GymLeaderIntroParams ov115_022604A0;
extern const GymLeaderIntroParams ov115_022604B4;
extern const GymLeaderIntroParams ov115_022604C8;
extern const GymLeaderIntroParams ov115_022604DC;

void ScaleSprites_Init(ScaleSprites *scaleSprites, EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources *resources, fx32 x, fx32 y, enum HeapID heapID) {
    int i;

    scaleSprites->timer = 0;
    scaleSprites->count = 0;
    for (i = 0; i < SCALE_SPRITE_COUNT; i++) {
        scaleSprites->sprites[i] = EncounterEffect_CreateSprite(spriteManager, resources, x, y, 0, 0);
        Sprite_SetDrawFlag(scaleSprites->sprites[i], FALSE);
        Sprite_SetAnimActiveFlag(scaleSprites->sprites[i], TRUE);
        if (i != SCALE_SPRITE_COUNT - 1) {
            Sprite_SetAffineOverwriteMode(scaleSprites->sprites[i], 2);
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

// force_active only sets a section flag; it is kept so the objects stay identical to the matched ones
#pragma force_active on
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
#pragma force_active reset

String *LeadersEffect_GetTrainerNameString(u32 trainerId, enum HeapID heapID) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0189_bin, heapID);
    MessageFormat *msgFormat = MessageFormat_New(heapID);
    String *string = String_New(128, heapID);
    String *template = String_New(128, heapID);

    ReadMsgDataIntoString(msgData, 0, template);
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

    ReadMsgDataIntoString(msgData, 0, template);
    BufferRivalsName(msgFormat, 0, saveData);
    StringExpandPlaceholders(msgFormat, string, template);
    DestroyMsgData(msgData);
    MessageFormat_Delete(msgFormat);
    String_Delete(template);
    return string;
}

#pragma force_active on
BOOL LeadersEffect_RunGymLeaderIntro(EncounterEffectTask *effect, enum HeapID heapID, const GymLeaderIntroParams *params) {
    u32 state = effect->state;
    GymLeaderIntroWork *work = effect->work;
    String *string;
    BOOL done;
    VecFx32 position;

    switch (state) {
    case 0:
        effect->work = Heap_Alloc(heapID, sizeof(GymLeaderIntroWork));
        memset(effect->work, 0, sizeof(GymLeaderIntroWork));
        work = effect->work;
        GfGfxLoader_GXLoadPalFromOpenNarc(effect->narc, 16, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_2_OFFSET, 32, heapID);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        AddWindowParameterized(effect->fieldSystem->bgConfig, &work->window, GF_BG_LYR_MAIN_2, 0, 10, 16, 2, 2, 1);
        FillWindowPixelBuffer(&work->window, 0);
        if (params->trainerClass == TRAINERCLASS_RIVAL) {
            string = LeadersEffect_GetRivalNameString(effect->fieldSystem->saveData, heapID);
        } else {
            string = LeadersEffect_GetTrainerNameString(params->trainerId, heapID);
        }
        AddTextPrinterParameterizedWithColor(&work->window, 0, string, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(string);
        EncounterEffect_InitSprites(&work->spriteManager, 8, 3);
        EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->leaderResources, params->spriteMembers[0], 1, params->spriteMembers[1], params->spriteMembers[2], params->spriteMembers[3], 600000);
        EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->scaleSpriteResources, 59, 1, 60, 61, 62, 600001);
        work->leaderSprite = EncounterEffect_CreateSprite(&work->spriteManager, &work->leaderResources, FX32_CONST(272), FX32_CONST(66), 0, 0);
        Sprite_SetDrawFlag(work->leaderSprite, FALSE);
        ScaleSprites_Init(&work->scaleSprites, &work->spriteManager, &work->scaleSpriteResources, FX32_CONST(72), FX32_CONST(74), heapID);
        LeadersEffect_LoadBlendedSpritePalette(work->leaderSprite, heapID, params->spriteMembers[0], 14, RGB_BLACK);
        work->band = ZigzagWipe_New();
        effect->state++;
        break;
    case 1:
        EncounterEffect_StartFlash(1, 16, 16, &effect->fadeDone, 1);
        effect->state++;
        break;
    case 2:
        if (effect->fadeDone) {
            effect->state = state + 1;
        }
        break;
    case 3:
        EncounterEffect_LoadBgGraphics(effect->narc, params->bgMembers[2], params->bgMembers[1], params->bgMembers[0], 0, 1, effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3);
        work->scrolling = TRUE;
        ZigzagWipe_Start(effect, work->band, 6, 8, 16, 31, 23);
        SetBgPriority(GF_BG_LYR_MAIN_2, 0);
        SetBgPriority(GF_BG_LYR_MAIN_3, 1);
        SetBgPriority(GF_BG_LYR_MAIN_0, 2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
        effect->state++;
        break;
    case 4:
        if (ov01_021EFE30(effect)) {
            effect->state++;
            ZigzagWipe_Free(work->band);
            work->timer = 10;
        }
        break;
    case 5:
        if (--work->timer < 0) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
            if (ScaleSprites_Update(&work->scaleSprites) == TRUE) {
                effect->state++;
            }
        }
        break;
    case 6:
        EncounterEffectQuadInterp_Init(&work->slide, FX32_CONST(272), params->endX, -FX32_CONST(64), 4);
        Sprite_SetDrawFlag(work->leaderSprite, TRUE);
        Sprite_SetPriority(work->leaderSprite, 1);
        EncounterEffect_SetVecFx32(&position, work->slide.value, FX32_CONST(66), 0);
        Sprite_SetMatrix(work->leaderSprite, &position);
        effect->state++;
        break;
    case 7:
        done = EncounterEffectQuadInterp_Update(&work->slide);
        EncounterEffect_SetVecFx32(&position, work->slide.value, FX32_CONST(66), 0);
        Sprite_SetMatrix(work->leaderSprite, &position);
        if (done == TRUE) {
            effect->state++;
        }
        break;
    case 8:
        EncounterEffectInterp_Init(&work->brightness, 0, 16, 3);
        work->timer = 10;
        effect->state++;
        break;
    case 9:
        if (--work->timer < 0) {
            done = EncounterEffectInterp_Update(&work->brightness);
            EncounterEffect_ApplyMainBrightness(&work->brightness);
            if (done == TRUE) {
                LeadersEffect_LoadBlendedSpritePalette(work->leaderSprite, heapID, params->spriteMembers[0], 0, RGB_BLACK);
                SetBlendBrightness(-14, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BD), 1);
                ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, -((work->slide.value >> FX32_SHIFT) - 92));
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
                effect->state++;
            }
        }
        break;
    case 10:
        EncounterEffectInterp_Init(&work->brightness, 16, 0, 3);
        effect->state++;
        break;
    case 11:
        done = EncounterEffectInterp_Update(&work->brightness);
        EncounterEffect_ApplyMainBrightness(&work->brightness);
        if (done == TRUE) {
            effect->state++;
            work->timer = 26;
        }
        break;
    case 12:
        if (--work->timer < 0) {
            effect->state++;
        }
        break;
    case 13:
        BeginNormalPaletteFade(3, 0, 0, RGB_WHITE, 15, 1, HEAP_ID_FIELD1);
        effect->state++;
        break;
    case 14:
        if (IsPaletteFadeFinished()) {
            effect->state++;
        }
        break;
    case 15:
        SetMasterBrightnessByColor(1, RGB_WHITE);
        if (effect->done) {
            *effect->done = TRUE;
        }
        Sprite_Delete(work->leaderSprite);
        ScaleSprites_Delete(&work->scaleSprites);
        EncounterEffect_FreeSpriteResources(&work->spriteManager, &work->leaderResources);
        EncounterEffect_FreeSpriteResources(&work->spriteManager, &work->scaleSpriteResources);
        EncounterEffect_FreeSprites(&work->spriteManager);
        RemoveWindow(&work->window);
        reg_GX_DISPCNT &= 0xFFFF1FFF;
        SetBlendBrightness(0, (GXBlendPlaneMask)0, 1);
        BgSetPosTextAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        return TRUE;
    }
    if (work->scrolling == TRUE) {
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, work->scroll);
        work->scroll = (work->scroll + 30) % 512;
    }
    if (effect->state != 15) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
    return FALSE;
}
#pragma force_active reset

void EncounterEffect_LeaderFalkner(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_022603B0) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderBugsy(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_022603C4) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderWhitney(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_022603D8) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderMorty(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_022603EC) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderJasmine(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_02260400) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderChuck(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_02260414) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderPryce(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_02260428) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderClair(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_0226043C) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderBrock(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_02260450) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderMisty(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_02260464) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderLtSurge(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_02260478) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderErika(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_0226048C) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderJanine(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_022604A0) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderSabrina(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_022604B4) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderBlaine(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_022604C8) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderBlue(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &ov115_022604DC) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_Rival(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &_02260374) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

u32 LeadersEffect_GetPlayerGender(FieldSystem *fieldSystem) {
    return PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(fieldSystem->saveData));
}

static inline void EliteFourIntro_CreateSprites(EncounterEffectSpriteManager *spriteManager, EncounterEffectSpriteResources resources[4], Sprite *sprites[4]) {
    EncounterEffectSpriteResources *cursor = resources;
    int i;

    for (i = 0; i < 4; i++, cursor++) {
        EncounterEffectSpriteResources *selected;
        int animSeq;

        if (i < 3) {
            selected = cursor;
            animSeq = 0;
        } else {
            selected = &resources[i - 1];
            animSeq = 1;
        }
        sprites[i] = EncounterEffect_CreateSprite(spriteManager, selected, 0, 0, 0, 0);
        Sprite_SetDrawFlag(sprites[i], FALSE);
        Sprite_SetAnimCtrlSeq(sprites[i], animSeq);
        Sprite_SetPriority(sprites[i], 1);
    }
}

// sprites[0]: player, [1]: opponent, [2] and [3]: their nameplates
#pragma force_active on
BOOL LeadersEffect_RunEliteFourIntro(EncounterEffectTask *effect, enum HeapID heapID, const EliteFourIntroParams *params) {
    VecFx32 pos;
    VecFx32 initial;
    EliteFourIntroWork *work = effect->work;
    EncounterEffectSpriteResources *resources;
    EncounterEffectSpriteManager *spriteManager;
    int i;
    BOOL done;
    BOOL ready;
    String *string;

    switch (effect->state) {
    case 0:
        effect->work = Heap_Alloc(heapID, sizeof(EliteFourIntroWork));
        memset(effect->work, 0, sizeof(EliteFourIntroWork));
        work = effect->work;
        EncounterEffect_InitSprites(&work->spriteManager, 10, 4);
        if (!LeadersEffect_GetPlayerGender(effect->fieldSystem)) {
            EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->resources[0], 207, 1, 208, 209, 210, 600000);
            work->playerGender = 0;
            work->playerMember = 207;
        } else {
            EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->resources[0], 211, 1, 212, 213, 214, 600000);
            work->playerGender = 1;
            work->playerMember = 211;
        }
        EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->resources[1], params->opponentMember, 1, params->opponentMember + 1, params->opponentMember + 2, params->opponentMember + 3, 600001);
        EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->resources[2], params->nameplateMember, 12, 48, 49, 50, 600002);
        EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->resources[3], 59, 1, 60, 61, 62, 600003);
        effect->state++;
        break;
    case 1:
        EncounterEffect_SetVecFx32(&initial, FX32_CONST(2), FX32_CONST(2), 0);
        EliteFourIntro_CreateSprites(&work->spriteManager, work->resources, work->sprites);
        LeadersEffect_LoadBlendedSpritePalette(work->sprites[0], heapID, work->playerMember, 14, RGB_BLACK);
        LeadersEffect_LoadBlendedSpritePalette(work->sprites[1], heapID, params->opponentMember, 14, RGB_BLACK);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        ScaleSprites_Init(&work->scaleSprites, &work->spriteManager, &work->resources[3], FX32_CONST(128), FX32_CONST(96), heapID);
        EncounterEffect_StartParticleMode(effect->fieldSystem);
        effect->state++;
        break;
    case 2:
        EncounterEffect_StartFlash(1, 16, 16, &effect->fadeDone, 1);
        effect->timer = 0;
        effect->state++;
        break;
    case 3:
        if (++effect->timer == 8) {
            EncounterEffect_RequestParticleSetup();
        }
        if (effect->fadeDone) {
            effect->state++;
        }
        break;
    case 4:
        if (EncounterEffect_IsParticleSetupDone() == TRUE) {
            EncounterEffect_LoadParticles(effect->narc, 151);
            G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 0, 8);
            effect->state++;
        }
        break;
    case 5:
        EncounterEffectQuadInterp_Init(&work->x, -FX32_CONST(128), FX32_CONST(56), FX32_CONST(80), 6);
        EncounterEffect_SetVecFx32(&pos, work->x.value, FX32_CONST(92), 0);
        Sprite_SetMatrix(work->sprites[0], &pos);
        pos.y += FX32_CONST(4);
        pos.x += FX32_CONST(16);
        Sprite_SetMatrix(work->sprites[2], &pos);
        Sprite_SetDrawFlag(work->sprites[0], TRUE);
        Sprite_SetDrawFlag(work->sprites[2], TRUE);
        EncounterEffectQuadInterp_Init(&work->slide, FX32_CONST(384), FX32_CONST(200), -FX32_CONST(80), 6);
        EncounterEffect_SetVecFx32(&pos, work->slide.value, FX32_CONST(92), 0);
        Sprite_SetMatrix(work->sprites[1], &pos);
        pos.y += FX32_CONST(4);
        pos.x -= FX32_CONST(16);
        Sprite_SetMatrix(work->sprites[3], &pos);
        Sprite_SetDrawFlag(work->sprites[1], TRUE);
        Sprite_SetDrawFlag(work->sprites[3], TRUE);
        GfGfxLoader_GXLoadPalFromOpenNarc(effect->narc, 16, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_2_OFFSET, 32, heapID);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        AddWindowParameterized(effect->fieldSystem->bgConfig, &work->window, GF_BG_LYR_MAIN_2, 21, 13, 11, 2, 2, 1);
        FillWindowPixelBuffer(&work->window, 0);
        string = LeadersEffect_GetTrainerNameString(params->trainerId, heapID);
        AddTextPrinterParameterizedWithColor(&work->window, 0, string, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(string);
        effect->fadeDone = 3; // reused as a frame counter from here on
        effect->state++;
        break;
    case 6:
        if (effect->fadeDone > 0) {
            if (--effect->fadeDone == 0) {
                EncounterEffect_CreateEmitters(3);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            }
        } else {
            ScaleSprites_Update(&work->scaleSprites);
        }
        EncounterEffectQuadInterp_Update(&work->x);
        EncounterEffect_SetVecFx32(&pos, work->x.value, FX32_CONST(92), 0);
        work->playerPos = pos;
        Sprite_SetMatrix(work->sprites[0], &pos);
        pos.y += FX32_CONST(4);
        pos.x += FX32_CONST(16);
        Sprite_SetMatrix(work->sprites[2], &pos);
        ready = EncounterEffectQuadInterp_Update(&work->slide);
        EncounterEffect_SetVecFx32(&pos, work->slide.value, FX32_CONST(92), 0);
        work->opponentPos = pos;
        Sprite_SetMatrix(work->sprites[1], &pos);
        pos.y += FX32_CONST(4);
        pos.x -= FX32_CONST(16);
        Sprite_SetMatrix(work->sprites[3], &pos);
        if (ready == TRUE) {
            effect->state++;
        }
        break;
    case 7:
        ready = ScaleSprites_Update(&work->scaleSprites);
        done = EncounterEffect_AreParticlesDone();
        if (ready && done) {
            EncounterEffectInterp_Init(&work->fade, 0, 16, 3);
            EncounterEffect_FreeParticles();
            effect->state++;
        }
        break;
    case 8:
        ready = EncounterEffectInterp_Update(&work->fade);
        EncounterEffect_ApplyMainBrightness(&work->fade);
        if (ready == TRUE) {
            LeadersEffect_LoadBlendedSpritePalette(work->sprites[0], heapID, work->playerMember, 0, RGB_BLACK);
            LeadersEffect_LoadBlendedSpritePalette(work->sprites[1], heapID, params->opponentMember, 0, RGB_BLACK);
            Sprite_SetAnimActiveFlag(work->sprites[2], TRUE);
            Sprite_SetAnimSpeed(work->sprites[2], FX32_CONST(2));
            Sprite_SetAnimActiveFlag(work->sprites[3], TRUE);
            Sprite_SetAnimSpeed(work->sprites[3], FX32_CONST(2));
            EncounterEffect_LoadParticles(effect->narc, 152);
            effect->state++;
        }
        break;
    case 9:
        EncounterEffectInterp_Init(&work->fade, 16, 0, 6);
        EncounterEffect_CreateEmitters(4);
        SetBgPriority(GF_BG_LYR_MAIN_0, 1);
        effect->state++;
        break;
    case 10:
        ready = EncounterEffectInterp_Update(&work->fade);
        EncounterEffect_ApplyMainBrightness(&work->fade);
        if (ready == TRUE) {
            effect->state++;
            effect->fadeDone = 8;
        }
        break;
    case 11:
        if (effect->fadeDone > 0) {
            effect->fadeDone--;
        } else {
            EncounterEffectQuadInterp_Init(&work->x, 0, -FX32_CONST(2), 0, params->shakeFrames);
            EncounterEffectQuadInterp_Init(&work->y, 0, -FX32_CONST(2), 0, params->shakeFrames);
            effect->fadeDone = 0;
            effect->state++;
        }
        break;
    case 12:
        effect->fadeDone++;
        ready = EncounterEffectQuadInterp_Update(&work->x);
        EncounterEffectQuadInterp_Update(&work->y);
        if ((effect->fadeDone / 2) % 2 == 0) {
            EncounterEffect_SetVecFx32(&pos, work->playerPos.x + work->x.value, work->playerPos.y + work->y.value, 0);
        } else {
            EncounterEffect_SetVecFx32(&pos, work->playerPos.x - work->x.value, work->playerPos.y - work->y.value, 0);
        }
        if (ready) {
            work->playerPos = pos;
        }
        Sprite_SetMatrix(work->sprites[0], &pos);
        pos.y += FX32_CONST(4);
        pos.x += FX32_CONST(16);
        Sprite_SetMatrix(work->sprites[2], &pos);
        if ((effect->fadeDone / 2) % 2 == 0) {
            EncounterEffect_SetVecFx32(&pos, work->opponentPos.x - work->x.value, work->opponentPos.y - work->y.value, 0);
        } else {
            EncounterEffect_SetVecFx32(&pos, work->opponentPos.x + work->x.value, work->opponentPos.y + work->y.value, 0);
        }
        if (ready) {
            work->opponentPos = pos;
        }
        Sprite_SetMatrix(work->sprites[1], &pos);
        pos.y += FX32_CONST(4);
        pos.x -= FX32_CONST(16);
        Sprite_SetMatrix(work->sprites[3], &pos);
        if (ready) {
            effect->state++;
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            EncounterEffectQuadInterp_Init(&work->x, 0, FX32_CONST(192), FX32_CONST(24), 16);
            EncounterEffectQuadInterp_Init(&work->y, 0, FX32_CONST(192), FX32_CONST(24), 16);
            BeginNormalPaletteFade(3, 0, 0, RGB_WHITE, 8, 1, HEAP_ID_FIELD1);
        }
        break;
    case 13:
        EncounterEffectQuadInterp_Update(&work->x);
        EncounterEffectQuadInterp_Update(&work->y);
        EncounterEffect_SetVecFx32(&pos, work->playerPos.x - work->x.value, work->playerPos.y - work->y.value, 0);
        Sprite_SetMatrix(work->sprites[0], &pos);
        pos.y += FX32_CONST(4);
        pos.x += FX32_CONST(16);
        Sprite_SetMatrix(work->sprites[2], &pos);
        EncounterEffect_SetVecFx32(&pos, work->opponentPos.x + work->x.value, work->opponentPos.y + work->y.value, 0);
        Sprite_SetMatrix(work->sprites[1], &pos);
        pos.y += FX32_CONST(4);
        pos.x -= FX32_CONST(16);
        Sprite_SetMatrix(work->sprites[3], &pos);
        if (IsPaletteFadeFinished()) {
            effect->state++;
        }
        break;
    case 14:
        SetMasterBrightnessByColor(1, RGB_WHITE);
        if (effect->done) {
            *effect->done = TRUE;
        }
        ScaleSprites_Delete(&work->scaleSprites);
        RemoveWindow(&work->window);
        for (i = 0; i < 4; i++) {
            Sprite_Delete(work->sprites[i]);
        }
        for (i = 0, resources = work->resources, spriteManager = &work->spriteManager; i < 4; i++, resources++) {
            EncounterEffect_FreeSpriteResources(spriteManager, resources);
        }
        EncounterEffect_FreeSprites(&work->spriteManager);
        EncounterEffect_FreeParticles();
        EncounterEffect_EndParticleMode();
        reg_G2_BLDCNT = 0;
        return TRUE;
    }
    if (effect->state != 14) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
        if ((int)effect->state > 4) {
            Thunk_G3X_Reset();
            EncounterEffect_UpdateParticles();
            EncounterEffect_DrawParticles();
            RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        }
    }
    return FALSE;
}
#pragma force_active reset

void LeadersEffect_LoadBlendedSpritePalette(Sprite *sprite, enum HeapID heapID, s32 member, u8 amount, u16 color) {
    NNSG2dPaletteData *paletteData;
    void *file;
    NNSG2dImagePaletteProxy *proxy;
    u16 *buffer = Heap_Alloc(heapID, 32);

    file = GfGfxLoader_GetPlttData(NARC_a_1_0_9, member, &paletteData, heapID);
    BlendPalette(paletteData->pRawData, buffer, 16, amount, color);
    proxy = Sprite_GetPaletteProxy(sprite);
    DC_FlushRange(buffer, 32);
    GX_LoadOBJPltt(buffer, NNS_G2dGetImagePaletteLocation(proxy, NNS_G2D_VRAM_TYPE_2DMAIN), 32);
    Heap_Free(buffer);
    Heap_Free(file);
}

void EncounterEffect_EliteFourWill(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunEliteFourIntro(effect, HEAP_ID_FIELD1, &ov115_02260388) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_EliteFourKoga(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunEliteFourIntro(effect, HEAP_ID_FIELD1, &ov115_02260390) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_EliteFourBruno(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunEliteFourIntro(effect, HEAP_ID_FIELD1, &ov115_02260398) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_EliteFourKaren(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunEliteFourIntro(effect, HEAP_ID_FIELD1, &ov115_022603A0) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_Champion(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunEliteFourIntro(effect, HEAP_ID_FIELD1, &ov115_022603A8) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}
