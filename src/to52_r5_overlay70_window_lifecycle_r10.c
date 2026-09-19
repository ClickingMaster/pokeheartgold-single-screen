#include "to52_r5_overlay70_window_lifecycle_r10_private.h"

void ov70_022394B8(Ov70Work *work) {
    int i;
    int baseTile;
    int y;
    u8 *window;

    AddWindowParameterized(PTR_AT(work, 4), work + 0xF38, 1, 2, 1, 0x1C, 2, 0xD, 0x28);
    FillWindowPixelBuffer(work + 0xF38, 0);
    AddTextPrinterParameterizedWithColor(work + 0xF38, 1, PTR_AT(work, 0xBC0), 0, 2, 0, 0xF0600, NULL);

    i = 0;
    baseTile = 0x61;
    y = 6;
    window = work + 0xF58;
    for (; i < 3; i++) {
        AddWindowParameterized(PTR_AT(work, 4), window, 1, 9, (u8)y, 0xF, 2, 1, (u16)baseTile);
        FillWindowPixelBuffer(window, 0);
        baseTile += 0x1E;
        y += 5;
        window += 0x10;
    }

    AddWindowParameterized(PTR_AT(work, 4), work + 0xF18, 0, 2, 0x15, 0x1B, 2, 0xD, 0x60);
    FillWindowPixelBuffer(work + 0xF18, 0xF);
    AddWindowParameterized(PTR_AT(work, 4), work + 0x1158, 0, 2, 0x13, 0x1B, 4, 0xD, 0x96);
    FillWindowPixelBuffer(work + 0x1158, 0xF);
    ov70_02239D44(work, 0);
}

void ov70_022395C4(Ov70Work *work) {
    int i;
    u8 *window;

    RemoveWindow(work + 0x1198);
    RemoveWindow(work + 0x1158);
    RemoveWindow(work + 0xF18);
    window = work + 0xF58;
    for (i = 0; i < 3; i++) {
        RemoveWindow(window);
        window += 0x10;
    }
    RemoveWindow(work + 0xF38);
}

void ov70_02239614(Ov70Work *work) {
    PTR_AT(work, 0xBBC) = String_New(0xB4, 0x3D);
    PTR_AT(work, 0xBC0) = NewString_ReadMsgData(PTR_AT(work, 0xBA0), 0x27);
}
