#include "to52_overlay14_r21_private.h"

int ov14_021EB1A4(Ov14Context *ctx) {
    u32 value = *(u32 *)(ctx->work + 0x444);

    if (value == 0) {
        return ctx->state;
    }
    *(u32 *)(ctx->work + 0x444) = value - 1;
    return 4;
}

int ov14_021EB1C0(Ov14Context *ctx) {
    if (*(u32 *)(ctx->work + 4) == 0) {
        return ov14_021F7D9C[ctx->state](ctx);
    }
    return 5;
}

int ov14_021EB1E0(Ov14Context *ctx) {
    if (System_GetTouchNew() == 1) {
        PlaySE(0x5DD);
        return ctx->state;
    }
    if ((*(u32 *)(gSystem + 0x48) & 3) != 0) {
        PlaySE(0x5DD);
        return ctx->state;
    }
    return 6;
}

int ov14_021EB218(Ov14Context *ctx) {
    int input = YesNoPrompt_HandleInput(*(void **)(ctx->work + 0x434));

    switch (input) {
    case 1:
        YesNoPrompt_Reset(*(void **)(ctx->work + 0x434));
        return ov14_021F7D74[*(u16 *)(ctx->work + 0x438)].handler(ctx);
    case 2:
        YesNoPrompt_Reset(*(void **)(ctx->work + 0x434));
        return ov14_021F7D78[*(u16 *)(ctx->work + 0x438)].handler(ctx);
    default:
        return 7;
    }
}
