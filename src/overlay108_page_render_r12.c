#include "overlay108_page_render_r12_private.h"

void ov108_021E77D4(Ov108Work12 *work) {
    u32 x;
    FillWindowPixelBuffer(&work->windows[3], 2);
    BufferIntegerAsString(work->format, 0, work->page + 1, 1, 1, 1);
    StringExpandPlaceholders(work->format, work->string, work->strings01[4]);
    x = (GetWindowWidth(&work->windows[3]) * 8 - FontID_String_GetWidth(0, work->string, 0)) / 2;
    AddTextPrinterParameterizedWithColor(&work->windows[3], 0, work->string, x, 0, 0, 0x30102, 0);
    ScheduleBgTilemapBufferTransfer(work->bgConfig, 3);
    Sprite_SetAnimActiveFlag(work->sprites[4], FALSE);
    if (work->page == 0) {
        Sprite_SetAnimCtrlSeq(work->sprites[4], 6);
    } else {
        Sprite_SetAnimCtrlSeq(work->sprites[4], 4);
    }
    Sprite_SetAnimActiveFlag(work->sprites[5], FALSE);
    if (work->page == 1) {
        Sprite_SetAnimCtrlSeq(work->sprites[5], 7);
    } else {
        Sprite_SetAnimCtrlSeq(work->sprites[5], 5);
    }
}
