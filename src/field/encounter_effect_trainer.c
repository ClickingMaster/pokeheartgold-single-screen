#include "global.h"

#include "field/encounter_effect.h"

#include "bg_window.h"
#include "camera.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "sprite.h"
#include "unk_0200FA24.h"

// Trainer battle transitions (effects 6-11). Most of them zoom the field camera in while a
// couple of Poke Ball sprites spin over the screen.

typedef struct SpinZoomEffectWork {
    EncounterEffectQuadInterp scale;
    EncounterEffectInterp rotation;
    EncounterEffectQuadInterp movement;
    void *flash;
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources spriteResources;
    Sprite *sprites[2];
    Camera *camera;
    EncounterEffectQuadInterp cameraDistance;
    u32 unk_1DC;
} SpinZoomEffectWork;

typedef struct SpinApartEffectWork {
    EncounterEffectInterp position;
    EncounterEffectInterp rotation;
    void *flash;
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources spriteResources;
    Sprite *sprites[2];
    Camera *camera;
    EncounterEffectQuadInterp cameraDistance;
    u32 unk_1C0;
} SpinApartEffectWork;

typedef struct FadeSpinEffectWork {
    EncounterEffectQuadInterp scale;
    EncounterEffectInterp blend;
    EncounterEffectInterp rotation;
    EncounterEffectHBlankWave wave;
    BOOL waveActive;
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources spriteResources;
    Sprite *sprites[2];
    Camera *camera;
    EncounterEffectQuadInterp cameraDistance;
    int timer;
} FadeSpinEffectWork;

#define BALL_DROP_SPRITE_COUNT 3

typedef struct BallDropEffectWork {
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources spriteResources;
    Sprite *sprites[BALL_DROP_SPRITE_COUNT];
    EncounterEffectInterp y[BALL_DROP_SPRITE_COUNT];
    EncounterEffectInterp rotation[BALL_DROP_SPRITE_COUNT];
    void *trails[BALL_DROP_SPRITE_COUNT];
    BOOL active[BALL_DROP_SPRITE_COUNT];
    Window *window;
    EncounterEffectHBlankWave wave;
    BOOL waveActive;
    Camera *camera;
    EncounterEffectQuadInterp cameraDistance;
    int timer;
} BallDropEffectWork;

typedef struct RisingBallEffectWork {
    EncounterEffectQuadInterp y;
    EncounterEffectQuadInterp scaleX;
    EncounterEffectQuadInterp scaleY;
    EncounterEffectInterp rotation;
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources spriteResources;
    Sprite *sprite;
    Camera *camera;
    EncounterEffectQuadInterp cameraDistance;
    u32 unk_1EC;
} RisingBallEffectWork;

typedef struct BallFallEffectWork {
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources spriteResources;
    Sprite *sprites[BALL_DROP_SPRITE_COUNT];
    EncounterEffectInterp y[BALL_DROP_SPRITE_COUNT];
    EncounterEffectInterp rotation[BALL_DROP_SPRITE_COUNT];
    BOOL active[BALL_DROP_SPRITE_COUNT];
    Window *window;
    void *circleWipe;
    Camera *camera;
    EncounterEffectQuadInterp cameraDistance;
    s16 timer;
} BallFallEffectWork;

BOOL ov01_021EFE30(EncounterEffectTask *effect);
void EncounterEffectInterp_InitFx(EncounterEffectInterp *interp, s32 start, s32 end, s32 duration);
BOOL EncounterEffectInterp_UpdateFx(EncounterEffectInterp *interp);
void *StripeWipe_New(void);
void StripeWipe_Free(void *flash);
void StripeWipe_Start(EncounterEffectTask *effect, void *flash, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void *SplitWipe_New(void);
void SplitWipe_Free(void *flash);
void SplitWipe_Start(EncounterEffectTask *effect, void *flash, s32 arg2, s32 arg3, s32 arg4);
void EncounterEffect_SetVecFx32(VecFx32 *vec, fx32 x, fx32 y, fx32 z);
void *RectTrail_New(int heapID);
void RectTrail_Free(void *trail);
void RectTrail_Start(void *trail, int arg1, int arg2, int arg3, int arg4, int arg5, Window *window, int arg7, int arg8, int arg9);
BOOL RectTrail_Update(void *trail);
void *BlockWipe_New(enum HeapID heapID);
void BlockWipe_Free(void *circleWipe);
void BlockWipe_Start(void *circleWipe, u8 arg1, u8 arg2, Window *window, u8 fillColor);
BOOL BlockWipe_Update(void *circleWipe);
void EncounterEffectHBlankWave_AlternateDirection(EncounterEffectHBlankWave *wave, int unused);
void SetBothScreensBrightnessByColor(u16 color);

void EncounterEffect_TrainerSpinZoom(SysTask *task, EncounterEffectTask *effect) {
    SpinZoomEffectWork *work = effect->work;
    VecFx32 scale;
    VecFx32 left;
    VecFx32 right;
    VecFx32 leftPos;
    VecFx32 rightPos;
    BOOL done;
    s32 distance;
    int i;

    switch (effect->state) {
    case 0:
        effect->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(SpinZoomEffectWork));
        memset(effect->work, 0, sizeof(SpinZoomEffectWork));
        work = effect->work;
        work->camera = effect->fieldSystem->camera;
        work->flash = StripeWipe_New();
        EncounterEffect_InitSprites(&work->spriteManager, 2, 1);
        EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->spriteResources, 0, 1, 7, 9, 8, 600000);
        for (i = 0; i < 2; i++) {
            work->sprites[i] = EncounterEffect_CreateSprite(&work->spriteManager, &work->spriteResources, FX32_CONST(128), FX32_CONST(96), 0, 0);
            Sprite_SetDrawFlag(work->sprites[i], FALSE);
            Sprite_SetDrawPriority(work->sprites[i], i * 2);
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
        }
        break;
    case 3:
        EncounterEffectQuadInterp_Init(&work->scale, 41, FX32_ONE, 2, 10);
        EncounterEffect_SetVecFx32(&scale, work->scale.value, work->scale.value, work->scale.value);
        for (i = 0; i < 2; i++) {
            Sprite_SetDrawFlag(work->sprites[i], TRUE);
            Sprite_SetAffineOverwriteMode(work->sprites[i], 2);
            Sprite_SetAffineScale(work->sprites[i], &scale);
        }
        EncounterEffectInterp_Init(&work->rotation, 0, 0xFFFF, 10);
        effect->state++;
        break;
    case 4:
        done = EncounterEffectQuadInterp_Update(&work->scale);
        EncounterEffect_SetVecFx32(&scale, work->scale.value, work->scale.value, work->scale.value);
        EncounterEffectInterp_Update(&work->rotation);
        for (i = 0; i < 2; i++) {
            Sprite_SetAffineScale(work->sprites[i], &scale);
        }
        Sprite_SetAffineZRotation(work->sprites[0], (u16)work->rotation.value);
        Sprite_SetAffineZRotation(work->sprites[1], (u16)(work->rotation.value - 0x100));
        if (done == TRUE) {
            effect->state++;
        }
        break;
    case 5:
        StripeWipe_Start(effect, work->flash, 96, 6, 0, FX32_CONST(255), FX32_CONST(10));
        Sprite_SetAnimCtrlSeq(work->sprites[0], 1);
        Sprite_SetAnimCtrlSeq(work->sprites[1], 2);
        EncounterEffectQuadInterp_Init(&work->movement, 0, FX32_CONST(255), FX32_CONST(10), 6);
        distance = Camera_GetDistance(work->camera);
        EncounterEffectQuadInterp_Init(&work->cameraDistance, distance, distance - (125 << 14), -FX32_CONST(10), 6);
        Sprite_SetAffineZRotation(work->sprites[0], 0);
        Sprite_SetAffineZRotation(work->sprites[1], 0);
        effect->state++;
        break;
    case 6:
        EncounterEffectQuadInterp_Update(&work->movement);
        EncounterEffect_SetVecFx32(&leftPos, FX32_CONST(128) - work->movement.value, FX32_CONST(96), 0);
        left = leftPos;
        EncounterEffect_SetVecFx32(&rightPos, FX32_CONST(128) + work->movement.value, FX32_CONST(96), 0);
        right = rightPos;
        Sprite_SetMatrix(work->sprites[0], &left);
        Sprite_SetMatrix(work->sprites[1], &right);
        EncounterEffectQuadInterp_Update(&work->cameraDistance);
        Camera_SetDistance(work->cameraDistance.value, work->camera);
        if (ov01_021EFE30(effect)) {
            effect->state++;
        }
        break;
    case 7:
        SetBothScreensBrightnessByColor(RGB_BLACK);
        if (effect->done != NULL) {
            *effect->done = TRUE;
        }
        for (i = 0; i < 2; i++) {
            Sprite_Delete(work->sprites[i]);
        }
        EncounterEffect_FreeSpriteResources(&work->spriteManager, &work->spriteResources);
        EncounterEffect_FreeSprites(&work->spriteManager);
        StripeWipe_Free(work->flash);
        EncounterEffect_Finish(effect, task);
        break;
    }
    if (effect->state != 7) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
}

void EncounterEffect_TrainerSpinApart(SysTask *task, EncounterEffectTask *effect) {
    u32 state = effect->state;
    SpinApartEffectWork *work = effect->work;
    VecFx32 top0;
    VecFx32 bottom0;
    VecFx32 top1;
    VecFx32 bottom1;
    VecFx32 topPos0;
    VecFx32 bottomPos0;
    VecFx32 topPos1;
    VecFx32 bottomPos1;
    BOOL done;
    s32 distance;
    int i;

    switch (state) {
    case 0:
        effect->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(SpinApartEffectWork));
        memset(effect->work, 0, sizeof(SpinApartEffectWork));
        work = effect->work;
        work->camera = effect->fieldSystem->camera;
        work->flash = SplitWipe_New();
        EncounterEffect_InitSprites(&work->spriteManager, 2, 1);
        EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->spriteResources, 0, 1, 4, 6, 5, 600000);
        for (i = 0; i < 2; i++) {
            work->sprites[i] = EncounterEffect_CreateSprite(&work->spriteManager, &work->spriteResources, FX32_CONST(128), 0, 0, 0);
            Sprite_SetDrawFlag(work->sprites[i], FALSE);
            Sprite_SetAffineOverwriteMode(work->sprites[i], 2);
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        effect->state++;
        break;
    case 1:
        EncounterEffect_StartFlash(1, 16, -16, &effect->fadeDone, 2);
        effect->state++;
        break;
    case 2:
        if (effect->fadeDone) {
            effect->state++;
        }
        break;
    case 3:
        EncounterEffectInterp_InitFx(&work->position, -FX32_CONST(192), FX32_CONST(192), 8);
        Sprite_SetDrawFlag(work->sprites[0], TRUE);
        Sprite_SetDrawFlag(work->sprites[1], TRUE);
        EncounterEffect_SetVecFx32(&topPos0, FX32_CONST(128) - work->position.value, FX32_CONST(64), 0);
        top0 = topPos0;
        EncounterEffect_SetVecFx32(&bottomPos0, FX32_CONST(128) + work->position.value, FX32_CONST(128), 0);
        bottom0 = bottomPos0;
        Sprite_SetMatrix(work->sprites[0], &top0);
        Sprite_SetMatrix(work->sprites[1], &bottom0);
        EncounterEffectInterp_Init(&work->rotation, 0, 0x1FFFE, 8);
        effect->state++;
        break;
    case 4:
        done = EncounterEffectInterp_UpdateFx(&work->position);
        EncounterEffect_SetVecFx32(&topPos1, FX32_CONST(128) - work->position.value, FX32_CONST(64), 0);
        top1 = topPos1;
        EncounterEffect_SetVecFx32(&bottomPos1, FX32_CONST(128) + work->position.value, FX32_CONST(128), 0);
        bottom1 = bottomPos1;
        Sprite_SetMatrix(work->sprites[0], &top1);
        Sprite_SetMatrix(work->sprites[1], &bottom1);
        EncounterEffectInterp_Update(&work->rotation);
        Sprite_SetAffineZRotation(work->sprites[0], (u16)work->rotation.value);
        Sprite_SetAffineZRotation(work->sprites[1], (u16)-work->rotation.value);
        if (done == TRUE) {
            effect->state++;
        }
        break;
    case 5:
        SplitWipe_Start(effect, work->flash, 8, FX32_ONE, FX32_ONE);
        distance = Camera_GetDistance(work->camera);
        EncounterEffectQuadInterp_Init(&work->cameraDistance, distance, distance - (125 << 14), -FX32_CONST(10), 8);
        effect->state++;
        break;
    case 6:
        EncounterEffectQuadInterp_Update(&work->cameraDistance);
        Camera_SetDistance(work->cameraDistance.value, work->camera);
        if (ov01_021EFE30(effect) == TRUE) {
            effect->state++;
        }
        break;
    case 7:
        SetBothScreensBrightnessByColor(RGB_BLACK);
        if (effect->done != NULL) {
            *effect->done = TRUE;
        }
        for (i = 0; i < 2; i++) {
            Sprite_Delete(work->sprites[i]);
        }
        EncounterEffect_FreeSpriteResources(&work->spriteManager, &work->spriteResources);
        EncounterEffect_FreeSprites(&work->spriteManager);
        SplitWipe_Free(work->flash);
        EncounterEffect_Finish(effect, task);
        break;
    }
    if (effect->state != 7) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
}

void EncounterEffect_TrainerFadeSpin(SysTask *task, EncounterEffectTask *effect) {
    FadeSpinEffectWork *work = effect->work;
    VecFx32 scale5;
    VecFx32 scale6;
    VecFx32 scalePos5;
    VecFx32 scalePos6;
    BOOL done;
    s32 angle;
    int i;

    switch (effect->state) {
    case 0:
        effect->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(FadeSpinEffectWork));
        memset(effect->work, 0, sizeof(FadeSpinEffectWork));
        work = effect->work;
        work->camera = effect->fieldSystem->camera;
        EncounterEffectHBlankWave_Init(&work->wave, HEAP_ID_FIELD1);
        work->timer = 12;
        EncounterEffect_InitSprites(&work->spriteManager, 2, 1);
        EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->spriteResources, 0, 1, 7, 9, 8, 600000);
        for (i = 0; i < 2; i++) {
            work->sprites[i] = EncounterEffect_CreateSprite(&work->spriteManager, &work->spriteResources, FX32_CONST(128), FX32_CONST(96), 0, 0);
            Sprite_SetDrawFlag(work->sprites[i], FALSE);
            Sprite_SetDrawPriority(work->sprites[i], i);
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        effect->state++;
        break;
    case 1:
        EncounterEffect_StartFlash(1, -16, -16, &effect->fadeDone, 2);
        effect->state++;
        break;
    case 2:
        work->timer--;
        if (work->timer == 0) {
            EncounterEffectHBlankWave_Start(&work->wave, 0, 191, 682, FX32_CONST(12), 800, (void *)&reg_G2_BG0HOFS, 0, HEAP_ID_FIELD1);
            work->waveActive = TRUE;
        }
        if (effect->fadeDone) {
            effect->state++;
        }
        break;
    case 3:
        EncounterEffectInterp_Init(&work->blend, 0, 16, 8);
        G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, work->blend.value, 16 - work->blend.value);
        for (i = 0; i < 2; i++) {
            Sprite_SetDrawFlag(work->sprites[i], TRUE);
            Sprite_SetAffineOverwriteMode(work->sprites[i], 2);
            Sprite_SetOamMode(work->sprites[i], GX_OAM_MODE_XLU);
        }
        EncounterEffectInterp_Init(&work->rotation, 0, 0xFFFF, 8);
        effect->state++;
        break;
    case 4:
        done = EncounterEffectInterp_Update(&work->blend);
        reg_G2_BLDALPHA = (u16)(work->blend.value | ((16 - work->blend.value) << 8));
        angle = (u16)work->rotation.value;
        if (EncounterEffectInterp_Update(&work->rotation) == FALSE) {
            Sprite_SetAffineZRotation(work->sprites[0], (u16)work->rotation.value);
            Sprite_SetAffineZRotation(work->sprites[1], (u32)(angle << 16) >> 16);
        } else {
            Sprite_SetAffineZRotation(work->sprites[0], 0);
            Sprite_SetAffineZRotation(work->sprites[1], 0);
        }
        if (done != TRUE) {
            break;
        }
        reg_G2_BLDCNT = 0;
        for (i = 0; i < 2; i++) {
            Sprite_SetOamMode(work->sprites[i], GX_OAM_MODE_NORMAL);
        }
        effect->state++;
        break;
    case 5:
        EncounterEffectQuadInterp_Init(&work->scale, FX32_ONE, 41, 410, 8);
        EncounterEffect_SetVecFx32(&scalePos5, work->scale.value, work->scale.value, work->scale.value);
        scale5 = scalePos5;
        for (i = 0; i < 2; i++) {
            Sprite_SetAffineScale(work->sprites[i], &scale5);
        }
        {
            s32 distance = Camera_GetDistance(work->camera);
            EncounterEffectQuadInterp_Init(&work->cameraDistance, distance, distance - (125 << 14), -FX32_CONST(10), 8);
        }
        BeginNormalPaletteFade(3, 24, 0, RGB_BLACK, 8, 1, HEAP_ID_FIELD1);
        effect->state++;
        break;
    case 6:
        done = EncounterEffectQuadInterp_Update(&work->scale);
        EncounterEffect_SetVecFx32(&scalePos6, work->scale.value, work->scale.value, work->scale.value);
        scale6 = scalePos6;
        for (i = 0; i < 2; i++) {
            Sprite_SetAffineScale(work->sprites[i], &scale6);
        }
        EncounterEffectQuadInterp_Update(&work->cameraDistance);
        Camera_SetDistance(work->cameraDistance.value, work->camera);
        if (done == TRUE && IsPaletteFadeFinished() == TRUE) {
            effect->state++;
        }
        break;
    case 7:
        SetMasterBrightnessByColor(1, RGB_BLACK);
        if (effect->done != NULL) {
            *effect->done = TRUE;
        }
        EncounterEffectHBlankWave_Free(&work->wave);
        work->waveActive = FALSE;
        for (i = 0; i < 2; i++) {
            Sprite_Delete(work->sprites[i]);
        }
        EncounterEffect_FreeSpriteResources(&work->spriteManager, &work->spriteResources);
        EncounterEffect_FreeSprites(&work->spriteManager);
        EncounterEffect_Finish(effect, task);
        return;
    }
    if (work->waveActive == TRUE) {
        EncounterEffectHBlankWave_AlternateDirection(&work->wave, 2);
    }
    if (effect->state != 7) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
}

void EncounterEffect_TrainerBallDrop(SysTask *task, EncounterEffectTask *effect) {
    BallDropEffectWork *work = effect->work;
    VecFx32 position;
    VecFx32 matrix;
    u16 black;
    s32 distance;
    int i;

    switch (effect->state) {
    case 0:
        effect->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(BallDropEffectWork));
        memset(effect->work, 0, sizeof(BallDropEffectWork));
        work = effect->work;
        work->camera = effect->fieldSystem->camera;
        EncounterEffectHBlankWave_Init(&work->wave, HEAP_ID_FIELD1);
        work->timer = 14;
        EncounterEffect_InitSprites(&work->spriteManager, BALL_DROP_SPRITE_COUNT, 1);
        EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->spriteResources, 0, 1, 4, 6, 5, 600000);
        for (i = 0; i < BALL_DROP_SPRITE_COUNT; i++) {
            work->sprites[i] = EncounterEffect_CreateSprite(&work->spriteManager, &work->spriteResources, 0, 0, 0, 0);
            Sprite_SetDrawFlag(work->sprites[i], FALSE);
            Sprite_SetAffineOverwriteMode(work->sprites[i], 2);
            work->trails[i] = RectTrail_New(HEAP_ID_FIELD1);
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        work->window = AllocWindows(HEAP_ID_FIELD1, 1);
        AddWindowParameterized(effect->fieldSystem->bgConfig, work->window, GF_BG_LYR_MAIN_3, 0, 0, 32, 32, 0, 0);
        black = 0;
        BG_LoadPlttData(GF_BG_LYR_MAIN_3, &black, sizeof(black), 30);
        FillWindowPixelBuffer(work->window, 0);
        ScheduleWindowCopyToVram(work->window);
        effect->state++;
        break;
    case 1:
        EncounterEffect_StartFlash(1, 16, -16, &effect->fadeDone, 2);
        effect->state++;
        break;
    case 2:
        if (--work->timer == 0) {
            EncounterEffectHBlankWave_Start(&work->wave, 0, 191, 682, FX32_CONST(12), 800, (void *)&reg_G2_BG0HOFS, 0, HEAP_ID_FIELD1);
            work->waveActive = TRUE;
        }
        if (effect->fadeDone) {
            work->timer = 6;
            effect->state++;
        }
        break;
    case 3:
        if (--work->timer < 0) {
            distance = Camera_GetDistance(work->camera);
            EncounterEffectQuadInterp_Init(&work->cameraDistance, distance, distance - (125 << 14), -FX32_CONST(10), 16);
            EncounterEffectInterp_Init(&work->rotation[0], 0, 0xFFFF, 6);
            EncounterEffectInterp_Init(&work->y[0], 231, -32, 6);
            RectTrail_Start(work->trails[0], 43, 43, 312, 0, 6, work->window, 86, 64, 15);
            EncounterEffect_SetVecFx32(&position, FX32_CONST(43), FX32_CONST(231), 0);
            Sprite_SetMatrix(work->sprites[0], &position);
            Sprite_SetDrawFlag(work->sprites[0], TRUE);
            work->active[0] = TRUE;
            effect->state++;
            work->timer = 4;
        }
        break;
    case 4:
        if (--work->timer < 0) {
            EncounterEffectInterp_Init(&work->y[1], 231, -32, 6);
            EncounterEffectInterp_Init(&work->rotation[1], 0, -0xFFFF, 6);
            RectTrail_Start(work->trails[1], 215, 215, 312, 0, 6, work->window, 86, 64, 15);
            EncounterEffect_SetVecFx32(&position, FX32_CONST(215), FX32_CONST(231), 1);
            Sprite_SetMatrix(work->sprites[1], &position);
            Sprite_SetDrawFlag(work->sprites[1], TRUE);
            work->active[1] = TRUE;
            effect->state++;
            work->timer = 2;
        }
        break;
    case 5:
        if (--work->timer < 0) {
            EncounterEffectInterp_Init(&work->y[2], 231, -32, 6);
            EncounterEffectInterp_Init(&work->rotation[2], 0, 0xFFFF, 6);
            RectTrail_Start(work->trails[2], 129, 129, 312, 0, 6, work->window, 86, 64, 15);
            EncounterEffect_SetVecFx32(&position, FX32_CONST(129), FX32_CONST(231), 2);
            Sprite_SetMatrix(work->sprites[2], &position);
            Sprite_SetDrawFlag(work->sprites[2], TRUE);
            work->active[2] = TRUE;
            effect->state++;
        }
        break;
    case 6:
        EncounterEffectQuadInterp_Update(&work->cameraDistance);
        Camera_SetDistance(work->cameraDistance.value, work->camera);
        if (work->active[0] == FALSE && work->active[1] == FALSE && work->active[2] == FALSE) {
            effect->state++;
        }
        break;
    case 7:
        SetBothScreensBrightnessByColor(RGB_BLACK);
        if (effect->done != NULL) {
            *effect->done = TRUE;
        }
        EncounterEffectHBlankWave_Free(&work->wave);
        work->waveActive = FALSE;
        for (i = 0; i < BALL_DROP_SPRITE_COUNT; i++) {
            Sprite_Delete(work->sprites[i]);
            RectTrail_Free(work->trails[i]);
        }
        EncounterEffect_FreeSpriteResources(&work->spriteManager, &work->spriteResources);
        EncounterEffect_FreeSprites(&work->spriteManager);
        ClearWindowTilemapAndCopyToVram(work->window);
        RemoveWindow(work->window);
        WindowArray_Delete(work->window, 1);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 32, 0, HEAP_ID_FIELD1);
        BgClearTilemapBufferAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3);
        EncounterEffect_Finish(effect, task);
        return;
    }
    if (work->waveActive == TRUE) {
        EncounterEffectHBlankWave_AlternateDirection(&work->wave, 2);
    }
    for (i = 0; i < BALL_DROP_SPRITE_COUNT; i++) {
        if (work->active[i] == TRUE) {
            if (RectTrail_Update(work->trails[i])) {
                work->active[i] = FALSE;
            }
            EncounterEffectInterp_Update(&work->y[i]);
            EncounterEffectInterp_Update(&work->rotation[i]);
            Sprite_SetAffineZRotation(work->sprites[i], (u16)work->rotation[i].value);
            matrix = *Sprite_GetMatrixPtr(work->sprites[i]);
            matrix.y = work->y[i].value << FX32_SHIFT;
            Sprite_SetMatrix(work->sprites[i], &matrix);
        }
    }
    ScheduleWindowCopyToVram(work->window);
    if (effect->state != 7) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
}

void EncounterEffect_TrainerRisingBall(SysTask *task, EncounterEffectTask *effect) {
    VecFx32 scale;
    VecFx32 matrix;
    VecFx32 position;
    BOOL done;
    s32 distance;
    s32 x;
    RisingBallEffectWork *work = effect->work;

    switch (effect->state) {
    case 0:
        effect->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(RisingBallEffectWork));
        memset(effect->work, 0, sizeof(RisingBallEffectWork));
        work = effect->work;
        work->camera = effect->fieldSystem->camera;
        EncounterEffect_InitSprites(&work->spriteManager, 1, 1);
        EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->spriteResources, 0, 1, 4, 6, 5, 600000);
        work->sprite = EncounterEffect_CreateSprite(&work->spriteManager, &work->spriteResources, FX32_CONST(128), -FX32_CONST(32), 0, 0);
        Sprite_SetDrawFlag(work->sprite, FALSE);
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
        }
        break;
    case 3:
        EncounterEffectQuadInterp_Init(&work->y, 0, FX32_CONST(256), FX32_CONST(2), 12);
        Sprite_SetDrawFlag(work->sprite, TRUE);
        EncounterEffectQuadInterp_Init(&work->scaleX, 410, FX32_CONST(2), 0, 12);
        EncounterEffectQuadInterp_Init(&work->scaleY, 410, FX32_CONST(2), 0, 12);
        EncounterEffect_SetVecFx32(&scale, work->scaleX.value, work->scaleY.value, 0);
        Sprite_SetScaleAndAffineType(work->sprite, &scale, 2);
        EncounterEffectInterp_Init(&work->rotation, 0, 0xFFFF, 12);
        effect->state++;
        break;
    case 4:
        done = EncounterEffectQuadInterp_Update(&work->y);
        x = 2 << 18;
        EncounterEffect_SetVecFx32(&position, x, work->y.value - (x >> 2), 0);
        matrix = position;
        Sprite_SetMatrix(work->sprite, &matrix);
        EncounterEffectQuadInterp_Update(&work->scaleX);
        EncounterEffectQuadInterp_Update(&work->scaleY);
        EncounterEffect_SetVecFx32(&scale, work->scaleX.value, work->scaleY.value, 0);
        Sprite_SetAffineScale(work->sprite, &scale);
        EncounterEffectInterp_Update(&work->rotation);
        Sprite_SetAffineZRotation(work->sprite, (u16)work->rotation.value);
        if (done == TRUE) {
            Sprite_SetDrawFlag(work->sprite, FALSE);
            effect->state++;
        }
        break;
    case 5:
        HBlankSystem_Stop(effect->fieldSystem->unk4->hBlankSystem);
        distance = Camera_GetDistance(work->camera);
        EncounterEffectQuadInterp_Init(&work->cameraDistance, distance, distance - (250 << 14), FX32_CONST(10), 8);
        BeginNormalPaletteFade(3, 18, 0, RGB_BLACK, 8, 1, HEAP_ID_FIELD1);
        effect->state++;
        break;
    case 6:
        EncounterEffectQuadInterp_Update(&work->cameraDistance);
        Camera_SetDistance(work->cameraDistance.value, work->camera);
        if (IsPaletteFadeFinished()) {
            effect->state++;
        }
        break;
    case 7:
        SetMasterBrightnessByColor(1, RGB_BLACK);
        HBlankSystem_Start(effect->fieldSystem->unk4->hBlankSystem);
        if (effect->done != NULL) {
            *effect->done = TRUE;
        }
        Sprite_Delete(work->sprite);
        EncounterEffect_FreeSpriteResources(&work->spriteManager, &work->spriteResources);
        EncounterEffect_FreeSprites(&work->spriteManager);
        EncounterEffect_Finish(effect, task);
        break;
    }

    if (effect->state != 7) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
}

void EncounterEffect_TrainerBallFall(SysTask *task, EncounterEffectTask *effect) {
    BallFallEffectWork *work = effect->work;
    VecFx32 position;
    VecFx32 matrix;
    u16 black[2];
    BOOL done;
    s32 distance;
    int i;

    switch (effect->state) {
    case 0:
        effect->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(BallFallEffectWork));
        memset(effect->work, 0, sizeof(BallFallEffectWork));
        work = effect->work;
        work->camera = effect->fieldSystem->camera;
        EncounterEffect_InitSprites(&work->spriteManager, BALL_DROP_SPRITE_COUNT, 1);
        EncounterEffect_LoadSpriteResources(effect->narc, &work->spriteManager, &work->spriteResources, 0, 1, 4, 6, 5, 600000);
        for (i = 0; i < BALL_DROP_SPRITE_COUNT; i++) {
            work->sprites[i] = EncounterEffect_CreateSprite(&work->spriteManager, &work->spriteResources, 0, -32, 0, 0);
            Sprite_SetDrawFlag(work->sprites[i], FALSE);
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        work->window = AllocWindows(HEAP_ID_FIELD1, 1);
        AddWindowParameterized(effect->fieldSystem->bgConfig, work->window, GF_BG_LYR_MAIN_3, 0, 0, 32, 32, 0, 0);
        black[0] = 0;
        BG_LoadPlttData(GF_BG_LYR_MAIN_3, black, sizeof(black[0]), 30);
        FillWindowPixelBuffer(work->window, 0);
        ScheduleWindowCopyToVram(work->window);
        work->circleWipe = BlockWipe_New(HEAP_ID_FIELD1);
        effect->state++;
        break;
    case 1:
        EncounterEffect_StartFlash(1, 16, -16, &effect->fadeDone, 2);
        effect->state++;
        break;
    case 2:
        if (effect->fadeDone) {
            effect->state++;
            work->timer = 0;
        }
        break;
    case 3:
        if (--work->timer <= 0) {
            EncounterEffectInterp_Init(&work->y[0], -32, 224, 5);
            EncounterEffect_SetVecFx32(&position, FX32_CONST(128), -FX32_CONST(32), 0);
            Sprite_SetMatrix(work->sprites[0], &position);
            Sprite_SetDrawFlag(work->sprites[0], TRUE);
            EncounterEffectInterp_Init(&work->rotation[0], 0, 0xFFFF, 5);
            Sprite_SetAffineOverwriteMode(work->sprites[0], 2);
            work->active[0] = TRUE;
            effect->state++;
            work->timer = 1;
        }
        break;
    case 4:
        if (--work->timer <= 0) {
            EncounterEffectInterp_Init(&work->y[1], -32, 224, 5);
            EncounterEffect_SetVecFx32(&position, FX32_CONST(208), -FX32_CONST(32), 0);
            Sprite_SetMatrix(work->sprites[1], &position);
            Sprite_SetDrawFlag(work->sprites[1], TRUE);
            EncounterEffectInterp_Init(&work->rotation[1], 0, -0xFFFF, 5);
            Sprite_SetAffineOverwriteMode(work->sprites[1], 2);
            work->active[1] = TRUE;
            effect->state++;
            work->timer = 3;
        }
        break;
    case 5:
        if (--work->timer <= 0) {
            EncounterEffectInterp_Init(&work->y[2], -32, 224, 5);
            EncounterEffect_SetVecFx32(&position, FX32_CONST(48), -FX32_CONST(32), 0);
            Sprite_SetMatrix(work->sprites[2], &position);
            Sprite_SetDrawFlag(work->sprites[2], TRUE);
            EncounterEffectInterp_Init(&work->rotation[2], 0, 0xFFFF, 5);
            Sprite_SetAffineOverwriteMode(work->sprites[2], 2);
            work->active[2] = TRUE;
            effect->state++;
        }
        break;
    case 6:
        if (work->active[0] == FALSE && work->active[1] == FALSE && work->active[2] == FALSE) {
            for (i = 0; i < BALL_DROP_SPRITE_COUNT; i++) {
                Sprite_SetDrawFlag(work->sprites[i], FALSE);
            }
            effect->state++;
        }
        break;
    case 7:
        BlockWipe_Start(work->circleWipe, 1, 1, work->window, 15);
        distance = Camera_GetDistance(work->camera);
        EncounterEffectQuadInterp_Init(&work->cameraDistance, distance, distance - (125 << 15), FX32_CONST(10), 64);
        effect->state++;
        break;
    case 8:
        done = BlockWipe_Update(work->circleWipe);
        ScheduleWindowCopyToVram(work->window);
        EncounterEffectQuadInterp_Update(&work->cameraDistance);
        Camera_SetDistance(work->cameraDistance.value, work->camera);
        if (done == TRUE) {
            effect->state++;
        }
        break;
    case 9:
        SetBothScreensBrightnessByColor(RGB_BLACK);
        if (effect->done != NULL) {
            *effect->done = TRUE;
        }
        for (i = 0; i < BALL_DROP_SPRITE_COUNT; i++) {
            Sprite_Delete(work->sprites[i]);
        }
        EncounterEffect_FreeSpriteResources(&work->spriteManager, &work->spriteResources);
        EncounterEffect_FreeSprites(&work->spriteManager);
        BlockWipe_Free(work->circleWipe);
        ClearWindowTilemapAndCopyToVram(work->window);
        RemoveWindow(work->window);
        WindowArray_Delete(work->window, 1);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 32, 0, HEAP_ID_FIELD1);
        BgClearTilemapBufferAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3);
        EncounterEffect_Finish(effect, task);
        break;
    }

    for (i = 0; i < BALL_DROP_SPRITE_COUNT; i++) {
        if (work->active[i] == TRUE) {
            done = EncounterEffectInterp_Update(&work->y[i]);
            EncounterEffectInterp_Update(&work->rotation[i]);
            if (done) {
                work->active[i] = FALSE;
            }
            matrix = *Sprite_GetMatrixPtr(work->sprites[i]);
            matrix.y = work->y[i].value << FX32_SHIFT;
            Sprite_SetMatrix(work->sprites[i], &matrix);
            Sprite_SetAffineZRotation(work->sprites[i], (u16)work->rotation[i].value);
        }
    }
    if (effect->state != 9) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
}
