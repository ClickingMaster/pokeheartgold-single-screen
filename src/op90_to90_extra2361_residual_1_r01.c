#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "font.h"
#include "text.h"
typedef struct State {
    u32 heap;
    u8 pad[0x310 - 4];
    String *strings2[5];
    String *strings[9];
    u8 pad348[0x3b4 - 0x348];
    Window windows[15];
} State;
void ov108_021E7700(State *s, u8 index, u8 msg);
// @fn ov108_021E7700
void ov108_021E7700(State *s, u8 index, u8 msg) {
    String **strings;
    u32 x;
    u8 width, height;
    u32 color;
    int y;
    width = GetWindowWidth(&s->windows[index]) * 8;
    height = GetWindowHeight(&s->windows[index]) * 8;
    color = 0x30102;
    if (index != 2) {
        strings = s->strings;
        x = 0;
        y = 4;
        if (index <= 1) {
            color = 0xd0c0e;
            FillWindowPixelBuffer(&s->windows[index], 14);
            FillWindowPixelRect(&s->windows[index], 6, 0, 0, width, 2);
            FillWindowPixelRect(&s->windows[index], 6, 0, height - 2, width, 2);
        } else {
            FillWindowPixelBuffer(&s->windows[index], 2);
        }
    } else {
        strings = s->strings2;
        x = (u8)((width - FontID_String_GetWidth(0, strings[msg], 0)) / 2);
        y = 4;
    }
    AddTextPrinterParameterizedWithColor(&s->windows[index], 0, strings[msg], x, y, 0, color, NULL);
}
