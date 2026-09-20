#include "to55_ov112_r170_private.h"

void ov112_021F13CC(void *p, int msg1, int msg2) {
    void *str1;
    void *str2;
    void *joined;
    void *window = (u8 *)p + 0x38;

    FillWindowPixelBuffer(window, 0);
    str1 = ReadMsgData_ExpandPlaceholders(PTR_AT(p, 0x58), PTR_AT(p, 0x5c), msg1, U32_AT(p, 4));
    if (msg2 != 0) {
        str2 = ReadMsgData_ExpandPlaceholders(PTR_AT(p, 0x58), PTR_AT(p, 0x5c), msg2, U32_AT(p, 4));
        joined = String_New(String_GetLength(str1) + String_GetLength(str2) + 2, U32_AT(p, 4));
        String_Copy(joined, str1);
        String_AddChar(joined, 0xe000);
        String_Cat(joined, str2);
        AddTextPrinterParameterizedWithColor(window, 1, joined, 0, 0, 0xff, 0x10200, NULL);
        String_Delete(str2);
        String_Delete(joined);
    } else {
        AddTextPrinterParameterizedWithColor(window, 1, str1, 0, 0, 0xff, 0x10200, NULL);
    }
    String_Delete(str1);
    CopyWindowToVram(window);
}
