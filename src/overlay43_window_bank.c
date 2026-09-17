#include "overlay43_window_bank_private.h"

#define ABS_COORD(value) ((value) < 0 ? -(value) : (value))

void ov43_0222DE14(Ov43WindowBank *bank, BgConfig **bgConfig, u32 bankIndex, u32 count, const Ov43WindowSpec *specs, u32 heapId) {
    u32 i;
    u32 baseTile;

    bank->windows[bankIndex] = AllocWindows(heapId, (u8)count);
    bank->counts[bankIndex] = count;
    baseTile = (u16)bank->baseTile;
    for (i = 0; i < count; i++) {
        InitWindow((u8 *)bank->windows[bankIndex] + i * 0x10);
        AddWindowParameterized(*bgConfig, (u8 *)bank->windows[bankIndex] + i * 0x10, 3, (u8)(specs[i].x + ABS_COORD(bank->xOffset)), (u8)(specs[i].y + ABS_COORD(bank->yOffset)), specs[i].width, specs[i].height, 11, baseTile);
        FillWindowPixelBuffer((u8 *)bank->windows[bankIndex] + i * 0x10, 0);
        baseTile = (u16)(baseTile + specs[i].width * specs[i].height);
    }
}

void ov43_0222DED0(Ov43WindowBank *bank, u32 bankIndex) {
    if (bank->windows[bankIndex] != NULL) {
        WindowArray_Delete(bank->windows[bankIndex], (u8)bank->counts[bankIndex]);
        bank->windows[bankIndex] = NULL;
        bank->counts[bankIndex] = 0;
    }
}

void ov43_0222DEF8(Ov43WindowBank *bank, u32 bankIndex) {
    u32 i;

    for (i = 0; i < bank->counts[bankIndex]; i++) {
        ScheduleWindowCopyToVram((u8 *)bank->windows[bankIndex] + i * 0x10);
    }
}

void ov43_0222DF1C(Ov43WindowBank *bank, u32 bankIndex) {
    u32 i;

    for (i = 0; i < bank->counts[bankIndex]; i++) {
        ClearWindowTilemapAndScheduleTransfer((u8 *)bank->windows[bankIndex] + i * 0x10);
        FillWindowPixelBuffer((u8 *)bank->windows[bankIndex] + i * 0x10, 0);
    }
}

void ov43_0222DF4C(void *arg0, void *arg1, void *arg2, u32 page, u32 unused, BOOL alternate, void *arg6) {
    ov43_0222DF90(arg0, arg2);
    if (alternate) {
        ov43_0222F12C[page](arg0, arg1, arg2, arg6);
    } else {
        _0222F0E0[page](arg0, arg1, arg2, arg6);
    }
}
