#include "global.h"

#include "field/encounter_effect.h"

#include "gf_gfx_planes.h"
#include "heap.h"
#include "screen_fade_legacy.h"
#include "sprite.h"

typedef struct {
    EncounterEffectInterp fade;
    EncounterEffectQuadInterp scale;
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources spriteResources;
    Sprite *sprite;
} FrontierOrLinkSingleEffectWork;

typedef struct {
    EncounterEffectSpriteManager spriteManager;
    EncounterEffectSpriteResources spriteResources;
    Sprite *sprites[4];
    EncounterEffectQuadInterp horiz;
    EncounterEffectQuadInterp vert;
} DoubleEffectWork;

enum FrontierSingleEffectState {
    FRONTIER_SINGLE_STATE_LOAD,
    FRONTIER_SINGLE_STATE_START_FLASH,
    FRONTIER_SINGLE_STATE_WAIT_FLASH,
    FRONTIER_SINGLE_STATE_START_FADE_IN,
    FRONTIER_SINGLE_STATE_FADE_IN,
    FRONTIER_SINGLE_STATE_START_ZOOM,
    FRONTIER_SINGLE_STATE_ZOOM,
    FRONTIER_SINGLE_STATE_END,
};

void EncounterEffect_FrontierOrLinkSingle(SysTask *task, EncounterEffectTask *ctx) {
    VecFx32 scale5;
    VecFx32 scale6;
    VecFx32 tmp5;
    VecFx32 tmp6;
    BOOL done;
    FrontierOrLinkSingleEffectWork *work = ctx->work;

    switch (ctx->state) {
    case FRONTIER_SINGLE_STATE_LOAD:
        ctx->work = Heap_Alloc(HEAP_ID_FIELD1, 0x1a4);
        memset(ctx->work, 0, 0x1a4);
        work = ctx->work;
        EncounterEffect_InitSprites(&work->spriteManager, 1, 1);
        EncounterEffect_LoadSpriteResources(ctx->narc, &work->spriteManager, &work->spriteResources, 0, 1, 7, 9, 8, 0x927c0);
        work->sprite = EncounterEffect_CreateSprite(&work->spriteManager, &work->spriteResources, 2 << 18, 6 << 16, 0, 0);
        Sprite_SetDrawFlag(work->sprite, FALSE);
        Sprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
        ctx->state++;
        break;
    case FRONTIER_SINGLE_STATE_START_FLASH:
        EncounterEffect_StartFlash(1, 16, -16, &ctx->fadeDone, 2);
        ctx->state++;
        break;
    case FRONTIER_SINGLE_STATE_WAIT_FLASH:
        if (ctx->fadeDone != 0) {
            ctx->state++;
        }
        break;
    case FRONTIER_SINGLE_STATE_START_FADE_IN:
        EncounterEffectInterp_Init(&work->fade, 0, 16, 12);
        G2x_SetBlendAlpha_(0x04000050, 0, 15, work->fade.value, 16 - work->fade.value);
        Sprite_SetDrawFlag(work->sprite, TRUE);
        ctx->state++;
        break;
    case FRONTIER_SINGLE_STATE_FADE_IN:
        done = EncounterEffectInterp_Update(&work->fade);
        *(vu16 *)0x04000052 = (u16)(((16 - work->fade.value) << 8) | work->fade.value);
        if (done == TRUE) {
            *(vu16 *)0x04000050 = 0;
            Sprite_SetOamMode(work->sprite, GX_OAM_MODE_NORMAL);
            ctx->state++;
        }
        break;
    case FRONTIER_SINGLE_STATE_START_ZOOM:
        EncounterEffectQuadInterp_Init(&work->scale, 1 << 12, 0x19a, 1, 6);
        Sprite_SetAffineOverwriteMode(work->sprite, 2);
        EncounterEffect_SetVecFx32(&tmp5, work->scale.value, work->scale.value, work->scale.value);
        scale5 = tmp5;
        Sprite_SetAffineScale(work->sprite, &scale5);
        HBlankSystem_Stop(ctx->fieldSystem->mapSystems->hBlankSystem);
        BeginNormalPaletteFade(3, 0x10, 0, 0, 6, 1, HEAP_ID_FIELD1);
        ctx->state++;
        break;
    case FRONTIER_SINGLE_STATE_ZOOM:
        done = EncounterEffectQuadInterp_Update(&work->scale);
        EncounterEffect_SetVecFx32(&tmp6, work->scale.value, work->scale.value, work->scale.value);
        scale6 = tmp6;
        Sprite_SetAffineScale(work->sprite, &scale6);
        if (done == TRUE && IsPaletteFadeFinished() == TRUE) {
            ctx->state++;
        }
        break;
    case FRONTIER_SINGLE_STATE_END:
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, 0);
        HBlankSystem_Start(ctx->fieldSystem->mapSystems->hBlankSystem);
        if (ctx->done != NULL) {
            *ctx->done = TRUE;
        }
        Sprite_Delete(work->sprite);
        EncounterEffect_FreeSpriteResources(&work->spriteManager, &work->spriteResources);
        EncounterEffect_FreeSprites(&work->spriteManager);
        EncounterEffect_Finish(ctx, task);
        break;
    }

    if (ctx->state != FRONTIER_SINGLE_STATE_END) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
}

enum DoubleEffectState {
    DOUBLE_EFFECT_STATE_LOAD,
    DOUBLE_EFFECT_STATE_START_FLASH,
    DOUBLE_EFFECT_STATE_WAIT_FLASH,
    DOUBLE_EFFECT_STATE_START_SLIDE,
    DOUBLE_EFFECT_STATE_SLIDE,
    DOUBLE_EFFECT_STATE_START_FADE,
    DOUBLE_EFFECT_STATE_WAIT_FADE,
    DOUBLE_EFFECT_STATE_END,
};

void EncounterEffect_Double(SysTask *task, EncounterEffectTask *ctx) {
    VecFx32 matrix;
    BOOL done;
    int i;
    EncounterEffectQuadInterp *interp;
    DoubleEffectWork *work = ctx->work;

    switch (ctx->state) {
    case DOUBLE_EFFECT_STATE_LOAD:
        ctx->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(DoubleEffectWork));
        memset(ctx->work, 0, sizeof(DoubleEffectWork));
        work = ctx->work;
        EncounterEffect_InitSprites(&work->spriteManager, 4, 1);
        EncounterEffect_LoadSpriteResources(ctx->narc, &work->spriteManager, &work->spriteResources, 0, 1, 4, 6, 5, 0x927c0);
        for (i = 0; i < 4; i++) {
            work->sprites[i] = EncounterEffect_CreateSprite(&work->spriteManager, &work->spriteResources, 2 << 18, 6 << 16, 0, 0);
            Sprite_SetDrawFlag(work->sprites[i], FALSE);
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
        ctx->state++;
        break;
    case DOUBLE_EFFECT_STATE_START_FLASH:
        EncounterEffect_StartFlash(1, 16, -16, &ctx->fadeDone, 2);
        ctx->state++;
        break;
    case DOUBLE_EFFECT_STATE_WAIT_FLASH:
        if (ctx->fadeDone != 0) {
            ctx->state++;
        }
        break;
    case DOUBLE_EFFECT_STATE_START_SLIDE:
        EncounterEffectQuadInterp_Init(&work->horiz, 0, 2 << 18, 0x19a, 4);
        EncounterEffectQuadInterp_Init(&work->vert, 0, 10 << 16, 0x19a, 4);
        for (i = 0; i < 4; i++) {
            Sprite_SetDrawFlag(work->sprites[i], TRUE);
        }
        ctx->state++;
        break;
    case DOUBLE_EFFECT_STATE_SLIDE:
        i = 0;
        interp = &work->horiz;
        while (i < 2) {
            done = EncounterEffectQuadInterp_Update(interp);
            i++;
            interp++;
        }
        EncounterEffect_SetVecFx32(&matrix, 2 << 18, (6 << 16) - work->horiz.value, 0);
        Sprite_SetMatrix(work->sprites[0], &matrix);
        EncounterEffect_SetVecFx32(&matrix, 2 << 18, work->horiz.value + (6 << 16), 0);
        Sprite_SetMatrix(work->sprites[1], &matrix);
        EncounterEffect_SetVecFx32(&matrix, (2 << 18) - work->vert.value, 6 << 16, 0);
        Sprite_SetMatrix(work->sprites[2], &matrix);
        EncounterEffect_SetVecFx32(&matrix, work->vert.value + (2 << 18), 6 << 16, 0);
        Sprite_SetMatrix(work->sprites[3], &matrix);
        if (done == TRUE) {
            ctx->state++;
        }
        break;
    case DOUBLE_EFFECT_STATE_START_FADE:
        HBlankSystem_Stop(ctx->fieldSystem->mapSystems->hBlankSystem);
        BeginNormalPaletteFade(3, 0x22, 0, 0, 8, 1, HEAP_ID_FIELD1);
        ctx->state++;
        break;
    case DOUBLE_EFFECT_STATE_WAIT_FADE:
        if (IsPaletteFadeFinished() == TRUE) {
            ctx->state++;
        }
        break;
    case DOUBLE_EFFECT_STATE_END:
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, 0);
        HBlankSystem_Start(ctx->fieldSystem->mapSystems->hBlankSystem);
        if (ctx->done != NULL) {
            *ctx->done = TRUE;
        }
        for (i = 0; i < 4; i++) {
            Sprite_Delete(work->sprites[i]);
        }
        EncounterEffect_FreeSpriteResources(&work->spriteManager, &work->spriteResources);
        EncounterEffect_FreeSprites(&work->spriteManager);
        EncounterEffect_Finish(ctx, task);
        break;
    }

    if (ctx->state != DOUBLE_EFFECT_STATE_END) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
}
