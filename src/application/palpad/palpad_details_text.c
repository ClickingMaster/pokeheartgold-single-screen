#include "global.h"

#include "application/palpad/palpad_internal.h"

#include "font.h"
#include "text.h"

// Text helpers of the details screen; this file links after the overlay's data.

#pragma force_active on

void PalPadDetailsSlot_ClearBg(PalPadGfx *gfx, PalPadDetailsSlot *slot) {
    int y = slot->layout.y;
    int x;

    if (y < 0) {
        y = -y;
    }
    x = slot->layout.x;
    if (x < 0) {
        x = -x;
    }
    FillBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_3, 0, x, y, 32, 24, 0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_3);
}

// align 0: left, 1: centred on x, 2: right-aligned to x
void PalPadDetailsSlot_Print(PalPadDetailsSlot *slot, u32 page, u32 window, PalPadShared *shared, PalPadGfx *gfx, int msgId, int x, int y, u32 color, String *text, String *buffer, int align) {
    ReadMsgDataIntoString(gfx->msgData, msgId, buffer);
    StringExpandPlaceholders(gfx->msgFormat, text, buffer);
    switch (align) {
    case 1:
        x -= (FontID_String_GetWidth(1, text, 0) + 1) / 2;
        break;
    case 2:
        x -= FontID_String_GetWidth(1, text, 0);
        break;
    }
    AddTextPrinterParameterizedWithColor(&slot->windows[page][window], 1, text, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
}

#pragma force_active reset

void PalPadDetails_PrintProfileText(Window *window, PalPadShared *shared, PalPadGfx *gfx, int msgId, int x, int y, u32 color) {
    ReadMsgDataIntoString(gfx->msgData, msgId, gfx->pageBuffer);
    StringExpandPlaceholders(gfx->msgFormat, gfx->pageText, gfx->pageBuffer);
    AddTextPrinterParameterizedWithColor(window, 1, gfx->pageText, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
}

void PalPadDetails_MovePage(PalPadDetails *details, int delta) {
    int pageCount = details->hasFrontierPages == TRUE ? 8 : 3;

    if (delta > 0) {
        details->prevPage = details->page;
        details->page = (details->page + delta) % pageCount;
    } else if (delta < 0) {
        details->prevPage = details->page;
        details->page += delta;
        if (details->page < 0) {
            details->page += pageCount;
        }
    }
}
