#ifndef OVERLAY_86_RESIDUAL_3_PRIVATE_H
#define OVERLAY_86_RESIDUAL_3_PRIVATE_H
#include "global.h"

#include "bg_window.h"

typedef struct Overlay86GfxState {
    u8 padding_000[0xC];
    BgConfig *bgConfig;
    u8 padding_010[0x324];
    u8 screenData[0x60];
} Overlay86GfxState;

u32 ov86_021E5E98(u32 kind);
void ov86_021E5ECC(Overlay86GfxState *state, GFBgLayer layer);
void ov86_021E5F58(Overlay86GfxState *state, GFBgLayer layer);
#endif
