#include "to53_overlay112_w09_private.h"


void ov112_021E90FC(Window *win, void *p) {
    String *s = PlayerProfile_GetPlayerName_NewString(Save_PlayerData_GetProfile(*(void **)((u8 *)p + 0x20)), 0x9A);
    FontID_Alloc(5, 0x9A);
    AddTextPrinterParameterizedWithColor(win, 5, s, 2, 0, 0xFF, 0x30100, 0);
    FontID_Release(5);
    String_Delete(s);
}
