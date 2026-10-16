#include "overlay114_scene_transition_private.h"

void EncounterEffect_FrontierOrLinkSingle(SysTask *task, EncounterEffectTask *ctx) {
    VecFx32 scale5;
    VecFx32 scale6;
    VecFx32 tmp5;
    VecFx32 tmp6;
    BOOL done;
    Ov114WorkA *work = ctx->work;

    switch (ctx->state) {
    case 0:
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
    case 1:
        EncounterEffect_StartFlash(1, 16, -16, &ctx->fadeDone, 2);
        ctx->state++;
        break;
    case 2:
        if (ctx->fadeDone != 0) {
            ctx->state++;
        }
        break;
    case 3:
        EncounterEffectInterp_Init(&work->fade, 0, 16, 12);
        G2x_SetBlendAlpha_(0x04000050, 0, 15, work->fade.value, 16 - work->fade.value);
        Sprite_SetDrawFlag(work->sprite, TRUE);
        ctx->state++;
        break;
    case 4:
        done = EncounterEffectInterp_Update(&work->fade);
        *(vu16 *)0x04000052 = (u16)(((16 - work->fade.value) << 8) | work->fade.value);
        if (done == TRUE) {
            *(vu16 *)0x04000050 = 0;
            Sprite_SetOamMode(work->sprite, GX_OAM_MODE_NORMAL);
            ctx->state++;
        }
        break;
    case 5:
        EncounterEffectQuadInterp_Init(&work->scale, 1 << 12, 0x19a, 1, 6);
        Sprite_SetAffineOverwriteMode(work->sprite, 2);
        EncounterEffect_SetVecFx32(&tmp5, work->scale.value, work->scale.value, work->scale.value);
        scale5 = tmp5;
        Sprite_SetAffineScale(work->sprite, &scale5);
        HBlankSystem_Stop(ctx->fieldSystem->mapSystems->hBlankSystem);
        BeginNormalPaletteFade(3, 0x10, 0, 0, 6, 1, HEAP_ID_FIELD1);
        ctx->state++;
        break;
    case 6:
        done = EncounterEffectQuadInterp_Update(&work->scale);
        EncounterEffect_SetVecFx32(&tmp6, work->scale.value, work->scale.value, work->scale.value);
        scale6 = tmp6;
        Sprite_SetAffineScale(work->sprite, &scale6);
        if (done == TRUE && IsPaletteFadeFinished() == TRUE) {
            ctx->state++;
        }
        break;
    case 7:
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

    if (ctx->state != 7) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
}

void EncounterEffect_Double(SysTask *task, EncounterEffectTask *ctx) {
    VecFx32 matrix;
    BOOL done;
    int i;
    EncounterEffectQuadInterp *interp;
    Ov114WorkB *work = ctx->work;

    switch (ctx->state) {
    case 0:
        ctx->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(Ov114WorkB));
        memset(ctx->work, 0, sizeof(Ov114WorkB));
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
    case 1:
        EncounterEffect_StartFlash(1, 16, -16, &ctx->fadeDone, 2);
        ctx->state++;
        break;
    case 2:
        if (ctx->fadeDone != 0) {
            ctx->state++;
        }
        break;
    case 3:
        EncounterEffectQuadInterp_Init(&work->horiz, 0, 2 << 18, 0x19a, 4);
        EncounterEffectQuadInterp_Init(&work->vert, 0, 10 << 16, 0x19a, 4);
        for (i = 0; i < 4; i++) {
            Sprite_SetDrawFlag(work->sprites[i], TRUE);
        }
        ctx->state++;
        break;
    case 4:
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
    case 5:
        HBlankSystem_Stop(ctx->fieldSystem->mapSystems->hBlankSystem);
        BeginNormalPaletteFade(3, 0x22, 0, 0, 8, 1, HEAP_ID_FIELD1);
        ctx->state++;
        break;
    case 6:
        if (IsPaletteFadeFinished() == TRUE) {
            ctx->state++;
        }
        break;
    case 7:
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

    if (ctx->state != 7) {
        SpriteList_RenderAndAnimateSprites(work->spriteManager.spriteList);
    }
}
