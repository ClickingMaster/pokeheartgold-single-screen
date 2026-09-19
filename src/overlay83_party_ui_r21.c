#include "overlay83_party_ui_r21_private.h"

void ov83_022428A8(void *work, u32 selected) {
    void *screenRaw;
    NNSG2dScreenData *screen;
    u32 i;
    u8 *cursor;
    u32 entryOffset;
    u32 value;

    for (i = 0, cursor = work; i < 6; i++, cursor += 4) {
        ClearWindowTilemapAndScheduleTransfer((Window *)((u8 *)work + 0x50 + ((i + 0x30) << 4)));
        if (i != selected) {
            ov83_0224755C(PTR_AT(cursor, 0x784), 0);
        } else {
            ov83_02247568(PTR_AT(cursor, 0x784), 0x44, 0x4C);
        }
    }
    ov83_0224755C(PTR_AT(work, 0x77C), 0);
    screenRaw = GfGfxLoader_GetScrnDataFromOpenNarc(PTR_AT(work, 0x7A8), 0x27, TRUE, &screen, HEAP_ID_107);
    LoadRectToBgTilemapRect(PTR_AT(work, 0x4C), 2, screen->rawData, 0, 0, 0x20, 0x18);
    ScheduleBgTilemapBufferTransfer(PTR_AT(work, 0x4C), 2);
    Heap_Free(screenRaw);

    FillWindowPixelBuffer((Window *)((u8 *)work + 0x3B0), 0);
    FillWindowPixelBuffer((Window *)((u8 *)work + 0x3C0), 0);
    entryOffset = 6 * S16_AT(work, 0x862);
    entryOffset += selected;
    entryOffset <<= 3;
    AddTextPrinterParameterizedWithColor((u8 *)work + 0x3B0, 0, PTR_AT(PTR_AT(work, 0x4DC), entryOffset), 0, 0, 0xFF, 0x10200, NULL);
    value = ov83_02240EC4(work, (u16)U32_AT(PTR_AT(work, 0x4DC), entryOffset + 4), U8_AT(work, 0x13));
    ov83_02240C48(work, 0, value, 2, 0);
    ov83_02241DD8(work, (u8 *)work + 0x3C0, PTR_AT(work, 0x20), 0x68, 0, 0, 0, 0x10200, 0);
    ScheduleWindowCopyToVram((Window *)((u8 *)work + 0x3B0));
    ScheduleWindowCopyToVram((Window *)((u8 *)work + 0x3C0));
}

void ov83_022429E4(void *work) {
    void *screenRaw;
    NNSG2dScreenData *screen;
    u32 i;
    u8 *cursor;
    u32 base;
    u16 xy[2];

    ClearWindowTilemapAndScheduleTransfer((Window *)((u8 *)work + 0x3B0));
    ClearWindowTilemapAndScheduleTransfer((Window *)((u8 *)work + 0x3C0));
    screenRaw = GfGfxLoader_GetScrnDataFromOpenNarc(PTR_AT(work, 0x7A8), 0x26, TRUE, &screen, HEAP_ID_107);
    LoadRectToBgTilemapRect(PTR_AT(work, 0x4C), 2, screen->rawData, 0, 0, 0x20, 0x18);
    ScheduleBgTilemapBufferTransfer(PTR_AT(work, 0x4C), 2);
    Heap_Free(screenRaw);

    base = 6 * S16_AT(work, 0x862);
    for (i = 0, cursor = work; i < 6; i++, cursor += 4) {
        ov83_02242894(i, &xy[1], &xy[0]);
        ov83_02247568(PTR_AT(cursor, 0x784), xy[1], xy[0]);
        if (base + i < U8_AT(work, 0x861)) {
            ov83_0224755C(PTR_AT(cursor, 0x784), 1);
        }
    }
    ov83_0224755C(PTR_AT(work, 0x77C), 1);
    ov83_02240664(work);
    ov83_022407FC(work);
    ov83_0224088C(work);
}
