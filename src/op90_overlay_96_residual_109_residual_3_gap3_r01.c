#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "text.h"
typedef struct {
    u8 pad0[0x18];
    Window windows[8];
    u8 pad98[0x7B8 - 0x98];
    String *str7B8;
    String *str7BC;
    String *str7C0;
    String *strs[8];
    u8 pad7E4[0x818 - 0x7E4];
    u8 flags[8];
} W96;
void ov96_02214044(W96 *w, u8 idx, int mode);
// @fn ov96_02214044
void ov96_02214044(W96 *w, u8 idx, int mode){String *str; u32 color; FillWindowPixelBuffer(&w->windows[idx], 0); if (mode != 0) { if (mode == 2) { str = w->str7B8; } else if (mode == 1) { str = w->str7BC; } else if (mode == 3) { str = w->str7C0; } else { GF_AssertFail(); str = w->str7B8; } AddTextPrinterParameterizedWithColor(&w->windows[idx], 4, str, 0, 0, 0xFF, 0x10200, NULL); } else { if (w->flags[idx]) { color = 0x30400; } else { color = 0x10200; } AddTextPrinterParameterizedWithColor(&w->windows[idx], 4, w->strs[idx], 0, 0, 0xFF, color, NULL); } CopyWindowToVram(&w->windows[idx]);}
