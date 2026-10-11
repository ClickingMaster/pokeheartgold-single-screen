#include "private.h"

int ov40_022325B0(void *p) {
    void *w = PTR(p, 0x860);
    switch (WORD(p, 8)) {
    case 0:
        if (WORD(w, 0xC) == 0) {
            ov40_0223064C((u8 *)w + 0x9C, p);
        } else {
            ov40_0222E7B8((u8 *)w + 0x10, p);
        }
        GfGfx_EngineATogglePlanes(4, 0);
        WORD(p, 8)
        ++;
        break;
    case 1:
        if (ov40_0222DA00(w, (u8 *)w + 4, 1, 1)) {
            WORD(p, 8)
            ++;
        }
        break;
    case 2:
        ov40_02230964(p, 1);
        if (WORD(w, 0xC) == 0) {
            ov40_0222E79C((u8 *)w + 0x10, p);
            ov40_0222E7DC((u8 *)w + 0x10, 0);
            ov40_022307DC(p, 0x50, 3);
        } else {
            ov40_02230638((u8 *)w + 0x9C, p);
            ov40_022306A0((u8 *)w + 0x9C, 0);
            ov40_022307DC(p, ov40_02230410((u8 *)w + 0x9C), 3);
        }
        ov40_02230964(p, 0);
        WORD(p, 8)
        ++;
        break;
    case 3:
        if (ov40_0222DA00(w, (u8 *)w + 4, 0, 1)) {
            if (WORD(w, 0xC) == 0) {
                ov40_0222E7DC((u8 *)w + 0x10, 1);
            } else {
                ov40_022306A0((u8 *)w + 0x9C, 1);
            }
            GfGfx_EngineBTogglePlanes(4, 1);
            GfGfx_EngineATogglePlanes(4, 1);
            WORD(p, 8)
            ++;
        }
        break;
    default:
        WORD(w, 0xC) ^= 1;
        ov40_02232FEC(p, WORD(w, 0xC) + 0x79);
        VsRecorder_SetSubState(p, 3);
        break;
    }
    return 0;
}

void ov40_022326DC(void *p) {
    void *w = PTR(p, 0x860);
    void *win = (u8 *)w + 0x178;
    void *str;
    InitWindow(win);
    AddWindowParameterized(PTR(p, 0x24), win, 6, 4, 6, 0xA, 2, 0xE, 1);
    FillWindowPixelBuffer(win, 0);
    str = NewString_ReadMsgData(PTR(p, 0x48), 0x82);
    AddTextPrinterParameterizedWithColor(win, 0, str, ov40_022306C0(win, str), 0, 0xFF, 0xF0D00, NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(win);
    win = (u8 *)w + 0x188;
    InitWindow(win);
    AddWindowParameterized(PTR(p, 0x24), win, 6, 0x12, 6, 0xA, 2, 0xE, 0x15);
    FillWindowPixelBuffer(win, 0);
    str = NewString_ReadMsgData(PTR(p, 0x48), 0x83);
    AddTextPrinterParameterizedWithColor(win, 0, str, ov40_022306C0(win, str), 0, 0xFF, 0xF0D00, NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(win);
}
