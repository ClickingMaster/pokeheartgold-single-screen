#include "overlay74_thumb_w07_private.h"

u32 ov74_0222EE08(void) {
    return 1;
}
int ov74_0222EE0C(void *data, void *window, int color) {
    void *string = String_New(0x25, U32AT(data, 0));
    CopyU16ArrayToStringN(string, (u16 *)((u8 *)(void *)U32AT(data, 0x2bb4) + 0x104), 0x24);
    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, 0xff, color, 0);
    String_Delete(string);
    return 1;
}
int ov74_0222EE58(void *data, void *window, int color) {
    int msg = U8AT((void *)U32AT(data, 0x2bb4), 0x350) == 0 ? 0x51 : 0x52;
    void *string = ReadMsgData_ExpandPlaceholders((void *)U32AT(data, 0x2a00), (void *)U32AT(data, 0x2a04), msg, U32AT(data, 0));
    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, 0xff, color, 0);
    String_Delete(string);
    return 1;
}
