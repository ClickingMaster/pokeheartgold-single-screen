#include "overlay74_thumb_w07_private.h"

int ov74_0222EF18(void *data, void *window, int color) {
    void *string = String_New(0xfb, 0x55);
    CopyU16ArrayToStringN(string, (u16 *)((u8 *)(void *)U32AT(data, 0x2bb4) + 0x154), 0xfa);
    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, 0xff, color, 0);
    String_Delete(string);
    return 0;
}
u32 ov74_0222EF64(void) {
    return 0;
}
