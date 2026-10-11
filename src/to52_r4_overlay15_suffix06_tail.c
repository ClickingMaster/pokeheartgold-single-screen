#include "global.h"

#include "bg_window.h"

typedef struct BagWorkRaw {
    u8 raw[0x900];
} BagWorkRaw;

void BagApp_RemoveItemListWindows(BagWorkRaw *);
void BagApp_AddItemActionWindows(BagWorkRaw *);
void BagApp_PrintItemListEntry(BagWorkRaw *, Window *, void *, void *, u32);
void BagApp_DrawSelectedItemEntry(BagWorkRaw *work, s32 a, s32 b);

void BagApp_DrawSelectedItemEntry(BagWorkRaw *work, s32 a, s32 b) {
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
    BagApp_RemoveItemListWindows(work);
    ClearWindowTilemapAndScheduleTransfer((Window *)&work->raw[0x64]);
    BagApp_AddItemActionWindows(work);
    BagApp_PrintItemListEntry(work, (Window *)&work->raw[0x174], *(void **)&work->raw[0x350 + index * 4], entry, index);
    ScheduleWindowCopyToVram((Window *)&work->raw[0x174]);
}
