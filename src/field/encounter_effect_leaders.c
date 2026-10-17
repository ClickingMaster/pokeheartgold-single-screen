#include "field/encounter_effect_leaders.h"

#include "global.h"

#include "constants/trainer_class.h"
#include "constants/trainers.h"

#include "field/encounter_effect.h"

#include "bg_window.h"
#include "brightness.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "palette.h"
#include "player_data.h"
#include "pm_string.h"
#include "screen_fade_legacy.h"
#include "sprite.h"
#include "swap_3d_buffers.h"
#include "text.h"

// Battle intros of the gym leaders and the rival (effects 12-28): the leader's picture slides
// in over a scrolling band with the leader's name. Elite Four and champion (effects 29-33):
// the player and the opponent slide in from both sides and shake.

typedef struct GymLeaderIntroWork {
    EncounterEffectQuadInterp slide;
    EncounterEffectInterp brightness;
    u8 unk_2C[0x14];
    ZigzagWipe *wipe;
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

BOOL LeadersEffect_RunGymLeaderIntro(EncounterEffectTask *effect, enum HeapID heapID, const GymLeaderIntroParams *params);
u32 LeadersEffect_GetPlayerGender(FieldSystem *fieldSystem);
BOOL LeadersEffect_RunEliteFourIntro(EncounterEffectTask *effect, enum HeapID heapID, const EliteFourIntroParams *params);
void LeadersEffect_LoadBlendedSpritePalette(Sprite *sprite, enum HeapID heapID, s32 member, u8 amount, u16 color);

static const EliteFourIntroParams sEncounterEliteFourKarenIntroParams = {
    .opponentMember = 143,
    .nameplateMember = 53,
    .shakeFrames = 32,
    .unk_04 = 88,
    .trainerId = TRAINER_ELITE_FOUR_KAREN_KAREN,
};

static const EliteFourIntroParams sEncounterEliteFourKogaIntroParams = {
    .opponentMember = 135,
    .nameplateMember = 51,
    .shakeFrames = 32,
    .unk_04 = 89,
    .trainerId = TRAINER_ELITE_FOUR_KOGA_KOGA,
};

static const EliteFourIntroParams sEncounterChampionIntroParams = {
    .opponentMember = 147,
    .nameplateMember = 54,
    .shakeFrames = 9,
    .unk_04 = 86,
    .trainerId = TRAINER_CHAMPION_LANCE,
};

static const EliteFourIntroParams sEncounterEliteFourBrunoIntroParams = {
    .opponentMember = 139,
    .nameplateMember = 52,
    .shakeFrames = 32,
    .unk_04 = 112,
    .trainerId = TRAINER_ELITE_FOUR_BRUNO_BRUNO,
};

static const EliteFourIntroParams sEncounterEliteFourWillIntroParams = {
    .opponentMember = 131,
    .nameplateMember = 47,
    .shakeFrames = 32,
    .unk_04 = 87,
    .trainerId = TRAINER_ELITE_FOUR_WILL_WILL,
};

static const GymLeaderIntroParams sEncounterLeaderErikaIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_ERIKA_ERIKA,
    .trainerClass = TRAINERCLASS_LEADER_ERIKA,
    .unk_0A = 1,
    .spriteMembers = { 107, 108, 109, 110 },
    .bgMembers = { 30, 29, 31 },
};

static const GymLeaderIntroParams sEncounterLeaderBrockIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_BROCK_BROCK,
    .trainerClass = TRAINERCLASS_LEADER_BROCK,
    .unk_0A = 1,
    .spriteMembers = { 95, 96, 97, 98 },
    .bgMembers = { 21, 20, 22 },
};

static const GymLeaderIntroParams sEncounterLeaderClairIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_CLAIR_CLAIR,
    .trainerClass = TRAINERCLASS_LEADER_CLAIR,
    .unk_0A = 1,
    .spriteMembers = { 91, 92, 93, 94 },
    .bgMembers = { 42, 41, 43 },
};

static const GymLeaderIntroParams sEncounterLeaderPryceIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_PRYCE_PRYCE,
    .trainerClass = TRAINERCLASS_LEADER_PRYCE,
    .unk_0A = 1,
    .spriteMembers = { 87, 88, 89, 90 },
    .bgMembers = { 39, 38, 40 },
};

static const GymLeaderIntroParams sEncounterLeaderChuckIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_CHUCK_CHUCK,
    .trainerClass = TRAINERCLASS_LEADER_CHUCK,
    .unk_0A = 1,
    .spriteMembers = { 83, 84, 85, 86 },
    .bgMembers = { 36, 35, 37 },
};

static const GymLeaderIntroParams sEncounterLeaderMortyIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_MORTY_MORTY,
    .trainerClass = TRAINERCLASS_LEADER_MORTY,
    .unk_0A = 1,
    .spriteMembers = { 75, 76, 77, 78 },
    .bgMembers = { 30, 29, 31 },
};

static const GymLeaderIntroParams sEncounterLeaderJasmineIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_JASMINE_JASMINE,
    .trainerClass = TRAINERCLASS_LEADER_JASMINE,
    .unk_0A = 1,
    .spriteMembers = { 79, 80, 81, 82 },
    .bgMembers = { 33, 32, 34 },
};

static const GymLeaderIntroParams sEncounterLeaderLtSurgeIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_LT_SURGE_LT__SURGE,
    .trainerClass = TRAINERCLASS_LEADER_LT_SURGE,
    .unk_0A = 0,
    .spriteMembers = { 103, 104, 105, 106 },
    .bgMembers = { 27, 26, 28 },
};

static const GymLeaderIntroParams sEncounterLeaderJanineIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_JANINE_JANINE,
    .trainerClass = TRAINERCLASS_LEADER_JANINE,
    .unk_0A = 1,
    .spriteMembers = { 111, 112, 113, 114 },
    .bgMembers = { 33, 32, 34 },
};

static const GymLeaderIntroParams sEncounterLeaderFalknerIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_FALKNER_FALKNER,
    .trainerClass = TRAINERCLASS_LEADER_FALKNER,
    .unk_0A = 1,
    .spriteMembers = { 63, 64, 65, 66 },
    .bgMembers = { 21, 20, 22 },
};

static const GymLeaderIntroParams sEncounterLeaderBugsyIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_BUGSY_BUGSY,
    .trainerClass = TRAINERCLASS_LEADER_BUGSY,
    .unk_0A = 1,
    .spriteMembers = { 67, 68, 69, 70 },
    .bgMembers = { 24, 23, 25 },
};

static const GymLeaderIntroParams sEncounterLeaderMistyIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_MISTY_MISTY,
    .trainerClass = TRAINERCLASS_LEADER_MISTY,
    .unk_0A = 1,
    .spriteMembers = { 99, 100, 101, 102 },
    .bgMembers = { 24, 23, 25 },
};

static const GymLeaderIntroParams sEncounterLeaderSabrinaIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_SABRINA_SABRINA,
    .trainerClass = TRAINERCLASS_LEADER_SABRINA,
    .unk_0A = 1,
    .spriteMembers = { 115, 116, 117, 118 },
    .bgMembers = { 36, 35, 37 },
};

static const GymLeaderIntroParams sEncounterLeaderWhitneyIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_WHITNEY,
    .trainerClass = TRAINERCLASS_LEADER_WHITNEY,
    .unk_0A = 0,
    .spriteMembers = { 71, 72, 73, 74 },
    .bgMembers = { 27, 26, 28 },
};

static const GymLeaderIntroParams sEncounterLeaderBlaineIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_BLAINE_BLAINE,
    .trainerClass = TRAINERCLASS_LEADER_BLAINE,
    .unk_0A = 1,
    .spriteMembers = { 119, 120, 121, 122 },
    .bgMembers = { 39, 38, 40 },
};

static const GymLeaderIntroParams sEncounterLeaderBlueIntroParams = {
    .endX = FX32_CONST(214),
    .trainerId = TRAINER_LEADER_BLUE_BLUE,
    .trainerClass = TRAINERCLASS_LEADER_BLUE,
    .unk_0A = 1,
    .spriteMembers = { 123, 124, 125, 126 },
    .bgMembers = { 42, 41, 43 },
};

enum GymLeaderIntroState {
    GYM_LEADER_INTRO_STATE_INIT,
    GYM_LEADER_INTRO_STATE_START_FLASH,
    GYM_LEADER_INTRO_STATE_WAIT_FLASH,
    GYM_LEADER_INTRO_STATE_START_WIPE,
    GYM_LEADER_INTRO_STATE_WAIT_WIPE,
    GYM_LEADER_INTRO_STATE_SCALE_SPRITES,
    GYM_LEADER_INTRO_STATE_START_SLIDE_IN,
    GYM_LEADER_INTRO_STATE_SLIDE_IN,
    GYM_LEADER_INTRO_STATE_START_BRIGHTEN,
    GYM_LEADER_INTRO_STATE_BRIGHTEN,
    GYM_LEADER_INTRO_STATE_START_RESTORE_BRIGHTNESS,
    GYM_LEADER_INTRO_STATE_RESTORE_BRIGHTNESS,
    GYM_LEADER_INTRO_STATE_HOLD,
    GYM_LEADER_INTRO_STATE_START_FADE,
    GYM_LEADER_INTRO_STATE_WAIT_FADE,
    GYM_LEADER_INTRO_STATE_END,
};

BOOL LeadersEffect_RunGymLeaderIntro(EncounterEffectTask *effect, enum HeapID heapID, const GymLeaderIntroParams *params) {
    u32 state = effect->state;
    GymLeaderIntroWork *work = effect->work;
    String *string;
    BOOL done;
    VecFx32 position;

    switch (state) {
    case GYM_LEADER_INTRO_STATE_INIT:
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
        work->wipe = ZigzagWipe_New();
        effect->state++;
        break;
    case GYM_LEADER_INTRO_STATE_START_FLASH:
        EncounterEffect_StartFlash(1, 16, 16, &effect->fadeDone, 1);
        effect->state++;
        break;
    case GYM_LEADER_INTRO_STATE_WAIT_FLASH:
        if (effect->fadeDone) {
            effect->state = state + 1;
        }
        break;
    case GYM_LEADER_INTRO_STATE_START_WIPE:
        EncounterEffect_LoadBgGraphics(effect->narc, params->bgMembers[2], params->bgMembers[1], params->bgMembers[0], 0, 1, effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3);
        work->scrolling = TRUE;
        ZigzagWipe_Start(effect, work->wipe, 6, 8, 16, 31, 23);
        SetBgPriority(GF_BG_LYR_MAIN_2, 0);
        SetBgPriority(GF_BG_LYR_MAIN_3, 1);
        SetBgPriority(GF_BG_LYR_MAIN_0, 2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
        effect->state++;
        break;
    case GYM_LEADER_INTRO_STATE_WAIT_WIPE:
        if (EncounterEffect_IsWipeDone(effect)) {
            effect->state++;
            ZigzagWipe_Free(work->wipe);
            work->timer = 10;
        }
        break;
    case GYM_LEADER_INTRO_STATE_SCALE_SPRITES:
        if (--work->timer < 0) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
            if (ScaleSprites_Update(&work->scaleSprites) == TRUE) {
                effect->state++;
            }
        }
        break;
    case GYM_LEADER_INTRO_STATE_START_SLIDE_IN:
        EncounterEffectQuadInterp_Init(&work->slide, FX32_CONST(272), params->endX, -FX32_CONST(64), 4);
        Sprite_SetDrawFlag(work->leaderSprite, TRUE);
        Sprite_SetPriority(work->leaderSprite, 1);
        EncounterEffect_SetVecFx32(&position, work->slide.value, FX32_CONST(66), 0);
        Sprite_SetMatrix(work->leaderSprite, &position);
        effect->state++;
        break;
    case GYM_LEADER_INTRO_STATE_SLIDE_IN:
        done = EncounterEffectQuadInterp_Update(&work->slide);
        EncounterEffect_SetVecFx32(&position, work->slide.value, FX32_CONST(66), 0);
        Sprite_SetMatrix(work->leaderSprite, &position);
        if (done == TRUE) {
            effect->state++;
        }
        break;
    case GYM_LEADER_INTRO_STATE_START_BRIGHTEN:
        EncounterEffectInterp_Init(&work->brightness, 0, 16, 3);
        work->timer = 10;
        effect->state++;
        break;
    case GYM_LEADER_INTRO_STATE_BRIGHTEN:
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
    case GYM_LEADER_INTRO_STATE_START_RESTORE_BRIGHTNESS:
        EncounterEffectInterp_Init(&work->brightness, 16, 0, 3);
        effect->state++;
        break;
    case GYM_LEADER_INTRO_STATE_RESTORE_BRIGHTNESS:
        done = EncounterEffectInterp_Update(&work->brightness);
        EncounterEffect_ApplyMainBrightness(&work->brightness);
        if (done == TRUE) {
            effect->state++;
            work->timer = 26;
        }
        break;
    case GYM_LEADER_INTRO_STATE_HOLD:
        if (--work->timer < 0) {
            effect->state++;
        }
        break;
    case GYM_LEADER_INTRO_STATE_START_FADE:
        BeginNormalPaletteFade(3, 0, 0, RGB_WHITE, 15, 1, HEAP_ID_FIELD1);
        effect->state++;
        break;
    case GYM_LEADER_INTRO_STATE_WAIT_FADE:
        if (IsPaletteFadeFinished()) {
            effect->state++;
        }
        break;
    case GYM_LEADER_INTRO_STATE_END:
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
        SetBlendBrightness(0, GX_BLEND_PLANEMASK_NONE, 1);
        BgSetPosTextAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        return TRUE;
    }
    if (work->scrolling == TRUE) {
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, work->scroll);
        work->scroll = (work->scroll + 30) % 512;
    }
    if (effect->state != GYM_LEADER_INTRO_STATE_END) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
    return FALSE;
}

void EncounterEffect_LeaderFalkner(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderFalknerIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderBugsy(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderBugsyIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderWhitney(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderWhitneyIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderMorty(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderMortyIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderJasmine(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderJasmineIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderChuck(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderChuckIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderPryce(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderPryceIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderClair(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderClairIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderBrock(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderBrockIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderMisty(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderMistyIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderLtSurge(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderLtSurgeIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderErika(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderErikaIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderJanine(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderJanineIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderSabrina(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderSabrinaIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderBlaine(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderBlaineIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_LeaderBlue(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &sEncounterLeaderBlueIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_Rival(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunGymLeaderIntro(effect, HEAP_ID_FIELD1, &gEncounterRivalIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

u32 LeadersEffect_GetPlayerGender(FieldSystem *fieldSystem) {
    return PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(fieldSystem->saveData));
}

enum EliteFourIntroState {
    ELITE_FOUR_INTRO_STATE_LOAD_RESOURCES,
    ELITE_FOUR_INTRO_STATE_CREATE_SPRITES,
    ELITE_FOUR_INTRO_STATE_START_FLASH,
    ELITE_FOUR_INTRO_STATE_WAIT_FLASH,
    ELITE_FOUR_INTRO_STATE_LOAD_PARTICLES,
    ELITE_FOUR_INTRO_STATE_START_SLIDE_IN,
    ELITE_FOUR_INTRO_STATE_SLIDE_IN,
    ELITE_FOUR_INTRO_STATE_WAIT_EFFECTS,
    ELITE_FOUR_INTRO_STATE_BRIGHTEN,
    ELITE_FOUR_INTRO_STATE_START_RESTORE_BRIGHTNESS,
    ELITE_FOUR_INTRO_STATE_RESTORE_BRIGHTNESS,
    ELITE_FOUR_INTRO_STATE_WAIT_SHAKE,
    ELITE_FOUR_INTRO_STATE_SHAKE,
    ELITE_FOUR_INTRO_STATE_SLIDE_OUT,
    ELITE_FOUR_INTRO_STATE_END,
};

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
    case ELITE_FOUR_INTRO_STATE_LOAD_RESOURCES:
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
    case ELITE_FOUR_INTRO_STATE_CREATE_SPRITES:
        EncounterEffect_SetVecFx32(&initial, FX32_CONST(2), FX32_CONST(2), 0);
        EliteFourIntro_CreateSprites(&work->spriteManager, work->resources, work->sprites);
        LeadersEffect_LoadBlendedSpritePalette(work->sprites[0], heapID, work->playerMember, 14, RGB_BLACK);
        LeadersEffect_LoadBlendedSpritePalette(work->sprites[1], heapID, params->opponentMember, 14, RGB_BLACK);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        ScaleSprites_Init(&work->scaleSprites, &work->spriteManager, &work->resources[3], FX32_CONST(128), FX32_CONST(96), heapID);
        EncounterEffect_StartParticleMode(effect->fieldSystem);
        effect->state++;
        break;
    case ELITE_FOUR_INTRO_STATE_START_FLASH:
        EncounterEffect_StartFlash(1, 16, 16, &effect->fadeDone, 1);
        effect->timer = 0;
        effect->state++;
        break;
    case ELITE_FOUR_INTRO_STATE_WAIT_FLASH:
        if (++effect->timer == 8) {
            EncounterEffect_RequestParticleSetup();
        }
        if (effect->fadeDone) {
            effect->state++;
        }
        break;
    case ELITE_FOUR_INTRO_STATE_LOAD_PARTICLES:
        if (EncounterEffect_IsParticleSetupDone() == TRUE) {
            EncounterEffect_LoadParticles(effect->narc, 151);
            G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 0, 8);
            effect->state++;
        }
        break;
    case ELITE_FOUR_INTRO_STATE_START_SLIDE_IN:
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
    case ELITE_FOUR_INTRO_STATE_SLIDE_IN:
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
    case ELITE_FOUR_INTRO_STATE_WAIT_EFFECTS:
        ready = ScaleSprites_Update(&work->scaleSprites);
        done = EncounterEffect_AreParticlesDone();
        if (ready && done) {
            EncounterEffectInterp_Init(&work->fade, 0, 16, 3);
            EncounterEffect_FreeParticles();
            effect->state++;
        }
        break;
    case ELITE_FOUR_INTRO_STATE_BRIGHTEN:
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
    case ELITE_FOUR_INTRO_STATE_START_RESTORE_BRIGHTNESS:
        EncounterEffectInterp_Init(&work->fade, 16, 0, 6);
        EncounterEffect_CreateEmitters(4);
        SetBgPriority(GF_BG_LYR_MAIN_0, 1);
        effect->state++;
        break;
    case ELITE_FOUR_INTRO_STATE_RESTORE_BRIGHTNESS:
        ready = EncounterEffectInterp_Update(&work->fade);
        EncounterEffect_ApplyMainBrightness(&work->fade);
        if (ready == TRUE) {
            effect->state++;
            effect->fadeDone = 8;
        }
        break;
    case ELITE_FOUR_INTRO_STATE_WAIT_SHAKE:
        if (effect->fadeDone > 0) {
            effect->fadeDone--;
        } else {
            EncounterEffectQuadInterp_Init(&work->x, 0, -FX32_CONST(2), 0, params->shakeFrames);
            EncounterEffectQuadInterp_Init(&work->y, 0, -FX32_CONST(2), 0, params->shakeFrames);
            effect->fadeDone = 0;
            effect->state++;
        }
        break;
    case ELITE_FOUR_INTRO_STATE_SHAKE:
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
    case ELITE_FOUR_INTRO_STATE_SLIDE_OUT:
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
    case ELITE_FOUR_INTRO_STATE_END:
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
    if (effect->state != ELITE_FOUR_INTRO_STATE_END) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
        if ((int)effect->state > ELITE_FOUR_INTRO_STATE_LOAD_PARTICLES) {
            Thunk_G3X_Reset();
            EncounterEffect_UpdateParticles();
            EncounterEffect_DrawParticles();
            RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        }
    }
    return FALSE;
}

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
    if (LeadersEffect_RunEliteFourIntro(effect, HEAP_ID_FIELD1, &sEncounterEliteFourWillIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_EliteFourKoga(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunEliteFourIntro(effect, HEAP_ID_FIELD1, &sEncounterEliteFourKogaIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_EliteFourBruno(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunEliteFourIntro(effect, HEAP_ID_FIELD1, &sEncounterEliteFourBrunoIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_EliteFourKaren(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunEliteFourIntro(effect, HEAP_ID_FIELD1, &sEncounterEliteFourKarenIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}

void EncounterEffect_Champion(SysTask *task, EncounterEffectTask *effect) {
    if (LeadersEffect_RunEliteFourIntro(effect, HEAP_ID_FIELD1, &sEncounterChampionIntroParams) == TRUE) {
        EncounterEffect_Finish(effect, task);
    }
}
