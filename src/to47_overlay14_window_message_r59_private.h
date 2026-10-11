#ifndef POKEHEARTGOLD_TO47_OVERLAY14_WINDOW_MESSAGE_R59_PRIVATE_H
#define POKEHEARTGOLD_TO47_OVERLAY14_WINDOW_MESSAGE_R59_PRIVATE_H

#include "bg_window.h"
#include "global.h"

typedef struct Ov14R59Context {
    u8 filler_00[0x20];
    void *msgData;
    u8 filler_24[0x0C];
    Window windows[];
} Ov14R59Context;

void PCBox_PrintMsg(void *ctx, void *msgData, u32 windowIndex, u32 messageId, s32 x, s32 y, u32 font, u32 color, u32 alignment);
void PCBox_PrintCenteredLabel(Ov14R59Context *ctx, u32 windowIndex, u32 messageId, u32 unused);

#endif
