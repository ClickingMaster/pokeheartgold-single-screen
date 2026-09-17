#include "overlay119_transition_f670_private.h"

void ov119_0225F670(void *caller, Ov119TaskR2 *ctx) {
    Ov119WorkF670 *work = (Ov119WorkF670 *)ctx->work;
    VecFx32 vec5;
    VecFx32 vec6;
    VecFx32 src5;
    VecFx32 src6;
    BOOL done;
    s32 angle;
    int i;

    switch (ctx->state) {
    case 0:
        ctx->work = (Ov119WorkR2 *)Heap_Alloc((enum HeapID)4, sizeof(Ov119WorkF670));
        memset(ctx->work, 0, sizeof(Ov119WorkF670));
        work = (Ov119WorkF670 *)ctx->work;
        work->camera = *(Camera **)((u8 *)ctx->fieldSystem + 0x24);
        ov01_021F12B4(work->effect, 4);
        work->timer = 12;
        ov01_021F05C4(work->spriteResources, 2, 1);
        ov01_021F0614(ctx->spriteManager, work->spriteResources, &work->resource18c, 0, 1, 7, 9, 8, 0x927C0);
        for (i = 0; i < 2; i++) {
            work->sprites[i] = ov01_021F0718(work->spriteResources, &work->resource18c, 0x80000, 0x60000, 0, 0);
            Sprite_SetDrawFlag(work->sprites[i], FALSE);
            Sprite_SetDrawPriority(work->sprites[i], i);
        }
        GfGfx_EngineATogglePlanes(0x10, TRUE);
        ctx->state++;
        break;
    case 1:
        ov01_021EFCF8(1, -16, -16, &ctx->done, 2);
        ctx->state++;
        break;
    case 2:
        work->timer--;
        if (work->timer == 0) {
            ov01_021F12E8(work->effect, 0, 0xbf, 0x2aa, 0xC000, 0x320, 0x04000010, 0, 4);
            *(u32 *)((u8 *)work + 0x4c) = 1;
        }
        if (ctx->done != 0) {
            ctx->state++;
        }
        break;
    case 3:
        ov01_021EFE34(&work->blend, 0, 0x10, 8);
        G2x_SetBlendAlpha_(0x04000050, 0, 0xf, work->blend.value, 0x10 - work->blend.value);
        for (i = 0; i < 2; i++) {
            Sprite_SetDrawFlag(work->sprites[i], TRUE);
            Sprite_SetAffineOverwriteMode(work->sprites[i], 2);
            Sprite_SetOamMode(work->sprites[i], (GXOamMode)1);
        }
        ov01_021EFE34(&work->rotation, 0, 0xffff, 8);
        ctx->state++;
        break;
    case 4:
        done = ov01_021EFE44(&work->blend);
        *(volatile u16 *)0x04000052 = (u16)(work->blend.value | ((0x10 - work->blend.value) << 8));
        angle = (u16)work->rotation.value;
        if (ov01_021EFE44(&work->rotation) == FALSE) {
            Sprite_SetAffineZRotation(work->sprites[0], (u16)work->rotation.value);
            Sprite_SetAffineZRotation(work->sprites[1], (u32)(angle << 16) >> 16);
        } else {
            Sprite_SetAffineZRotation(work->sprites[0], 0);
            Sprite_SetAffineZRotation(work->sprites[1], 0);
        }
        if (done != TRUE) {
            break;
        }
        *(volatile u16 *)0x04000050 = 0;
        for (i = 0; i < 2; i++) {
            Sprite_SetOamMode(work->sprites[i], (GXOamMode)0);
        }
        ctx->state++;
        break;
    case 5:
        ov01_021EFEC8(&work->scale, 0x1000, 0x29, 0x19a, 8);
        ov01_021F074C(&src5, work->scale.value, work->scale.value, work->scale.value);
        vec5 = src5;
        for (i = 0; i < 2; i++) {
            Sprite_SetAffineScale(work->sprites[i], &vec5);
        }
        {
            s32 distance = Camera_GetDistance(work->camera);
            ov01_021EFEC8(&work->cameraInterp, distance, distance - (125 << 14), -0xA000, 8);
        }
        BeginNormalPaletteFade(3, 0x18, 0, 0, 8, 1, 4);
        ctx->state++;
        break;
    case 6:
        done = ov01_021EFF28(&work->scale);
        ov01_021F074C(&src6, work->scale.value, work->scale.value, work->scale.value);
        vec6 = src6;
        for (i = 0; i < 2; i++) {
            Sprite_SetAffineScale(work->sprites[i], &vec6);
        }
        ov01_021EFF28(&work->cameraInterp);
        Camera_SetDistance(work->cameraInterp.value, work->camera);
        if (done == TRUE && IsPaletteFadeFinished() == TRUE) {
            ctx->state++;
        }
        break;
    case 7:
        sub_0200FBF4(1, 0);
        if (ctx->completionFlag != NULL) {
            *ctx->completionFlag = 1;
        }
        ov01_021F12D0(work->effect);
        *(u32 *)((u8 *)work + 0x4c) = 0;
        for (i = 0; i < 2; i++) {
            Sprite_Delete(work->sprites[i]);
        }
        ov01_021F06EC(work->spriteResources, &work->resource18c);
        ov01_021F05F4(work->spriteResources);
        ov01_021EFCDC(ctx, caller);
        return;
    }
    if (*(u32 *)((u8 *)work + 0x4c) == 1) {
        ov01_021F1310(work->effect, 2);
    }
    if (ctx->state != 7) {
        SpriteList_RenderAndAnimateSprites(*(SpriteList **)&work->spriteResources[0]);
    }
}
