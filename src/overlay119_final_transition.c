#include "overlay119_final_transition_private.h"

void ov119_02260258(void *caller, Ov119Task *ctx) {
    Ov119TailWork *work = ctx->work;
    VecFx32 matrix;
    VecFx32 current;
    u16 color[2];
    BOOL done;
    s32 distance;
    int i;

    switch (ctx->state) {
    case 0:
        ctx->work = Heap_Alloc((enum HeapID)4, sizeof(Ov119TailWork));
        memset(ctx->work, 0, sizeof(Ov119TailWork));
        work = ctx->work;
        work->camera = *(Camera **)((u8 *)ctx->fieldSystem + 0x24);
        ov01_021F05C4(work->spriteResources, 3, 1);
        ov01_021F0614(ctx->spriteManager, work->spriteResources, &work->resource13c, 0, 1, 4, 6, 5, 0x927c0);
        for (i = 0; i < 3; i++) {
            work->sprites[i] = ov01_021F0718(work->spriteResources, &work->resource13c, 0, -32, 0, 0);
            Sprite_SetDrawFlag(work->sprites[i], FALSE);
        }
        GfGfx_EngineATogglePlanes(0x10, TRUE);
        work->window = AllocWindows((enum HeapID)4, 1);
        AddWindowParameterized(*(BgConfig **)((u8 *)ctx->fieldSystem + 8), work->window, 3, 0, 0, 32, 32, 0, 0);
        color[0] = 0;
        BG_LoadPlttData(3, color, 2, 0x1e);
        FillWindowPixelBuffer(work->window, 0);
        ScheduleWindowCopyToVram(work->window);
        work->effect = ov01_021F09BC((enum HeapID)4);
        ctx->state++;
        break;
    case 1:
        ov01_021EFCF8(1, 16, -16, &ctx->done, 2);
        ctx->state++;
        break;
    case 2:
        if (ctx->done != 0) {
            ctx->state++;
            work->timer = 0;
        }
        break;
    case 3:
        if (--work->timer <= 0) {
            ov01_021EFE34(&work->position[0], -32, 224, 5);
            ov01_021F074C(&matrix, 0x80000, -0x20000, 0);
            Sprite_SetMatrix(work->sprites[0], &matrix);
            Sprite_SetDrawFlag(work->sprites[0], TRUE);
            ov01_021EFE34(&work->rotation[0], 0, 0xffff, 5);
            Sprite_SetAffineOverwriteMode(work->sprites[0], 2);
            work->active[0] = 1;
            ctx->state++;
            work->timer = 1;
        }
        break;
    case 4:
        if (--work->timer <= 0) {
            ov01_021EFE34(&work->position[1], -32, 224, 5);
            ov01_021F074C(&matrix, 0xd0000, -0x20000, 0);
            Sprite_SetMatrix(work->sprites[1], &matrix);
            Sprite_SetDrawFlag(work->sprites[1], TRUE);
            ov01_021EFE34(&work->rotation[1], 0, 0xffff0001, 5);
            Sprite_SetAffineOverwriteMode(work->sprites[1], 2);
            work->active[1] = 1;
            ctx->state++;
            work->timer = 3;
        }
        break;
    case 5:
        if (--work->timer <= 0) {
            ov01_021EFE34(&work->position[2], -32, 224, 5);
            ov01_021F074C(&matrix, 0x30000, -0x20000, 0);
            Sprite_SetMatrix(work->sprites[2], &matrix);
            Sprite_SetDrawFlag(work->sprites[2], TRUE);
            ov01_021EFE34(&work->rotation[2], 0, 0xffff, 5);
            Sprite_SetAffineOverwriteMode(work->sprites[2], 2);
            work->active[2] = 1;
            ctx->state++;
        }
        break;
    case 6:
        if (work->active[0] == 0 && work->active[1] == 0 && work->active[2] == 0) {
            for (i = 0; i < 3; i++) {
                Sprite_SetDrawFlag(work->sprites[i], FALSE);
            }
            ctx->state++;
        }
        break;
    case 7:
        ov01_021F0A0C(work->effect, 1, 1, work->window, 15);
        distance = Camera_GetDistance(work->camera);
        ov01_021EFEC8(&work->cameraInterp, distance, distance - (125 << 15), 0xa000, 0x40);
        ctx->state++;
        break;
    case 8:
        done = ov01_021F0A4C(work->effect);
        ScheduleWindowCopyToVram(work->window);
        ov01_021EFF28(&work->cameraInterp);
        Camera_SetDistance(work->cameraInterp.value, work->camera);
        if (done == TRUE) {
            ctx->state++;
        }
        break;
    case 9:
        sub_0200FC20(0);
        if (ctx->completionFlag != NULL) {
            *ctx->completionFlag = 1;
        }
        for (i = 0; i < 3; i++) {
            Sprite_Delete(work->sprites[i]);
        }
        ov01_021F06EC(work->spriteResources, &work->resource13c);
        ov01_021F05F4(work->spriteResources);
        ov01_021F09EC(work->effect);
        ClearWindowTilemapAndCopyToVram(work->window);
        RemoveWindow(work->window);
        WindowArray_Delete(work->window, 1);
        BG_ClearCharDataRange(3, 0x20, 0, (enum HeapID)4);
        BgClearTilemapBufferAndCommit(*(BgConfig **)((u8 *)ctx->fieldSystem + 8), 3);
        ov01_021EFCDC(ctx, caller);
        break;
    }

    for (i = 0; i < 3; i++) {
        if (work->active[i] == 1) {
            done = ov01_021EFE44(&work->position[i]);
            ov01_021EFE44(&work->rotation[i]);
            if (done) {
                work->active[i] = 0;
            }
            current = *Sprite_GetMatrixPtr(work->sprites[i]);
            current.y = work->position[i].value << 12;
            Sprite_SetMatrix(work->sprites[i], &current);
            Sprite_SetAffineZRotation(work->sprites[i], (u16)work->rotation[i].value);
        }
    }
    if (ctx->state != 9) {
        SpriteList_RenderAndAnimateSprites(*(SpriteList **)&work->spriteResources[0]);
    }
}
