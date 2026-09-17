#include "overlay83_party_ui_gap28_internal.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
void ov83_02244AD8(u8 *work, void *window, int x, int y, u8 font) {
    void *profile = Save_PlayerData_GetProfile(PTR(work, 0x2bc));
    void *string = String_New(8, 0x6b);
    u32 color;
    CopyU16ArrayToString(string, PlayerProfile_GetNamePtr(profile));
    color = PlayerProfile_GetTrainerGender(profile) == 0 ? 0x70800 : 0x30400;
    AddTextPrinterParameterizedWithColor(window, font, string, x, y, 0xff, color, NULL);
    String_Delete(string);
}
void ov83_02244B40(u8 *work, void *window, int x, int y) {
    void *profile = sub_02034818(sub_0203769C(work) ^ 1);
    u32 color = PlayerProfile_GetTrainerGender(profile) == 0 ? 0x70800 : 0x30400;
    BufferPlayersName(PTR(work, 0x24), 0, profile);
    ov83_0224484C(work, window, 1, x, y, 0xff, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, 0);
}
void ov83_02244BA8(void *work, void *window, int value, int x, u8 y, u8 kind) {
    int msg, c1, c2, c3;
    if (kind == 0) {
        msg = 0x40;
        c1 = 7;
        c2 = 8;
        c3 = 0;
    } else if (kind == 1) {
        msg = 0x41;
        c1 = 3;
        c2 = 4;
        c3 = 0;
    } else {
        return;
    }
    ov83_0224484C(work, window, msg, value, x, 0xff, c1, c2, c3, y);
}
void ov83_02244BEC(u8 *work) {
    work[0xa] = ov83_022447E0(work, work + 0x60, 5, 0, 5, 0xff, 1, 2, 0, 0);
    ov83_02247944(work + 0xd0, Options_GetFrame(PTR(work, 0x2b8)));
    work[0xa] = ov83_022447E0(work, work + 0xd0, 4, 1, 1, 0xff, 1, 2, 0xf, 1);
}
