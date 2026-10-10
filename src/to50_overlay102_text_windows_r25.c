#include "to50_overlay102_text_windows_r25_internal.h"
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
void ov102_021EADF8(u8 *work, u32 index) {
    void *bg = ov102_021EA268(PTR(work, 0));
    void *window;
    FillBgTilemapRect(bg, 0, 0, 0, 0x14, 0x18, 4, 0x11);
    ClearWindowTilemapAndCopyToVram(PTR(work, 0x5C));
    window = work + 0x2C + index * 0x10;
    PTR(work, 0x5C) = window;
    DrawFrame2AroundWindow(window, U16(work, 0x1EC), 0x0F);
}
void ov102_021EAE40(u8 *work, u32 mode) {
    void *fmt;
    void *str;
    FillWindowPixelBuffer(PTR(work, 0x5C), 9);
    switch (mode) {
    case 0:
        fmt = MessageFormat_New_Custom(2, 0x3C, 0);
        str = String_New(0x12C, 0);
        BufferPocketName(fmt, 0, 0);
        ReadMsgDataIntoString(PTR(work, 0x78), ov102_021E8F70(PTR(work, 4)), PTR(work, 0x7C));
        StringExpandPlaceholders(fmt, str, PTR(work, 0x7C));
        AddTextPrinterParameterizedWithColor(PTR(work, 0x5C), 1, str, 0, 0, 0xFF, 0x00010209, NULL);
        String_Delete(str);
        MessageFormat_Delete(fmt);
        break;
    case 2:
        ReadMsgDataIntoString(PTR(work, 0x78), 6, PTR(work, 0x7C));
        AddTextPrinterParameterizedWithColor(PTR(work, 0x5C), 1, PTR(work, 0x7C), 0, 0, 0xFF, 0x00010209, NULL);
        break;
    case 1:
        ReadMsgDataIntoString(PTR(work, 0x78), 7, PTR(work, 0x7C));
        AddTextPrinterParameterizedWithColor(PTR(work, 0x5C), 1, PTR(work, 0x7C), 0, 0, 0xFF, 0x00010209, NULL);
        break;
    case 3:
        ReadMsgDataIntoString(PTR(work, 0x78), 8, PTR(work, 0x7C));
        AddTextPrinterParameterizedWithColor(PTR(work, 0x5C), 1, PTR(work, 0x7C), 0, 0, 0xFF, 0x00010209, NULL);
        break;
    }
    CopyWindowToVram(PTR(work, 0x5C));
}
