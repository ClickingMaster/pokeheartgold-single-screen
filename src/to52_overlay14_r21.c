#include "to52_overlay14_r21_private.h"

int PCBox_State_Delay(Ov14Context *ctx) {
    u32 value = *(u32 *)(ctx->work + 0x444);

    if (value == 0) {
        return ctx->state;
    }
    *(u32 *)(ctx->work + 0x444) = value - 1;
    return 4;
}

int PCBox_State_WaitUpdateFunc(Ov14Context *ctx) {
    if (*(u32 *)(ctx->work + 4) == 0) {
        return sPCBoxStateTable[ctx->state](ctx);
    }
    return 5;
}

int PCBox_State_WaitButton(Ov14Context *ctx) {
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

int PCBox_State_HandleYesNo(Ov14Context *ctx) {
    int input = YesNoPrompt_HandleInput(*(void **)(ctx->work + 0x434));

    switch (input) {
    case 1:
        YesNoPrompt_Reset(*(void **)(ctx->work + 0x434));
        return sPCBoxYesNoHandlers[*(u16 *)(ctx->work + 0x438)].handler(ctx);
    case 2:
        YesNoPrompt_Reset(*(void **)(ctx->work + 0x434));
        return ov14_021F7D78[*(u16 *)(ctx->work + 0x438)].handler(ctx);
    default:
        return 7;
    }
}
