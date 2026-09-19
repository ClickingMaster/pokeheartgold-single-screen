#include "to52_r5_overlay96_r90_gap4_private.h"

void ov96_022088AC(void *w, u32 i) {
    void *str;
    FillWindowPixelBuffer((u8 *)w + 0xc, 0);
    BufferString(PTR(w, 0x20), 0, PTR(w, 0x1a8 + i * 4), 2, 1, 2);
    str = ReadMsgData_ExpandPlaceholders(PTR(w, 0x20), PTR(w, 0x1c), 0x9a, *(u32 *)w);
    AddTextPrinterParameterizedWithColor((u8 *)w + 0xc, 0, str, 0, 0, 0xff, 0xf0e00, NULL);
    String_Delete(str);
    CopyWindowToVram((u8 *)w + 0xc);
}
