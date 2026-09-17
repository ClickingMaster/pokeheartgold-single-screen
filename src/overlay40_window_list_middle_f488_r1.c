#include "overlay40_window_list_middle_f488_r1_private.h"

int ov40_0222F488(void *p, void *owner) {
    int i;
    int start;
    void *window;

    if (SWORD(p, 0x38) == SWORD(p, 0xC)) {
        return 0;
    }
    if (SWORD(p, 0x10) == SWORD(p, 4)) {
        return 0;
    }

    window = (u8 *)p + 0x18;
    FillWindowPixelBuffer(window, 0);
    start = 0;
    if (SWORD(p, 0x3C) >= SWORD(p, 0x40)) {
        start = SWORD(p, 0x38) - SWORD(p, 0x40);
        if (SWORD(p, 0x38) >= SWORD(p, 4) - (SWORD(p, 0x40) + 1)) {
            start = SWORD(p, 4) - SWORD(p, 0x10);
        }
    }
    i = 0;
    if (SWORD(p, 0x10) > 0) {
        u32 *item = (u32 *)owner + start;
        int y = 4;
        do {
            void *name = String_New(0xFF, 0x6D);
            void *number = String_New(0xFF, 0x6D);
            void *message = NewString_ReadMsgData(PTR(owner, 0x48), 0x63);
            int width;
            String16_FormatInteger(number, start + i + 1, 2, 1, 1);
            CopyU16ArrayToString(name, *(u16 **)((u8 *)item + 0x2608));
            ov40_02230DCC(owner, name);
            width = FontID_String_GetWidth(0, number, 0);
            AddTextPrinterParameterizedWithColor(window, 0, number, 0x10 - width, y, 0xFF, 0xF0D00, NULL);
            AddTextPrinterParameterizedWithColor(window, 0, message, 0x10, y, 0xFF, 0xF0D00, NULL);
            AddTextPrinterParameterizedWithColor(window, 0, name, 0x16, y, 0xFF, 0xF0D00, NULL);
            String_Delete(name);
            String_Delete(number);
            String_Delete(message);
            item++;
            i++;
            y += 0x18;
        } while (i < SWORD(p, 0x10));
    }
    CopyWindowToVram(window);
    SWORD(p, 0xC) = SWORD(p, 0x38);
    return 0;
}
