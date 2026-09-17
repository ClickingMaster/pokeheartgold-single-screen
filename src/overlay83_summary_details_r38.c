#include "overlay83_summary_details_internal.h"

void ov83_02246114(u8 *work, int copyMode) {
    int displayMode;
    int monIndex;
    u8 i;
    void *string;
    int width1;
    int width2;
    u8 *moveSlot;

    displayMode = ov83_0224777C(PTR_AT(work, 0x2BC), work[9], 2);
    monIndex = ov83_02247768(work[0x14], work[0xD]);

    FillWindowPixelBuffer(work + 0x110, 0);
    FillWindowPixelBuffer(work + 0x120, 0);
    FillWindowPixelBuffer(work + 0x140, 0);
    FillWindowPixelBuffer(work + 0x160, 0);
    FillWindowPixelBuffer(work + 0x180, 0);
    FillWindowPixelBuffer(work + 0x1A0, 0);
    FillWindowPixelBuffer(work + 0x1C0, 0);
    FillWindowPixelBuffer(work + 0x1E0, 0);
    FillWindowPixelBuffer(work + 0x200, 0);
    FillWindowPixelBuffer(work + 0x220, 0);
    FillWindowPixelBuffer(work + 0x240, 0);
    FillWindowPixelBuffer(work + 0x260, 0);
    FillWindowPixelBuffer(work + 0x270, 0);

    if (!((Ov83R38Flags *)(work + 0x5C6))->flag) {
        if (((Ov83R38Flags *)(work + 0x5C6))->gender == 0) {
            ov83_022479E4(work + 0x120, PTR_AT(work, 0x20), 0x40, 0, 0, 0, 0x50600, 0);
        } else if (((Ov83R38Flags *)(work + 0x5C6))->gender == 1) {
            ov83_022479E4(work + 0x120, PTR_AT(work, 0x20), 0x41, 0, 0, 0, 0x30400, 0);
        }
    }

    ov83_02244A98(work, 0, work[0x5C7], 3, 0);
    ov83_02245D08(work, work + 0x140, PTR_AT(work, 0x20), 0x48, 0, 0, 0, 0x10200, 0);

    if (((u8 *)PTR_AT(work, 0x54C))[monIndex] != 0) {
        BufferBoxMonSpeciesName(PTR_AT(work, 0x24), 0, Mon_GetBoxMon(PTR_AT(work, 0x5BC)));
        ov83_02245D08(work, work + 0x110, PTR_AT(work, 0x20), 0x45, 0, 0, 0, 0x10200, 0);
        ov83_02244A98(work, 0, U16_AT(work, 0x5D0), 3, 0);
        ov83_02244A98(work, 1, U16_AT(work, 0x5D2), 3, 0);
        ov83_02245D08(work, work + 0x1C0, PTR_AT(work, 0x20), 0x4E, GetWindowWidth(work + 0x1C0) * 8, 0, 0, 0x10200, 1);
    } else {
        ov83_022479E4(work + 0x110, PTR_AT(work, 0x20), 0x4B, 0, 0, 0, 0x10200, 0);
        ov83_022479E4(work + 0x1C0, PTR_AT(work, 0x20), 0x4D, GetWindowWidth(work + 0x1C0) * 8, 0, 0, 0x10200, 1);
    }

    if (((u8 *)PTR_AT(work, 0x554))[monIndex] != 0) {
        BufferAbilityName(PTR_AT(work, 0x24), 0, work[0x5C8]);
        ov83_02245D08(work, work + 0x160, PTR_AT(work, 0x20), 0x35, 0, 0, 0, 0x10200, 0);
        BufferNatureName(PTR_AT(work, 0x24), 0, work[0x5C9]);
        ov83_02245D08(work, work + 0x180, PTR_AT(work, 0x20), 0x33, 0, 0, 0, 0x10200, 0);
        BufferItemName(PTR_AT(work, 0x24), 0, U16_AT(work, 0x5CA));
        ov83_02245D08(work, work + 0x1A0, PTR_AT(work, 0x20), 0x31, 0, 0, 0, 0x10200, 0);
        ov83_02244A98(work, 0, U16_AT(work, 0x5D4), 3, 0);
        ov83_02245D08(work, work + 0x1E0, PTR_AT(work, 0x20), 0x37, GetWindowWidth(work + 0x1E0) * 8, 0, 0, 0x10200, 1);
        ov83_02244A98(work, 0, U16_AT(work, 0x5D6), 3, 0);
        ov83_02245D08(work, work + 0x200, PTR_AT(work, 0x20), 0x3B, GetWindowWidth(work + 0x200) * 8, 0, 0, 0x10200, 1);
        ov83_02244A98(work, 0, U16_AT(work, 0x5D8), 3, 0);
        ov83_02245D08(work, work + 0x220, PTR_AT(work, 0x20), 0x39, GetWindowWidth(work + 0x220) * 8, 0, 0, 0x10200, 1);
        ov83_02244A98(work, 0, U16_AT(work, 0x5DA), 3, 0);
        ov83_02245D08(work, work + 0x240, PTR_AT(work, 0x20), 0x3D, GetWindowWidth(work + 0x240) * 8, 0, 0, 0x10200, 1);
        ov83_02244A98(work, 0, U16_AT(work, 0x5DC), 3, 0);
        ov83_02245D08(work, work + 0x260, PTR_AT(work, 0x20), 0x3F, GetWindowWidth(work + 0x260) * 8, 0, 0, 0x10200, 1);
    } else {
        ov83_022479E4(work + 0x160, PTR_AT(work, 0x20), 0x4B, 0, 0, 0, 0x10200, 0);
        ov83_022479E4(work + 0x180, PTR_AT(work, 0x20), 0x4B, 0, 0, 0, 0x10200, 0);
        ov83_022479E4(work + 0x1A0, PTR_AT(work, 0x20), 0x4B, 0, 0, 0, 0x10200, 0);
        ov83_022479E4(work + 0x1E0, PTR_AT(work, 0x20), 0x4A, GetWindowWidth(work + 0x1E0) * 8, 0, 0, 0x10200, 1);
        ov83_022479E4(work + 0x200, PTR_AT(work, 0x20), 0x4A, GetWindowWidth(work + 0x200) * 8, 0, 0, 0x10200, 1);
        ov83_022479E4(work + 0x220, PTR_AT(work, 0x20), 0x4A, GetWindowWidth(work + 0x220) * 8, 0, 0, 0x10200, 1);
        ov83_022479E4(work + 0x240, PTR_AT(work, 0x20), 0x4A, GetWindowWidth(work + 0x240) * 8, 0, 0, 0x10200, 1);
        ov83_022479E4(work + 0x260, PTR_AT(work, 0x20), 0x4A, GetWindowWidth(work + 0x260) * 8, 0, 0, 0x10200, 1);
    }

    if (displayMode == 1) {
        ov83_022479E4(work + 0x270, PTR_AT(work, 0x20), 0x4C, 0, 0, 0, 0x10200, 0);
    } else if (((u8 *)PTR_AT(work, 0x558))[monIndex] == 0) {
        for (i = 0; i < 4; i++) {
            ov83_022479E4(work + 0x270, PTR_AT(work, 0x20), 0x4B, 0, i * 16, 0, 0x10200, 0);
            string = NewString_ReadMsgData(PTR_AT(work, 0x20), 0x44);
            width1 = (u8)(FontID_String_GetWidth(0, string, 0) >> 1);
            ov83_02247998(work + 0x270, string, 0x78, i * 16, 0, 0x10200, 2);
            String_Delete(string);
            string = NewString_ReadMsgData(PTR_AT(work, 0x20), 0x49);
            width2 = FontID_String_GetWidth(0, string, 0);
            ov83_02247998(work + 0x270, string, 0x78 - width1 - width2, i * 16, 0, 0x10200, 0);
            String_Delete(string);
            ov83_022479E4(work + 0x270, PTR_AT(work, 0x20), 0x49, 0x78 + width1, i * 16, 0, 0x10200, 0);
        }
    } else {
        for (i = 0; i < 4; i++) {
            moveSlot = work + i * 2;
            BufferMoveName(PTR_AT(work, 0x24), i, U16_AT(moveSlot, 0x5E0));
            ov83_02245D08(work, work + 0x270, PTR_AT(work, 0x20), 0x54 + i, 0, i * 16, 0, 0x10200, 0);
            if (U16_AT(moveSlot, 0x5E0) == 0) {
                ov83_022479E4(work + 0x270, PTR_AT(work, 0x20), 0x5A, 0x78, i * 16, 0, 0x10200, 2);
            } else {
                string = NewString_ReadMsgData(PTR_AT(work, 0x20), 0x44);
                width1 = (u8)(FontID_String_GetWidth(0, string, 0) >> 1);
                ov83_02247998(work + 0x270, string, 0x78, i * 16, 0, 0x10200, 2);
                String_Delete(string);
                ov83_02244A98(work, 0, work[0x5E8 + i], 2, 0);
                string = NewString_ReadMsgData(PTR_AT(work, 0x20), 0x59);
                StringExpandPlaceholders(PTR_AT(work, 0x24), PTR_AT(work, 0x28), string);
                width2 = FontID_String_GetWidth(0, PTR_AT(work, 0x28), 0);
                ov83_02247998(work + 0x270, PTR_AT(work, 0x28), 0x78 - width1 - width2, i * 16, 0, 0x10200, 0);
                String_Delete(string);
                ov83_02244A98(work, 0, work[0x5EC + i], 2, 0);
                ov83_02245D08(work, work + 0x270, PTR_AT(work, 0x20), 0x59, 0x78 + width1, i * 16, 0, 0x10200, 0);
            }
        }
    }

    if (copyMode == 1) {
        for (i = 12; i <= 0x22; i++) {
            ScheduleWindowCopyToVram(work + 0x50 + i * 16);
        }
    } else {
        CopyWindowPixelsToVram_TextMode(work + 0x110);
        CopyWindowPixelsToVram_TextMode(work + 0x120);
        CopyWindowPixelsToVram_TextMode(work + 0x140);
        CopyWindowPixelsToVram_TextMode(work + 0x160);
        CopyWindowPixelsToVram_TextMode(work + 0x180);
        CopyWindowPixelsToVram_TextMode(work + 0x1A0);
        CopyWindowPixelsToVram_TextMode(work + 0x1C0);
        CopyWindowPixelsToVram_TextMode(work + 0x1E0);
        CopyWindowPixelsToVram_TextMode(work + 0x200);
        CopyWindowPixelsToVram_TextMode(work + 0x220);
        CopyWindowPixelsToVram_TextMode(work + 0x240);
        CopyWindowPixelsToVram_TextMode(work + 0x260);
        CopyWindowPixelsToVram_TextMode(work + 0x270);
    }
}
