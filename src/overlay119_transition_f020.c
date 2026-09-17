#include "overlay119_transition_f020_private.h"

void ov119_0225F020(void *caller, Ov119TaskR2 *ctx) {
    Ov119WorkR2 *work = ctx->work;
    VecFx32 scale;
    VecFx32 left;
    VecFx32 right;
    VecFx32 srcLeft;
    VecFx32 srcRight;
    BOOL done;
    s32 distance;
    int i;

    switch (ctx->state) {
    case 0:
        ctx->work = Heap_Alloc((enum HeapID)4, sizeof(Ov119WorkR2));
        memset(ctx->work, 0, sizeof(Ov119WorkR2));
        work = ctx->work;
        work->camera = *(Camera **)((u8 *)ctx->fieldSystem + 0x24);
        work->effect = ov01_021EFFEC();
        ov01_021F05C4(work->spriteResources, 2, 1);
        ov01_021F0614(ctx->spriteManager, work->spriteResources, &work->resource184, 0, 1, 7, 9, 8, 0x927C0);
        for (i = 0; i < 2; i++) {
            work->sprites[i] = ov01_021F0718(work->spriteResources, &work->resource184, 0x80000, 0x60000, 0, 0);
            Sprite_SetDrawFlag(work->sprites[i], FALSE);
            Sprite_SetDrawPriority(work->sprites[i], i * 2);
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
        }
        break;
    case 3:
        ov01_021EFEC8(&work->scale, 0x29, 0x1000, 2, 10);
        ov01_021F074C(&scale, work->scale.value, work->scale.value, work->scale.value);
        for (i = 0; i < 2; i++) {
            Sprite_SetDrawFlag(work->sprites[i], TRUE);
            Sprite_SetAffineOverwriteMode(work->sprites[i], 2);
            Sprite_SetAffineScale(work->sprites[i], &scale);
        }
        ov01_021EFE34(&work->rotation, 0, 0xffff, 10);
        ctx->state++;
        break;
    case 4:
        done = ov01_021EFF28(&work->scale);
        ov01_021F074C(&scale, work->scale.value, work->scale.value, work->scale.value);
        ov01_021EFE44(&work->rotation);
        for (i = 0; i < 2; i++) {
            Sprite_SetAffineScale(work->sprites[i], &scale);
        }
        Sprite_SetAffineZRotation(work->sprites[0], (u16)work->rotation.value);
        Sprite_SetAffineZRotation(work->sprites[1], (u16)(work->rotation.value - 0x100));
        if (done == TRUE) {
            ctx->state++;
        }
        break;
    case 5:
        ov01_021F0050(ctx, work->effect, 0x60, 6, 0, 0xff000, 0xa000);
        Sprite_SetAnimCtrlSeq(work->sprites[0], 1);
        Sprite_SetAnimCtrlSeq(work->sprites[1], 2);
        ov01_021EFEC8(&work->movement, 0, 0xff000, 0xa000, 6);
        distance = Camera_GetDistance(work->camera);
        ov01_021EFEC8(&work->cameraInterp, distance, distance - (125 << 14), -0xa000, 6);
        Sprite_SetAffineZRotation(work->sprites[0], 0);
        Sprite_SetAffineZRotation(work->sprites[1], 0);
        ctx->state++;
        break;
    case 6:
        ov01_021EFF28(&work->movement);
        ov01_021F074C(&srcLeft, 0x80000 - work->movement.value, 0x60000, 0);
        left = srcLeft;
        ov01_021F074C(&srcRight, 0x80000 + work->movement.value, 0x60000, 0);
        right = srcRight;
        Sprite_SetMatrix(work->sprites[0], &left);
        Sprite_SetMatrix(work->sprites[1], &right);
        ov01_021EFF28(&work->cameraInterp);
        Camera_SetDistance(work->cameraInterp.value, work->camera);
        if (ov01_021EFE30(ctx)) {
            ctx->state++;
        }
        break;
    case 7:
        sub_0200FC20(0);
        if (ctx->completionFlag != NULL) {
            *ctx->completionFlag = 1;
        }
        for (i = 0; i < 2; i++) {
            Sprite_Delete(work->sprites[i]);
        }
        ov01_021F06EC(work->spriteResources, &work->resource184);
        ov01_021F05F4(work->spriteResources);
        ov01_021F0028(work->effect);
        ov01_021EFCDC(ctx, caller);
        break;
    }
    if (ctx->state != 7) {
        SpriteList_RenderAndAnimateSprites(*(SpriteList **)&work->spriteResources[0]);
    }
}
