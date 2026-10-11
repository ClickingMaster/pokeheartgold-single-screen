#include "to47_overlay14_window_message_r59_private.h"

void PCBox_PrintCenteredLabel(Ov14R59Context *ctx, u32 windowIndex, u32 messageId, u32 unused) {
    Window *window = &ctx->windows[windowIndex];
    u8 x;

    FillWindowPixelBuffer(window, 0xD);
    x = GetWindowWidth(window) * 8 / 2;
    PCBox_PrintMsg(ctx, ctx->msgData, windowIndex, messageId, x, 0, 4, 0x00090A0D, 2);
    CopyWindowPixelsToVram_TextMode(window);
}
