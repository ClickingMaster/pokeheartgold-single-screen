#include "global.h"

#include "field/encounter_effect.h"
#include "msgdata/msg.naix"

#include "bg_window.h"
#include "brightness.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "sprite.h"
#include "text.h"
#include "unk_0200FA24.h"

// Team Rocket battle intros: effect 39 (grunts) and 40-44 (executives and the boss).

#define ROCKET_EFFECT_SPRITE_COUNT 6

typedef struct TeamRocketEffectWork {
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources spriteResources;
    Sprite *sprites[ROCKET_EFFECT_SPRITE_COUNT];
    EncounterEffectQuadInterp scale[ROCKET_EFFECT_SPRITE_COUNT];
    EncounterEffectQuadInterp x[ROCKET_EFFECT_SPRITE_COUNT];
    EncounterEffectQuadInterp y[ROCKET_EFFECT_SPRITE_COUNT];
    EncounterEffectInterp rotation[ROCKET_EFFECT_SPRITE_COUNT];
    BOOL active[ROCKET_EFFECT_SPRITE_COUNT];
    int next;
    int timer;
} TeamRocketEffectWork;

typedef struct RocketExecutiveIntroParams {
    u8 spriteMembers[4]; // character, palette, cell, animation
    u32 trainerId;
} RocketExecutiveIntroParams;

typedef struct RocketExecutiveEffectWork {
    Sprite *sprite;
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources spriteResources;
    EncounterEffectQuadInterp slide;
    EncounterEffectHBlankWave wave;
    Window window;
    VecFx32 position;
    int timer;
    BOOL waveActive;
} RocketExecutiveEffectWork;

void RocketEffect_LoadBgScreen(EncounterEffectTask *effect, u8 bgId, int member);
String *RocketEffect_GetTrainerNameString(u32 trainerId, enum HeapID heapID);
void RocketEffect_RunExecutiveIntro(SysTask *task, EncounterEffectTask *effect, const RocketExecutiveIntroParams *params);

void EncounterEffect_LoadBgGraphics(NARC *narc, int member1, int member2, int member3, int palette, int paletteCount, BgConfig *bgConfig, int bgId);
void EncounterEffect_SetVecFx32(VecFx32 *vec, fx32 x, fx32 y, fx32 z);
void EncounterEffectHBlankWave_AlternateDirection(EncounterEffectHBlankWave *wave, int unused);

// One entry per grunt sprite, 8 words apart: start/end/velocity of x, start/end/velocity of y,
// delay before the sprite appears, rotation
extern const s32 ov117_0225FAF4[];
extern const s32 ov117_0225FAF8[];
extern const s32 ov117_0225FAFC[];
extern const s32 ov117_0225FB00[];
extern const s32 ov117_0225FB04[];
extern const s32 ov117_0225FB08[];
extern const s32 ov117_0225FB0C[];
extern const s32 ov117_0225FB10[];
extern const RocketExecutiveIntroParams _0225FACC;
extern const RocketExecutiveIntroParams ov117_0225FAD4;
extern const RocketExecutiveIntroParams ov117_0225FADC;
extern const RocketExecutiveIntroParams ov117_0225FAE4;
extern const RocketExecutiveIntroParams ov117_0225FAEC;

// Six "R" sprites fly in one after another, then the screen fades to black.
void EncounterEffect_TeamRocket(SysTask *task, EncounterEffectTask *effect) {
    VecFx32 position;
    VecFx32 scale;
    BOOL done;
    int i;
    TeamRocketEffectWork *work = effect->work;

    switch (effect->state) {
    case 0:
        effect->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(TeamRocketEffectWork));
        memset(effect->work, 0, sizeof(TeamRocketEffectWork));
        work = effect->work;
        EncounterEffect_InitSprites(&work->spriteManager, ROCKET_EFFECT_SPRITE_COUNT, 1);
        EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->spriteResources, 3, 1, 156, 158, 157, 600000);
        for (i = 0; i < ROCKET_EFFECT_SPRITE_COUNT; i++) {
            work->sprites[i] = EncounterEffect_CreateSprite(&work->spriteManager, &work->spriteResources, 0, 0, 0, 0);
            Sprite_SetDrawFlag(work->sprites[i], FALSE);
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        effect->state++;
        break;
    case 1:
        EncounterEffect_StartFlash(1, -16, -16, &effect->fadeDone, 2);
        effect->state++;
        break;
    case 2:
        if (effect->fadeDone) {
            effect->state++;
            work->next = 0;
            work->timer = ov117_0225FB0C[work->next * 8];
        }
        break;
    case 3:
        if (--work->timer < 0) {
            EncounterEffectQuadInterp_Init(&work->x[work->next], ov117_0225FAF4[work->next * 8], ov117_0225FAF8[work->next * 8], ov117_0225FAFC[work->next * 8], 8);
            EncounterEffectQuadInterp_Init(&work->y[work->next], ov117_0225FB00[work->next * 8], ov117_0225FB04[work->next * 8], ov117_0225FB08[work->next * 8], 8);
            EncounterEffectQuadInterp_Init(&work->scale[work->next], FX32_CONST(2), 41, -0x666, 8);
            EncounterEffectInterp_Init(&work->rotation[work->next], 0, ov117_0225FB10[work->next * 8], 8);
            Sprite_SetDrawFlag(work->sprites[work->next], TRUE);
            EncounterEffect_SetVecFx32(&position, ov117_0225FAF4[work->next * 8], ov117_0225FB00[work->next * 8], 0);
            Sprite_SetMatrix(work->sprites[work->next], &position);
            EncounterEffect_SetVecFx32(&scale, FX32_CONST(2), FX32_CONST(2), 0);
            Sprite_SetScaleAndAffineType(work->sprites[work->next], &scale, 2);
            work->active[work->next] = TRUE;
            work->next++;
            if (work->next >= ROCKET_EFFECT_SPRITE_COUNT) {
                effect->state++;
            } else {
                work->timer = ov117_0225FB0C[work->next * 8];
            }
        }
        break;
    case 4:
        if (work->active[ROCKET_EFFECT_SPRITE_COUNT - 1] == FALSE) {
            effect->state++;
        }
        break;
    case 5:
        HBlankSystem_Stop(effect->fieldSystem->mapSystems->hBlankSystem);
        BeginNormalPaletteFade(3, 34, 0, RGB_BLACK, 12, 1, HEAP_ID_FIELD1);
        effect->state++;
        break;
    case 6:
        if (IsPaletteFadeFinished()) {
            effect->state++;
        }
        break;
    case 7:
        SetMasterBrightnessByColor(1, RGB_BLACK);
        HBlankSystem_Start(effect->fieldSystem->mapSystems->hBlankSystem);
        if (effect->done != NULL) {
            *effect->done = TRUE;
        }
        for (i = 0; i < ROCKET_EFFECT_SPRITE_COUNT; i++) {
            Sprite_Delete(work->sprites[i]);
        }
        EncounterEffect_FreeSpriteResources(&work->spriteManager, &work->spriteResources);
        EncounterEffect_FreeSprites(&work->spriteManager);
        EncounterEffect_Finish(effect, task);
        break;
    }
    for (i = 0; i < ROCKET_EFFECT_SPRITE_COUNT; i++) {
        if (work->active[i] == TRUE) {
            done = EncounterEffectQuadInterp_Update(&work->x[i]);
            EncounterEffectQuadInterp_Update(&work->y[i]);
            EncounterEffectQuadInterp_Update(&work->scale[i]);
            EncounterEffectInterp_Update(&work->rotation[i]);
            if (done) {
                work->active[i] = FALSE;
                Sprite_SetDrawFlag(work->sprites[i], FALSE);
            }
            EncounterEffect_SetVecFx32(&position, work->x[i].value, work->y[i].value, 0);
            Sprite_SetMatrix(work->sprites[i], &position);
            EncounterEffect_SetVecFx32(&scale, work->scale[i].value, work->scale[i].value, 0);
            Sprite_SetAffineScale(work->sprites[i], &scale);
            Sprite_SetAffineZRotation(work->sprites[i], (u16)work->rotation[i].value);
        }
    }
    if (effect->state != 7) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
}

void RocketEffect_LoadBgScreen(EncounterEffectTask *effect, u8 bgId, int member) {
    NNSG2dScreenData *screen;
    void *file = GfGfxLoader_GetScrnDataFromOpenNarc(effect->narc, member, FALSE, &screen, HEAP_ID_FIELD1);

    LoadRectToBgTilemapRect(effect->fieldSystem->bgConfig, bgId, screen->rawData, 0, 0, (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3));
    Heap_Free(file);
    BgCommitTilemapBufferToVram(effect->fieldSystem->bgConfig, bgId);
}

String *RocketEffect_GetTrainerNameString(u32 trainerId, enum HeapID heapID) {
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

void EncounterEffect_ExecutivePetrel(SysTask *task, EncounterEffectTask *effect) {
    RocketEffect_RunExecutiveIntro(task, effect, &_0225FACC);
}

void EncounterEffect_ExecutiveProton(SysTask *task, EncounterEffectTask *effect) {
    RocketEffect_RunExecutiveIntro(task, effect, &ov117_0225FAD4);
}

void EncounterEffect_ExecutiveArcher(SysTask *task, EncounterEffectTask *effect) {
    RocketEffect_RunExecutiveIntro(task, effect, &ov117_0225FADC);
}

void EncounterEffect_ExecutiveAriana(SysTask *task, EncounterEffectTask *effect) {
    RocketEffect_RunExecutiveIntro(task, effect, &ov117_0225FAE4);
}

void EncounterEffect_RocketBoss(SysTask *task, EncounterEffectTask *effect) {
    RocketEffect_RunExecutiveIntro(task, effect, &ov117_0225FAEC);
}

// The trainer's picture slides in behind a window that opens from the middle of the screen,
// with the trainer's name in a text window.
// force_active only sets a section flag; it is kept so the objects stay identical to the matched ones
#pragma force_active on
#pragma opt_propagation off
void RocketEffect_RunExecutiveIntro(SysTask *task, EncounterEffectTask *effect, const RocketExecutiveIntroParams *params) {
    enum HeapID heapID = HEAP_ID_FIELD1;
    RocketExecutiveEffectWork *work = effect->work;

    switch (effect->state) {
    case 0: {
        String *string;

        effect->work = Heap_Alloc(heapID, sizeof(RocketExecutiveEffectWork));
        memset(effect->work, 0, sizeof(RocketExecutiveEffectWork));
        work = effect->work;
        EncounterEffect_InitSprites(&work->spriteManager, 1, 1);
        EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->spriteResources, params->spriteMembers[0], 1, params->spriteMembers[1], params->spriteMembers[2], params->spriteMembers[3], 600000);
        work->sprite = EncounterEffect_CreateSprite(&work->spriteManager, &work->spriteResources, FX32_CONST(320), FX32_CONST(128), 0, 0);
        Sprite_SetDrawFlag(work->sprite, FALSE);
        Sprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
        Sprite_SetPriority(work->sprite, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        work->position.x = FX32_CONST(320);
        work->position.y = FX32_CONST(128);
        work->position.z = 0;
        G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 12, 4);
        SetBgPriority(GF_BG_LYR_MAIN_2, 0);
        SetBgPriority(GF_BG_LYR_MAIN_3, 1);
        SetBgPriority(GF_BG_LYR_MAIN_1, 2);
        SetBgPriority(GF_BG_LYR_MAIN_0, 3);
        ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_OFF);
        ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_OFF);
        ToggleBgLayer(GF_BG_LYR_MAIN_3, GF_PLANE_TOGGLE_OFF);
        EncounterEffect_LoadBgGraphics(effect->narc, 217, 216, 215, 0, 2, effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1);
        GfGfxLoader_GXLoadPalFromOpenNarc(effect->narc, 16, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_2_OFFSET, 32, heapID);
        AddWindowParameterized(effect->fieldSystem->bgConfig, &work->window, GF_BG_LYR_MAIN_2, 0, 20, 16, 2, 2, 1);
        FillWindowPixelBuffer(&work->window, 0);
        string = RocketEffect_GetTrainerNameString(params->trainerId, heapID);
        AddTextPrinterParameterizedWithColor(&work->window, 0, string, 0, 0, 0, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(string);
        EncounterEffect_LoadBgGraphics(effect->narc, 218, 216, 215, 0, 2, effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3);
        {
            G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG3, TRUE);
            G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_OBJ, TRUE);
            G2_SetWnd0Position(0, 96, 255, 96);
        }
        EncounterEffectHBlankWave_Init(&work->wave, HEAP_ID_FIELD1);
        EncounterEffect_StartFlash(1, 16, -16, &effect->fadeDone, 1);
        effect->state++;
        break;
    }
    case 1:
        ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_ON);
        work->timer = 12;
        effect->state++;
        // fallthrough
    case 2:
        BgSetPosTextAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_ADD_X, 24);
        if (--work->timer <= 0) {
            effect->state++;
        }
        break;
    case 3:
        reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xFFFF1FFF) | REG_GX_DISPCNT_W0_MASK;
        ToggleBgLayer(GF_BG_LYR_MAIN_3, GF_PLANE_TOGGLE_ON);
        EncounterEffectQuadInterp_Init(&work->slide, 0, FX32_CONST(96), FX32_CONST(4), 4);
        effect->state++;
        // fallthrough
    case 4:
        BgSetPosTextAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_ADD_X, 24);
        if (!EncounterEffectQuadInterp_Update(&work->slide)) {
            int height = work->slide.value >> FX32_SHIFT;

            reg_G2_WIN0H = 255;
            reg_G2_WIN0V = (((96 - height) << 8) & 0xFF00) | ((u8)(height + 96));
        } else {
            reg_GX_DISPCNT &= 0xFFFF1FFF;
            BgSetPosTextAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
            work->timer = 13;
            effect->state++;
        }
        break;
    case 5:
        if (--work->timer <= 0) {
            effect->state++;
        }
        break;
    case 6:
        RocketEffect_LoadBgScreen(effect, GF_BG_LYR_MAIN_1, 221);
        SetBgPriority(GF_BG_LYR_MAIN_1, 1);
        SetBgPriority(GF_BG_LYR_MAIN_3, 2);
        G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 0, 16);
        Sprite_SetDrawFlag(work->sprite, TRUE);
        work->timer = 0;
        effect->state++;
        // fallthrough
    case 7: {
        int blendDone = FALSE;

        if (work->timer <= 16) {
            work->timer += 4;
            if (work->timer > 16) {
                work->timer = 16;
                blendDone = TRUE;
            }
            G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, work->timer, 16 - work->timer);
        }
        work->position.x += (FX32_CONST(128) - work->position.x) * 2 / 3;
        if ((work->position.x >> FX32_SHIFT) <= 130) {
            work->position.x = FX32_CONST(128);
            if (blendDone == TRUE) {
                StartBrightnessTransition(8, 0, 16, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ), 1);
                BG_SetMaskColor(GF_BG_LYR_MAIN_0, RGB(5, 5, 5));
                ToggleBgLayer(GF_BG_LYR_MAIN_0, GF_PLANE_TOGGLE_OFF);
                effect->state++;
            }
        }
        if (work->timer > 4) {
            ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_ON);
        }
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, -((work->position.x >> FX32_SHIFT) + 8));
        Sprite_SetMatrix(work->sprite, &work->position);
        break;
    }
    case 8:
        if (IsBrightnessTransitionActive(1)) {
            work->timer = 16;
            effect->state++;
        }
        break;
    case 9:
        if (--work->timer <= 0) {
            EncounterEffectQuadInterp_Init(&work->slide, work->position.x, FX32_CONST(-96), FX32_CONST(2), 8);
            work->timer = 8;
            ToggleBgLayer(GF_BG_LYR_MAIN_3, GF_PLANE_TOGGLE_OFF);
            effect->state++;
        }
        break;
    case 10:
        work->position.x = work->slide.value;
        {
            int x = -((work->position.x >> FX32_SHIFT) + 8);

            if (x < 0) {
                ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, x);
            } else {
                ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_OFF);
            }
        }
        Sprite_SetMatrix(work->sprite, &work->position);
        if (EncounterEffectQuadInterp_Update(&work->slide)) {
            reg_GXS_DB_DISPCNT &= 0xFFFEFFFF;
            BG_SetMaskColor(GF_BG_LYR_SUB_0, RGB_BLACK);
            BeginNormalPaletteFade(0, 0, 0, RGB_WHITE, 4, 1, HEAP_ID_FIELD1);
            effect->state++;
        }
        break;
    case 11:
        if (IsPaletteFadeFinished()) {
            effect->state++;
        }
        break;
    case 12:
        if (effect->done) {
            *effect->done = TRUE;
        }
        EncounterEffectHBlankWave_Free(&work->wave);
        work->waveActive = FALSE;
        Sprite_Delete(work->sprite);
        EncounterEffect_FreeSpriteResources(&work->spriteManager, &work->spriteResources);
        EncounterEffect_FreeSprites(&work->spriteManager);
        RemoveWindow(&work->window);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 32, 0, HEAP_ID_FIELD1);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, HEAP_ID_FIELD1);
        BgClearTilemapBufferAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3);
        BgClearTilemapBufferAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1);
        EncounterEffect_Finish(effect, task);
        break;
    }
    if (work->waveActive == TRUE) {
        EncounterEffectHBlankWave_AlternateDirection(&work->wave, 2);
    }
    if (effect->state != 12) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
}
#pragma opt_propagation reset
#pragma force_active reset
