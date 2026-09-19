#include "global.h"

#include "bg_window.h"

typedef struct BagWorkRaw {
    u8 raw[0x900];
} BagWorkRaw;

void ov15_021FE1D0(BagWorkRaw *);
void ov15_021FE204(BagWorkRaw *);
void ov15_021FF570(BagWorkRaw *, Window *, void *, void *, u32);
void ov15_021FF4EC(BagWorkRaw *work, s32 a, s32 b);

void ov15_021FF4EC(BagWorkRaw *work, s32 a, s32 b) {
    u32 index = b;
    u8 *context = *(u8 **)&work->raw[0x234];
    u8 *entry = context + 4 + context[0x64] * 12;

    index += a;

    s32 i = 0;
    Window *window = (Window *)&work->raw[0xB4];

    for (; i < 6; i++) {
        ClearWindowTilemapAndScheduleTransfer(window);
        window++;
    }
    ov15_021FE1D0(work);
    ClearWindowTilemapAndScheduleTransfer((Window *)&work->raw[0x64]);
    ov15_021FE204(work);
    ov15_021FF570(work, (Window *)&work->raw[0x174], *(void **)&work->raw[0x350 + index * 4], entry, index);
    ScheduleWindowCopyToVram((Window *)&work->raw[0x174]);
}
