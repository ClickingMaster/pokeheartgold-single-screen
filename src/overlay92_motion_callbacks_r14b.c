#include "overlay92_motion_callbacks_r14b_private.h"

void ov92_022621A4(SysTask *task, Ov92Ctx *ctx) {
    Ov92Done4 f = ov92_02263E1C;
    fx32 x, y;
    fx32 newX, newY;
    s32 i;

    if (!IsPaletteFadeFinished() || ctx->unk17C->unk34 == 1) {
        SysTask_Destroy(task);
        return;
    }
    switch (ctx->state) {
    case 0:
        for (i = 0; i < ctx->count; i++) {
            ctx->motions[i].angle = 0;
            ctx->motions[i].baseX = 0x80;
            ctx->motions[i].baseY = 0xa0;
            ctx->motions[i].radiusX = 0x40;
            ctx->motions[i].radiusY = 0x18;
            ManagedSprite_SetDrawFlag(ctx->motions[i].sprite, TRUE);
            ctx->motions[i + 2].angle = 0;
            ctx->motions[i + 2].baseX = 0x80;
            ctx->motions[i + 2].baseY = 0xa0;
            ctx->motions[i + 2].radiusX = 0x40;
            ctx->motions[i + 2].radiusY = 0x18;
            ManagedSprite_SetDrawFlag(ctx->motions[i + 2].sprite, TRUE);
            ManagedSprite_GetPositionFxXYWithSubscreenOffset(ctx->motions[i].sprite, &x, &y, 1 << 20);
            newX = (ctx->motions[i].baseX << 12) + GF_SinDeg(ctx->motions[i].angle) * ctx->motions[i].radiusX;
            newY = (ctx->motions[i].baseY << 12) + GF_CosDeg(ctx->motions[i].angle) * ctx->motions[i].radiusY;
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(ctx->motions[i].sprite, newX, newY, 1 << 20);
            ManagedSprite_GetPositionFxXYWithSubscreenOffset(ctx->motions[i + 2].sprite, &x, &y, 1 << 20);
            newX = (ctx->motions[i + 2].baseX << 12) + GF_SinDeg(ctx->motions[i + 2].angle) * ctx->motions[i + 2].radiusX;
            newY = (ctx->motions[i + 2].baseY << 12) - GF_CosDeg(ctx->motions[i + 2].angle) * ctx->motions[i + 2].radiusY;
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(ctx->motions[i + 2].sprite, newX, newY, 1 << 20);
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ManagedSprite_SetDrawFlag(ctx->motions[1].sprite, FALSE);
        ctx->state++;
    case 1:
        f.done[0] = ov92_02261E88(&ctx->motions[2], ctx->dur1, 0, 3);
        f.done[1] = ov92_02261F60(&ctx->motions[3], ctx->dur1, 0, 2);
        f.done[2] = ov92_02262018(&ctx->motions[0], ctx->dur2, 1, 3);
        f.done[3] = TRUE;
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ManagedSprite_SetDrawFlag(ctx->motions[1].sprite, TRUE);
        ctx->state++;
    case 2:
        f.done[0] = TRUE;
        f.done[1] = ov92_022620D0(&ctx->motions[3], ctx->dur1, 0, 1);
        f.done[2] = ov92_02261E88(&ctx->motions[0], ctx->dur2, 1, 2);
        f.done[3] = ov92_02262018(&ctx->motions[1], ctx->dur2, 1, 3);
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ctx->state++;
    case 3:
        f.done[0] = ov92_02261F60(&ctx->motions[2], ctx->dur1, 0, 2);
        f.done[1] = ov92_02261E88(&ctx->motions[3], ctx->dur1, 0, 3);
        f.done[2] = TRUE;
        f.done[3] = ov92_02261E88(&ctx->motions[1], ctx->dur2, 1, 2);
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ctx->state++;
    case 4:
        f.done[0] = ov92_022620D0(&ctx->motions[2], ctx->dur1, 0, 1);
        f.done[1] = TRUE;
        f.done[2] = TRUE;
        f.done[3] = ov92_02261E88(&ctx->motions[1], ctx->dur2, 1, 2);
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        if (ctx->loopCount != 1) {
            ctx->loopCount++;
            ctx->state = 1;
        } else {
            ctx->loopCount = 0;
            ctx->state++;
        }
        break;
    default:
        SysTask_Destroy(task);
        break;
    }
}

void ov92_02262548(SysTask *task, Ov92Ctx *ctx) {
    Ov92Done4 f = ov92_02263E0C;
    fx32 x, y;
    fx32 newX, newY;
    s32 i;

    if (!IsPaletteFadeFinished() || ctx->unk17C->unk34 == 1) {
        SysTask_Destroy(task);
        return;
    }
    switch (ctx->state) {
    case 0:
        for (i = 0; i < ctx->count; i++) {
            ctx->motions[i].angle = 0;
            ctx->motions[i].baseX = 0x80;
            ctx->motions[i].baseY = 0xa0;
            ctx->motions[i].radiusX = 0x40;
            ctx->motions[i].radiusY = 0x18;
            ManagedSprite_SetDrawFlag(ctx->motions[i].sprite, TRUE);
            ctx->motions[i + 2].angle = 0;
            ctx->motions[i + 2].baseX = 0x80;
            ctx->motions[i + 2].baseY = 0xa0;
            ctx->motions[i + 2].radiusX = 0x40;
            ctx->motions[i + 2].radiusY = 0x18;
            ManagedSprite_SetDrawFlag(ctx->motions[i + 2].sprite, TRUE);
            ManagedSprite_GetPositionFxXYWithSubscreenOffset(ctx->motions[i].sprite, &x, &y, 1 << 20);
            newX = (ctx->motions[i].baseX << 12) + GF_SinDeg(ctx->motions[i].angle) * ctx->motions[i].radiusX;
            newY = (ctx->motions[i].baseY << 12) + GF_CosDeg(ctx->motions[i].angle) * ctx->motions[i].radiusY;
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(ctx->motions[i].sprite, newX, newY, 1 << 20);
            ManagedSprite_GetPositionFxXYWithSubscreenOffset(ctx->motions[i + 2].sprite, &x, &y, 1 << 20);
            newX = (ctx->motions[i + 2].baseX << 12) + GF_SinDeg(ctx->motions[i + 2].angle) * ctx->motions[i + 2].radiusX;
            newY = (ctx->motions[i + 2].baseY << 12) - GF_CosDeg(ctx->motions[i + 2].angle) * ctx->motions[i + 2].radiusY;
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(ctx->motions[i + 2].sprite, newX, newY, 1 << 20);
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ManagedSprite_SetDrawFlag(ctx->motions[1].sprite, FALSE);
        ctx->state++;
    case 1:
        f.done[0] = ov92_02261E88(&ctx->motions[2], ctx->dur1, 0, 3);
        f.done[1] = ov92_02261F60(&ctx->motions[3], ctx->dur1, 0, 2);
        f.done[2] = ov92_02262018(&ctx->motions[0], ctx->dur2, 1, 3);
        f.done[3] = TRUE;
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ManagedSprite_SetDrawFlag(ctx->motions[1].sprite, TRUE);
        ctx->state++;
    case 2:
        f.done[0] = TRUE;
        f.done[1] = ov92_022620D0(&ctx->motions[3], ctx->dur1, 0, 2);
        f.done[2] = ov92_02261E88(&ctx->motions[0], ctx->dur2, 1, 2);
        f.done[3] = ov92_02262018(&ctx->motions[1], ctx->dur2, 1, 3);
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ctx->state++;
    case 3:
        f.done[0] = ov92_02261F60(&ctx->motions[2], ctx->dur1, 0, 2);
        f.done[1] = ov92_02261E88(&ctx->motions[3], ctx->dur1, 0, 3);
        f.done[2] = TRUE;
        f.done[3] = ov92_02261E88(&ctx->motions[1], ctx->dur2, 1, 2);
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ctx->state++;
    case 4:
        f.done[0] = ov92_022620D0(&ctx->motions[2], ctx->dur1, 0, 2);
        f.done[1] = TRUE;
        f.done[2] = TRUE;
        f.done[3] = ov92_02261E88(&ctx->motions[1], ctx->dur2, 1, 2);
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        if (ctx->loopCount != 1) {
            ctx->loopCount++;
            ctx->state = 1;
        } else {
            ctx->loopCount = 0;
            ctx->state++;
        }
        break;
    default:
        SysTask_Destroy(task);
        break;
    }
}

void ov92_022628F0(SysTask *task, Ov92Ctx *ctx) {
    Ov92Done4 f = ov92_02263E2C;
    fx32 x, y;
    fx32 newX, newY;
    s32 i;

    if (!IsPaletteFadeFinished() || ctx->unk17C->unk34 == 1) {
        SysTask_Destroy(task);
        return;
    }
    switch (ctx->state) {
    case 0:
        for (i = 0; i < ctx->count; i++) {
            ctx->motions[i].angle = 0;
            ctx->motions[i].baseX = 0x80;
            ctx->motions[i].baseY = 0xa0;
            ctx->motions[i].radiusX = 0x40;
            ctx->motions[i].radiusY = 0x18;
            ManagedSprite_SetDrawFlag(ctx->motions[i].sprite, TRUE);
            ctx->motions[i + 2].angle = 0;
            ctx->motions[i + 2].baseX = 0x80;
            ctx->motions[i + 2].baseY = 0xa0;
            ctx->motions[i + 2].radiusX = 0x40;
            ctx->motions[i + 2].radiusY = 0x18;
            ManagedSprite_SetDrawFlag(ctx->motions[i + 2].sprite, TRUE);
            ManagedSprite_GetPositionFxXYWithSubscreenOffset(ctx->motions[i].sprite, &x, &y, 1 << 20);
            newX = (ctx->motions[i].baseX << 12) + GF_SinDeg(ctx->motions[i].angle) * ctx->motions[i].radiusX;
            newY = (ctx->motions[i].baseY << 12) + GF_CosDeg(ctx->motions[i].angle) * ctx->motions[i].radiusY;
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(ctx->motions[i].sprite, newX, newY, 1 << 20);
            ManagedSprite_GetPositionFxXYWithSubscreenOffset(ctx->motions[i + 2].sprite, &x, &y, 1 << 20);
            newX = (ctx->motions[i + 2].baseX << 12) + GF_SinDeg(ctx->motions[i + 2].angle) * ctx->motions[i + 2].radiusX;
            newY = (ctx->motions[i + 2].baseY << 12) - GF_CosDeg(ctx->motions[i + 2].angle) * ctx->motions[i + 2].radiusY;
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(ctx->motions[i + 2].sprite, newX, newY, 1 << 20);
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ManagedSprite_SetDrawFlag(ctx->motions[1].sprite, FALSE);
        ctx->state++;
    case 1:
        f.done[0] = ov92_02261E88(&ctx->motions[2], ctx->dur1, 0, 3);
        f.done[1] = ov92_02261F60(&ctx->motions[3], ctx->dur1, 0, 2);
        f.done[2] = ov92_02262018(&ctx->motions[0], ctx->dur2, 1, 3);
        f.done[3] = TRUE;
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ManagedSprite_SetDrawFlag(ctx->motions[1].sprite, TRUE);
        ctx->state++;
    case 2:
        f.done[0] = TRUE;
        f.done[1] = ov92_022620D0(&ctx->motions[3], ctx->dur1, 0, 1);
        f.done[2] = ov92_02261E88(&ctx->motions[0], ctx->dur2, 1, 2);
        f.done[3] = ov92_02262018(&ctx->motions[1], ctx->dur2, 1, 3);
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ctx->state++;
    case 3:
        f.done[1] = ov92_022620D0(&ctx->motions[3], ctx->dur1, 1, 1);
        if (!f.done[1]) {
            break;
        }
        ov92_02261E80(&ctx->motions[3]);
        ctx->state++;
    case 4:
        f.done[0] = ov92_02261F60(&ctx->motions[2], ctx->dur1, 0, 2);
        f.done[1] = ov92_02261E88(&ctx->motions[3], ctx->dur1, 0, 3);
        f.done[2] = TRUE;
        f.done[3] = ov92_02261E88(&ctx->motions[1], ctx->dur2, 1, 2);
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ctx->state++;
    case 5:
        f.done[0] = ov92_022620D0(&ctx->motions[2], ctx->dur1, 0, 1);
        f.done[1] = TRUE;
        f.done[2] = TRUE;
        f.done[3] = ov92_02261E88(&ctx->motions[1], ctx->dur2, 1, 2);
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ctx->state++;
    case 6:
        f.done[0] = ov92_022620D0(&ctx->motions[2], ctx->dur1, 1, 1);
        if (!f.done[0]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        if (ctx->loopCount != 1) {
            ctx->loopCount++;
            ctx->state = 1;
        } else {
            ctx->loopCount = 0;
            ctx->state++;
        }
        break;
    default:
        SysTask_Destroy(task);
        break;
    }
}

void ov92_02262CEC(SysTask *task, Ov92Ctx *ctx) {
    Ov92Done4 f = ov92_02263E4C;
    fx32 x, y;
    fx32 newX, newY;
    s32 i;

    if (!IsPaletteFadeFinished() || ctx->unk17C->unk34 == 1) {
        SysTask_Destroy(task);
        return;
    }
    switch (ctx->state) {
    case 0:
        for (i = 0; i < ctx->count; i++) {
            ctx->motions[i].angle = 0;
            ctx->motions[i].baseX = 0x80;
            ctx->motions[i].baseY = 0xa0;
            ctx->motions[i].radiusX = 0x40;
            ctx->motions[i].radiusY = 0x18;
            ManagedSprite_SetDrawFlag(ctx->motions[i].sprite, TRUE);
            ctx->motions[i + 2].angle = 0;
            ctx->motions[i + 2].baseX = 0x80;
            ctx->motions[i + 2].baseY = 0xa0;
            ctx->motions[i + 2].radiusX = 0x40;
            ctx->motions[i + 2].radiusY = 0x18;
            ManagedSprite_SetDrawFlag(ctx->motions[i + 2].sprite, TRUE);
            ManagedSprite_GetPositionFxXYWithSubscreenOffset(ctx->motions[i].sprite, &x, &y, 1 << 20);
            newX = (ctx->motions[i].baseX << 12) + GF_SinDeg(ctx->motions[i].angle) * ctx->motions[i].radiusX;
            newY = (ctx->motions[i].baseY << 12) + GF_CosDeg(ctx->motions[i].angle) * ctx->motions[i].radiusY;
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(ctx->motions[i].sprite, newX, newY, 1 << 20);
            ManagedSprite_GetPositionFxXYWithSubscreenOffset(ctx->motions[i + 2].sprite, &x, &y, 1 << 20);
            newX = (ctx->motions[i + 2].baseX << 12) + GF_SinDeg(ctx->motions[i + 2].angle) * ctx->motions[i + 2].radiusX;
            newY = (ctx->motions[i + 2].baseY << 12) - GF_CosDeg(ctx->motions[i + 2].angle) * ctx->motions[i + 2].radiusY;
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(ctx->motions[i + 2].sprite, newX, newY, 1 << 20);
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ManagedSprite_SetDrawFlag(ctx->motions[1].sprite, FALSE);
        ctx->state++;
    case 1:
        f.done[0] = ov92_02261E88(&ctx->motions[2], ctx->dur1, 0, 3);
        f.done[1] = ov92_02261F60(&ctx->motions[3], ctx->dur1, 0, 2);
        f.done[2] = ov92_02262018(&ctx->motions[0], ctx->dur2, 1, 3);
        f.done[3] = TRUE;
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ManagedSprite_SetDrawFlag(ctx->motions[1].sprite, TRUE);
        ctx->state++;
    case 2:
        f.done[0] = TRUE;
        f.done[1] = ov92_022620D0(&ctx->motions[3], ctx->dur1, 0, 1);
        f.done[2] = ov92_02261E88(&ctx->motions[0], ctx->dur2, 1, 2);
        f.done[3] = ov92_02262018(&ctx->motions[1], ctx->dur2, 1, 3);
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ctx->state++;
    case 3:
        f.done[1] = ov92_022620D0(&ctx->motions[3], ctx->dur1, 1, 1);
        if (!f.done[1]) {
            break;
        }
        ov92_02261E80(&ctx->motions[3]);
        ctx->state++;
    case 4:
        f.done[0] = ov92_02261F60(&ctx->motions[2], ctx->dur1, 0, 2);
        f.done[1] = ov92_02261E88(&ctx->motions[3], ctx->dur1, 0, 3);
        f.done[2] = TRUE;
        f.done[3] = ov92_02261E88(&ctx->motions[1], ctx->dur2, 1, 2);
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ctx->state++;
    case 5:
        f.done[0] = ov92_022620D0(&ctx->motions[2], ctx->dur1, 0, 1);
        f.done[1] = TRUE;
        f.done[2] = TRUE;
        f.done[3] = ov92_02261E88(&ctx->motions[1], ctx->dur2, 1, 2);
        if (!f.done[0] || !f.done[1] || !f.done[2] || !f.done[3]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        ov92_02261E80(&ctx->motions[3]);
        ov92_02261E80(&ctx->motions[0]);
        ov92_02261E80(&ctx->motions[1]);
        ctx->state++;
    case 6:
        f.done[0] = ov92_022620D0(&ctx->motions[2], ctx->dur1, 1, 1);
        if (!f.done[0]) {
            break;
        }
        ov92_02261E80(&ctx->motions[2]);
        if (ctx->loopCount != 8) {
            ctx->loopCount++;
            ctx->state = 1;
        } else {
            ctx->loopCount = 0;
            ctx->state++;
        }
        break;
    default:
        SysTask_Destroy(task);
        break;
    }
}
