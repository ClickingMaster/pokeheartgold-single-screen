#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct Choice74 {
    u32 text;
    u32 value;
} Choice74;
typedef struct Participant74 {
    u8 unk000[0x348];
    u8 flag;
} Participant74;
typedef struct WinTmpl74 {
    u32 unk00;
    u32 bgId;
    u32 x;
    u32 y;
    u32 width;
} WinTmpl74;
typedef struct Work74 {
    u8 unk0000[0x29FC];
    BgConfig *bgConfig;
    u8 unk2A00[0x198];
    u32 baseTile;
    WinTmpl74 *tmpl;
    u8 unk2BA0[0x14];
    Participant74 *participants[3];
    int selected;
} Work74;
extern Choice74 ov74_0223C2D0[];
void ov74_0222D11C(Work74 *work, Choice74 *choices, int count, Window *window, int a4);
BOOL ov74_0222D690(Work74 *work, Window *window);
// @fn ov74_0222D690
BOOL ov74_0222D690(Work74 *work, Window *window) {
    Choice74 choices[4];
    WinTmpl74 *tmpl;
    int n;
    int d;
    n = 0;
    choices[n++] = ov74_0223C2D0[6];
    if (work->participants[work->selected]->flag) {
        choices[n++] = ov74_0223C2D0[7];
    }
    choices[n] = ov74_0223C2D0[8];
    choices[n + 1] = ov74_0223C2D0[9];
    tmpl = work->tmpl;
    ClearWindowTilemapAndCopyToVram(window);
    RemoveWindow(window);
    d = (4 - (n + 2)) * 2;
    AddWindowParameterized(work->bgConfig, window, 2, tmpl->bgId, tmpl->x + d, tmpl->y, tmpl->width - d, 15, work->baseTile);
    ov74_0222D11C(work, choices, n + 2, window, 0);
    return TRUE;
}
