#include "to56_overlay08_bank30_private.h"

#pragma force_active on

void ov08_02223464(W19Work *w) {
    u8 *work = (u8 *)w;
    WindowArray_Delete(*(void **)(work + 0x2C), work[0x30]);
    RemoveWindow(work + 0x1C);
}

void ov08_02223480(W19Work *work, int state) {
    switch (state) {
    case 0:
        ov08_022234FC(work);
        break;
    case 1:
        ov08_022239B4(work);
        break;
    case 2:
        ov08_02223AA0(work);
        break;
    }
}

void ov08_022234A4(W19Work *w, u32 window, u32 msgNo, u32 font, u32 y, u32 color) {
    u8 *work = (u8 *)w;
    W19Window *windows = *(W19Window **)(work + 0x2C);
    W19Window *win = &windows[window];
    void *str = NewString_ReadMsgData(*(void **)(work + 0x10), msgNo);
    u32 width = FontID_String_GetWidth(font, str, 0);
    u32 x = (GetWindowWidth(win) * 8 - width) / 2;
    AddTextPrinterParameterizedWithColor(win, font, str, x, y, 0xFF, color, NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(win);
}

void ov08_022234FC(W19Work *w) {
    u32 i = 0;
    u8 *work = (u8 *)w;
    u32 off = i;
    u32 zero = i;
    do {
        FillWindowPixelBuffer((u8 *)*(void **)(work + 0x2C) + off, zero);
        i++;
        off += 0x10;
    } while (i < 5);
    ov08_022234A4(w, 0, 0, 4, 7, 0x30201);
    ov08_022234A4(w, 0, 1, 4, 0x17, 0x30201);
    ov08_022234A4(w, 1, 2, 4, 7, 0x30201);
    ov08_022234A4(w, 1, 3, 4, 0x17, 0x30201);
    ov08_022234A4(w, 2, 7, 4, 7, 0x30201);
    ov08_022234A4(w, 3, 6, 4, 7, 0x30201);
    if (*(u16 *)(*(u8 **)work + 0x20) != 0) {
        void *str = NewString_ReadMsgData(*(void **)(work + 0x10), 8);
        AddTextPrinterParameterizedWithColor((u8 *)*(void **)(work + 0x2C) + 0x40, 4, str, 0, 5, 0xFF, 0x30201, NULL);
        String_Delete(str);
        ScheduleWindowCopyToVram((u8 *)*(void **)(work + 0x2C) + 0x40);
    }
}
