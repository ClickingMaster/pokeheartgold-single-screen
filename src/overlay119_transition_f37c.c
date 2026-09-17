#include "overlay119_transition_f37c_private.h"

void ov119_0225F37C(void *caller, Ov119TaskR2 *ctx) {
    u32 state = ctx->state;
    Ov119WorkF37C *work = (Ov119WorkF37C *)ctx->work;
    VecFx32 b0;
    VecFx32 b1;
    VecFx32 d0;
    VecFx32 d1;
    VecFx32 a0;
    VecFx32 a1;
    VecFx32 c0;
    VecFx32 c1;
    BOOL done;
    s32 distance;
    int i;

    switch (state) {
    case 0:
        ctx->work = (Ov119WorkR2 *)Heap_Alloc((enum HeapID)4, sizeof(Ov119WorkF37C));
        memset(ctx->work, 0, sizeof(Ov119WorkF37C));
        work = (Ov119WorkF37C *)ctx->work;
        work->camera = *(Camera **)((u8 *)ctx->fieldSystem + 0x24);
        work->effect = ov01_021F0250();
        ov01_021F05C4(work->spriteResources, 2, 1);
        ov01_021F0614(ctx->spriteManager, work->spriteResources, &work->resource168, 0, 1, 4, 6, 5, 0x927C0);
        for (i = 0; i < 2; i++) {
            work->sprites[i] = ov01_021F0718(work->spriteResources, &work->resource168, 0x80000, 0, 0, 0);
            Sprite_SetDrawFlag(work->sprites[i], FALSE);
            Sprite_SetAffineOverwriteMode(work->sprites[i], 2);
        }
        GfGfx_EngineATogglePlanes(0x10, TRUE);
        ctx->state++;
        break;
    case 1:
        ov01_021EFCF8(1, 16, -16, &ctx->done, 2);
        ctx->state++;
        break;
    case 2:
        if (ctx->done != 0) {
            ctx->state++;
        }
        break;
    case 3:
        ov01_021EFE70(&work->position, (s32)0xFFF40000, 0xC0000, 8);
        Sprite_SetDrawFlag(work->sprites[0], TRUE);
        Sprite_SetDrawFlag(work->sprites[1], TRUE);
        ov01_021F074C(&a0, 0x80000 - work->position.value, 0x40000, 0);
        b0 = a0;
        ov01_021F074C(&a1, 0x80000 + work->position.value, 0x80000, 0);
        b1 = a1;
        Sprite_SetMatrix(work->sprites[0], &b0);
        Sprite_SetMatrix(work->sprites[1], &b1);
        ov01_021EFE34(&work->rotation, 0, 0x1FFFE, 8);
        ctx->state++;
        break;
    case 4:
        done = ov01_021EFE80(&work->position);
        ov01_021F074C(&c0, 0x80000 - work->position.value, 0x40000, 0);
        d0 = c0;
        ov01_021F074C(&c1, 0x80000 + work->position.value, 0x80000, 0);
        d1 = c1;
        Sprite_SetMatrix(work->sprites[0], &d0);
        Sprite_SetMatrix(work->sprites[1], &d1);
        ov01_021EFE44(&work->rotation);
        Sprite_SetAffineZRotation(work->sprites[0], (u16)work->rotation.value);
        Sprite_SetAffineZRotation(work->sprites[1], (u16)-work->rotation.value);
        if (done == TRUE) {
            ctx->state++;
        }
        break;
    case 5:
        ov01_021F02C4(ctx, work->effect, 8, 0x1000, 0x1000);
        distance = Camera_GetDistance(work->camera);
        ov01_021EFEC8(&work->cameraInterp, distance, distance - (125 << 14), -0xA000, 8);
        ctx->state++;
        break;
    case 6:
        ov01_021EFF28(&work->cameraInterp);
        Camera_SetDistance(work->cameraInterp.value, work->camera);
        if (ov01_021EFE30(ctx) == TRUE) {
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
        ov01_021F06EC(work->spriteResources, &work->resource168);
        ov01_021F05F4(work->spriteResources);
        ov01_021F029C(work->effect);
        ov01_021EFCDC(ctx, caller);
        break;
    }
    if (ctx->state != 7) {
        SpriteList_RenderAndAnimateSprites(*(SpriteList **)&work->spriteResources[0]);
    }
}
