#include "overlay117_sprite_burst_internal.h"
void ov117_0225F020(void *task, Ov117Task *ctx) {
    Triple matrix;
    Triple scale;
    BOOL done;
    int i;
    Ov117Work *work = ctx->work;
    switch (ctx->state) {
    case 0:
        ctx->work = Heap_Alloc(4, sizeof(Ov117Work));
        memset(ctx->work, 0, sizeof(Ov117Work));
        work = ctx->work;
        ov01_021F05C4(work->spriteResources, 6, 1);
        ov01_021F0614(ctx->spriteManager, work->spriteResources, &work->resource13c, 3, 1, 0x9c, 0x9e, 0x9d, 0x927c0);
        for (i = 0; i < 6; i++) {
            work->sprites170[i] = ov01_021F0718(work->spriteResources, &work->resource13c, 0, 0, 0, 0);
            Sprite_SetDrawFlag(work->sprites170[i], FALSE);
        }
        GfGfx_EngineATogglePlanes(0x10, TRUE);
        ctx->state++;
        break;
    case 1:
        ov01_021EFCF8(1, -16, -16, &ctx->done, 2);
        ctx->state++;
        break;
    case 2:
        if (ctx->done != 0) {
            ctx->state++;
            work->index3c8 = 0;
            work->timer3cc = ov117_0225FB0C[work->index3c8 * 8];
        }
        break;
    case 3:
        if (--work->timer3cc < 0) {
            ov01_021EFEC8(&work->interp218[work->index3c8].interp, ov117_0225FAF4[work->index3c8 * 8], ov117_0225FAF8[work->index3c8 * 8], ov117_0225FAFC[work->index3c8 * 8], 8);
            ov01_021EFEC8(&work->interp2a8[work->index3c8].interp, ov117_0225FB00[work->index3c8 * 8], ov117_0225FB04[work->index3c8 * 8], ov117_0225FB08[work->index3c8 * 8], 8);
            ov01_021EFEC8(&work->interp188[work->index3c8].interp, 0x2000, 0x29, -0x666, 8);
            ov01_021EFE34(&work->interp338[work->index3c8], 0, ov117_0225FB10[work->index3c8 * 8], 8);
            Sprite_SetDrawFlag(work->sprites170[work->index3c8], TRUE);
            ov01_021F074C(&matrix, ov117_0225FAF4[work->index3c8 * 8], ov117_0225FB00[work->index3c8 * 8], 0);
            Sprite_SetMatrix(work->sprites170[work->index3c8], &matrix);
            ov01_021F074C(&scale, 0x2000, 0x2000, 0);
            Sprite_SetScaleAndAffineType(work->sprites170[work->index3c8], &scale, 2);
            work->active3b0[work->index3c8] = 1;
            work->index3c8++;
            if (work->index3c8 >= 6) {
                ctx->state++;
            } else {
                work->timer3cc = ov117_0225FB0C[work->index3c8 * 8];
            }
        }
        break;
    case 4:
        if (work->active3b0[5] == 0) {
            ctx->state++;
        }
        break;
    case 5:
        HBlankSystem_Stop(*(void **)((u8 *)*(void **)((u8 *)ctx->fieldSystem + 4) + 0x1c));
        BeginNormalPaletteFade(3, 0x22, 0, 0, 12, 1, 4);
        ctx->state++;
        break;
    case 6:
        if (IsPaletteFadeFinished()) {
            ctx->state++;
        }
        break;
    case 7:
        sub_0200FBF4(1, 0);
        HBlankSystem_Start(*(void **)((u8 *)*(void **)((u8 *)ctx->fieldSystem + 4) + 0x1c));
        if (ctx->completionFlag != NULL) {
            *ctx->completionFlag = 1;
        }
        for (i = 0; i < 6; i++) {
            Sprite_Delete(work->sprites170[i]);
        }
        ov01_021F06EC(work->spriteResources, &work->resource13c);
        ov01_021F05F4(work->spriteResources);
        ov01_021EFCDC(ctx, task);
        break;
    }
    for (i = 0; i < 6; i++) {
        if (work->active3b0[i] == 1) {
            done = ov01_021EFF28(&work->interp218[i].interp);
            ov01_021EFF28(&work->interp2a8[i].interp);
            ov01_021EFF28(&work->interp188[i].interp);
            ov01_021EFE44(&work->interp338[i]);
            if (done) {
                work->active3b0[i] = 0;
                Sprite_SetDrawFlag(work->sprites170[i], FALSE);
            }
            ov01_021F074C(&matrix, work->interp218[i].interp.value, work->interp2a8[i].interp.value, 0);
            Sprite_SetMatrix(work->sprites170[i], &matrix);
            ov01_021F074C(&scale, work->interp188[i].interp.value, work->interp188[i].interp.value, 0);
            Sprite_SetAffineScale(work->sprites170[i], &scale);
            Sprite_SetAffineZRotation(work->sprites170[i], (u16)work->interp338[i].value);
        }
    }
    if (ctx->state != 7) {
        SpriteList_RenderAndAnimateSprites(*(SpriteList **)&work->spriteResources[0]);
    }
}
