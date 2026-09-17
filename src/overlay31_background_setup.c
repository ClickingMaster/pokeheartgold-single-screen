#include "global.h"

#include "bg_window.h"

typedef struct Overlay31State {
    u8 pad_00[4];
    BgConfig *bgConfig;
} Overlay31State;

extern const BgTemplate ov31_0225EED0;
extern const BgTemplate ov31_0225EEEC;
extern const BgTemplate ov31_0225EF08;
extern void GfGfxLoader_GXLoadPal(u32, s32, u32, u32, u32, u32);
extern void GfGfxLoader_LoadCharData(u32, s32, BgConfig *, u32, u32, u32, u32, u32);
extern void LoadFontPal1(u32, u32, u32);
extern void LoadFontPal0(u32, u32, u32);
extern void ov31_0225D9D4(Overlay31State *, int);
extern void ov31_0225DBD4(Overlay31State *);
extern void ov31_0225DCA8(Overlay31State *);
void ov31_0225DAC4(Overlay31State *);
void ov31_0225DB38(Overlay31State *);

void ov31_0225DAC4(Overlay31State *state) {
    vu32 *reg = (vu32 *)0x04001000;

    GXS_SetGraphicsMode((GXBGMode)0);
    GX_SetBankForSubBG((GXVRamSubBG)0x80);
    GX_SetBankForSubOBJ((GXVRamSubOBJ)0x100);
    *reg = (*reg & 0xFFCFFFEF) | 0x10;
    InitBgFromTemplate(state->bgConfig, 4, &ov31_0225EED0, 0);
    InitBgFromTemplate(state->bgConfig, 5, &ov31_0225EEEC, 0);
    InitBgFromTemplate(state->bgConfig, 6, &ov31_0225EF08, 0);
    BG_ClearCharDataRange(6, 0x20, 0, (enum HeapID)4);
    BgClearTilemapBufferAndCommit(state->bgConfig, 6);
}

void ov31_0225DB38(Overlay31State *state) {
    *(vu16 *)0x04001050 = 0;
    GfGfxLoader_GXLoadPal(0x3C, 0xF, 4, 0, 0, 8);
    GfGfxLoader_LoadCharData(0x3C, 0x10, state->bgConfig, 5, 0, 0, 0, 8);
    ov31_0225D9D4(state, 0);
    LoadFontPal1(4, 0x160, 8);
    LoadFontPal0(4, 0x180, 8);
    ov31_0225DBD4(state);
    ov31_0225DCA8(state);
}
