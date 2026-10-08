#pragma force_active on
#include "global.h"
#include "message_printer.h"
#include "party.h"
#include "pokemon.h"

typedef struct Ov83Work {
    u8 pad0[9];
    u8 type;
    u8 padA[0x504 - 0xA];
    MessagePrinter *printer;
    u8 pad508[0x7A4 - 0x508];
    Party *party;
} Ov83Work;

BOOL ov80_02237D8C(u8 type);
void ov83_022400BC(Ov83Work *work, Window *window, int slot, int mode);

// @fn ov83_022400BC
void ov83_022400BC(Ov83Work *work, Window *window, int slot, int mode) {
    u16 maxHpX;
    u16 slashX;
    Pokemon *mon;
    u16 base;
    u16 hpX;
    u8 y;

    mon = Party_GetMonByIndex(work->party, slot);

    if (ov80_02237D8C(work->type) == FALSE) {
        base = 0x24;
    } else {
        base = 4;
    }
    if (mode == 0) {
        hpX = base + slot * 64;
        y = 1;
        slashX = base + 0x18 + slot * 64;
        maxHpX = base + 0x20 + slot * 64;
    } else {
        slashX = 0x1C;
        hpX = 4;
        y = 0;
        maxHpX = 0x24;
    }
    PrintUIntOnWindow(work->printer, GetMonData(mon, MON_DATA_HP, NULL), 3, PRINTING_MODE_RIGHT_ALIGN, window, hpX, y);
    sub_0200CDAC(work->printer, 0, window, slashX, y);
    PrintUIntOnWindow(work->printer, GetMonData(mon, MON_DATA_MAX_HP, NULL), 3, PRINTING_MODE_LEFT_ALIGN, window, maxHpX, y);
}
