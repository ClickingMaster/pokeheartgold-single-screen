#include "global.h"

#include "bg_window.h"

typedef struct Overlay31WindowParam {
    u32 x;
    u32 y;
    u32 baseTile;
} Overlay31WindowParam;

typedef struct Overlay31State {
    u8 pad_00[4];
    BgConfig *bgConfig;
    u8 pad_08[0x2C];
    Window win34;
    Window win44;
    Window win54;
    Window win64;
    Window win74;
    Window windows[13];
} Overlay31State;

extern const Overlay31WindowParam ov31_0225EE88[6];
void ov31_0225DBD4(Overlay31State *);
void ov31_0225DCA8(Overlay31State *);

void ov31_0225DBD4(Overlay31State *state) {
    int i;

    AddWindowParameterized(state->bgConfig, &state->win34, 4, 2, 1, 0x1B, 2, 0xB, 1);
    AddWindowParameterized(state->bgConfig, &state->win44, 4, 2, 1, 0x1B, 4, 0xB, 0x1D);
    AddWindowParameterized(state->bgConfig, &state->win74, 4, 1, 0, 9, 4, 0xB, 0x89);
    AddWindowParameterized(state->bgConfig, &state->win54, 4, 0xA, 0x15, 7, 2, 0xC, 1);
    FillWindowPixelBuffer(&state->win54, 0);
    AddWindowParameterized(state->bgConfig, &state->win64, 4, 0x18, 0x15, 7, 2, 0xC, 0xF);
    FillWindowPixelBuffer(&state->win64, 0);
    for (i = 0; i < 13; i++, state = (Overlay31State *)((u8 *)state + sizeof(Window))) {
        *(u32 *)((u8 *)state + 0x84) = 0;
    }
}

void ov31_0225DCA8(Overlay31State *state) {
    int i;

    for (i = 0; i < 6; i++) {
        AddWindowParameterized(state->bgConfig, &state->windows[i], 4, (u8)ov31_0225EE88[i].x, (u8)ov31_0225EE88[i].y, 0xB, 4, 0xC, (u16)ov31_0225EE88[i].baseTile);
    }
}
