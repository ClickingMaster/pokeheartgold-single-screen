#include "to50_overlay70_window_setup_modes_private.h"

void ov70_0224190C(Ov70WindowSetupWork *work, int mode) {
    int i;
    switch (mode) {
    case 4:
        for (i = 0; i < 9; i++) {
            AddWindowParameterized(work->bgConfig, &work->windows[i], work->bgId, ov70_02245E0E[i].x, ov70_02245E0E[i].aux, 3, 2, 1, 0x30 + i * 6);
            FillWindowPixelBuffer(&work->windows[i], 0x22);
        }
        AddWindowParameterized(work->bgConfig, &work->windows[14], work->bgId, 9, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer(&work->windows[14], 0x22);
        break;
    case 5:
        AddWindowParameterized(work->bgConfig, &work->windows[0], work->bgId, 1, 2, 3, 2, 1, 0x30);
        FillWindowPixelBuffer(&work->windows[0], 0x22);
        for (i = 1; i < 4; i++) {
            AddWindowParameterized(work->bgConfig, &work->windows[i], work->bgId, ov70_02245E0E[i - 1].x, ov70_02245E0E[i - 1].aux, 2, 2, 1, 0x36 + (i - 1) * 4);
            FillWindowPixelBuffer(&work->windows[i], 0x22);
        }
        AddWindowParameterized(work->bgConfig, &work->windows[14], work->bgId, 9, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer(&work->windows[14], 0x22);
        break;
    case 0:
        for (i = 0; i < 4; i++) {
            AddWindowParameterized(work->bgConfig, &work->windows[i], work->bgId, ov70_02245DA2[i].x, ov70_02245DA2[i].aux, 8, 2, 1, 0x30 + i * 0x10);
            FillWindowPixelBuffer(&work->windows[i], 0x22);
        }
        AddWindowParameterized(work->bgConfig, &work->windows[4], work->bgId, 6, 0xe, 5, 1, 1, 0x70);
        FillWindowPixelBuffer(&work->windows[4], 0x22);
        AddWindowParameterized(work->bgConfig, &work->windows[14], work->bgId, 9, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer(&work->windows[14], 0x22);
        break;
    case 6:
        for (i = 0; i < 9; i++) {
            AddWindowParameterized(work->bgConfig, &work->windows[i], work->bgId, ov70_02245E0E[i].x, ov70_02245E0E[i].aux, 3, 2, 1, 0x30 + i * 6);
            FillWindowPixelBuffer(&work->windows[i], 0x22);
        }
        AddWindowParameterized(work->bgConfig, &work->windows[14], work->bgId, 9, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer(&work->windows[14], 0x22);
        AddWindowParameterized(work->bgConfig, &work->windows[15], work->bgId, 2, 2, 9, 2, 1, 0x66);
        FillWindowPixelBuffer(&work->windows[15], 0x22);
        break;
    case 3:
        for (i = 0; i < 5; i++) {
            AddWindowParameterized(work->bgConfig, &work->windows[i], work->bgId, ov70_02245D96[i].x, ov70_02245D96[i].aux, 0x17, 2, 1, 0x30 + i * 0x2e);
            FillWindowPixelBuffer(&work->windows[i], 0x22);
        }
        AddWindowParameterized(work->bgConfig, &work->windows[5], work->bgId, 0xd, 0x10, 5, 1, 1, 0x116);
        FillWindowPixelBuffer(&work->windows[5], 0x22);
        AddWindowParameterized(work->bgConfig, &work->windows[14], work->bgId, 0x18, 0x11, 6, 2, 1, 0x12f);
        FillWindowPixelBuffer(&work->windows[14], 0x22);
        break;
    case 1:
        for (i = 0; i < 3; i++) {
            AddWindowParameterized(work->bgConfig, &work->windows[i], work->bgId, ov70_02245D60[i].x, ov70_02245D60[i].aux, 8, 2, 1, 0x30 + i * 0x10);
            FillWindowPixelBuffer(&work->windows[i], 0x22);
        }
        AddWindowParameterized(work->bgConfig, &work->windows[14], work->bgId, 9, 0x11, 6, 2, 1, 0x60);
        FillWindowPixelBuffer(&work->windows[14], 0x22);
        break;
    case 2:
        for (i = 0; i < 4; i++) {
            AddWindowParameterized(work->bgConfig, &work->windows[i], work->bgId, ov70_02245D6E[i].x, ov70_02245D6E[i].aux, 0xb, 2, 1, 0x30 + i * 0x16);
            FillWindowPixelBuffer(&work->windows[i], 0x22);
        }
        AddWindowParameterized(work->bgConfig, &work->windows[4], work->bgId, 6, 0xe, 5, 1, 1, 0x88);
        FillWindowPixelBuffer(&work->windows[4], 0x22);
        AddWindowParameterized(work->bgConfig, &work->windows[14], work->bgId, 9, 0x11, 6, 2, 1, 0xa1);
        FillWindowPixelBuffer(&work->windows[14], 0x22);
        break;
    }
}
