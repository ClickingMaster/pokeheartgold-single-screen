#include "to53_overlay112_w09_private.h"


void ov112_021E9F40(void *p, int index) {
    ov112_021E9FA4(p, index, *(String **)((u8 *)p + 0x1E474), 4, 0xF0100);
}

void ov112_021E9F5C(void *p, int index, int msg, u32 color) {
    String *s = NewString_ReadMsgData(*(void **)((u8 *)p + 0x1E44C), msg);
    StringExpandPlaceholders(*(void **)((u8 *)p + 0x1E448), *(String **)((u8 *)p + 0x1E454), s);
    ov112_021E9FA4(p, index, *(String **)((u8 *)p + 0x1E454), 0, color);
    String_Delete(s);
}

void ov112_021E9FA4(void *p, int index, String *s, int font, u32 color) {
    Window *win = (Window *)((u8 *)p + 0x1EA68 + index * 0x10);
    FillWindowPixelBuffer(win, 0);
    AddTextPrinterParameterizedWithColor(win, font, s, 0, 0, 0, color, 0);
}

void ov112_021E9FD8(void *p, int index, String *s, int font) {
    Window *win = (Window *)((u8 *)p + 0x1EA68 + index * 0x10);
    FillWindowPixelBuffer(win, 0);
    AddTextPrinterParameterizedWithColor(win, font, s, 0, 0, 0, 0x10200, 0);
}

void ov112_021EA010(void *p, int index, String *s, int font, u32 color) {
    Window *win = (Window *)((u8 *)p + 0x1EBA8 + index * 0x10);
    FillWindowPixelBuffer(win, 0);
    AddTextPrinterParameterizedWithColor(win, font, s, 0, 0, 0, color, 0);
}

void ov112_021EA044(void *p, int index, int msg, u32 color) {
    String *s = NewString_ReadMsgData(*(void **)((u8 *)p + 0x1E44C), msg);
    StringExpandPlaceholders(*(void **)((u8 *)p + 0x1E448), *(String **)((u8 *)p + 0x1E454), s);
    ov112_021EA010(p, index, *(String **)((u8 *)p + 0x1E454), 0, color);
    String_Delete(s);
}
