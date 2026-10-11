#include "r40_overlay_14_residual_40_private.h"
#define SUB(ctx) (*(void **)(*(u8 **)((ctx) + 0x34) + 0x2f0))

void ov14_021F263C(u8 *ctx) {
    PCBox_SlideOutActionMenu(SUB(ctx));
    PCBox_SlideInPartyButton(SUB(ctx));
    ov14_021E82A8(SUB(ctx));
    ov14_021E8328(SUB(ctx));
    ov14_021E884C(*(void **)(ctx + 0x34));
    PCBox_SetSelectedIconOutlineVisible(ctx, 0);
    PCBox_WaitForUpdateFunc(ctx, ov14_021EA180, 0x4a);
}

void ov14_021F2690(u8 *ctx) {
    if (ctx[0x2b] == 0) {
        PCBox_ClearMonInfoAndSelection(ctx);
    } else {
        PCBox_SetSelectedIconOutlineVisible(ctx, 0);
    }
    ov14_021F0C0C(ctx);
}

void ov14_021F26B4(u8 *ctx) {
    PCBox_SlideOutActionMenu(SUB(ctx));
    ov14_021E8328(SUB(ctx));
    ov14_021E884C(*(void **)(ctx + 0x34));
    PCBox_SetSelectedIconOutlineVisible(ctx, 0);
    PCBox_WaitForUpdateFunc(ctx, ov14_021EA180, 0x4c);
}

void ov14_021F26F0(u8 *ctx) {
    PCBox_ClearMonInfoAndSelection(ctx);
    ov14_021F0B34(ctx);
}
