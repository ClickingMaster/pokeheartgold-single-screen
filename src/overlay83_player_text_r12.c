#include "overlay83_player_text_r12_private.h"

void ov83_02240C8C(void *work, void *window, u32 x, u32 y, u8 fontId) {
    void *profile = Save_PlayerData_GetProfile(PTR_AT(work, 0x50C));
    void *string = String_New(8, 0x6B);
    u32 color;

    CopyU16ArrayToString(string, PlayerProfile_GetNamePtr(profile));
    if (PlayerProfile_GetTrainerGender(profile) == 0) {
        color = 0x70800;
    } else {
        color = 0x30400;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0xFF, color, NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(string);
}

void ov83_02240CFC(void *work, void *window, u32 x, u32 y) {
    void *profile = sub_02034818(sub_0203769C(work) ^ 1);
    u32 color;

    if (PlayerProfile_GetTrainerGender(profile) == 0) {
        color = 0x70800;
    } else {
        color = 0x30400;
    }
    BufferPlayersName(PTR_AT(work, 0x24), 0, profile);
    ov83_0223FCB4(work, window, 1, x, y, 0xFF, (u8)(color >> 16), (color >> 8) & 0xFF, color & 0xFF, 0);
}

void ov83_02240D64(void *work, void *window, u32 x, u32 y, u8 finalArg, u8 mode) {
    u32 message;
    u32 color1;
    u32 color2;
    u32 color3;

    if (mode == 0) {
        message = 0x56;
        color1 = 7;
        color2 = 8;
        color3 = 0;
    } else if (mode == 1) {
        message = 0x57;
        color1 = 3;
        color2 = 4;
        color3 = 0;
    } else {
        return;
    }
    ov83_0223FCB4(work, window, message, x, y, 0xFF, color1, color2, color3, finalArg);
}
