#pragma force_active on
#include "global.h"
#include "message_printer.h"
#include "party.h"
#include "pokemon.h"
#include "unk_0200CE7C.h"

typedef struct Ov83Work {
    u8 pad0[9];
    u8 type;
    u8 padA[0x504 - 0xA];
    MessagePrinter *printer;
    u8 pad508[0x7A4 - 0x508];
    Party *party;
} Ov83Work;

BOOL ov80_02237D8C(u8 type);
void ov83_02240D64(Ov83Work *work, Window *window, u32 x, u32 y, int unk, u8 gender);
void ov83_022401A4(Ov83Work *work, Window *window, u32 slot, u8 mode);

// @fn ov83_022401A4
void ov83_022401A4(Ov83Work *work, Window *window, u32 slot, u8 mode) {
    unsigned int baseX, genderX, levelX, y;
    Pokemon *mon;

    if (ov80_02237D8C(work->type) == FALSE) {
        baseX = 0x28;
        genderX = 0x50;
    } else {
        baseX = 8;
        genderX = 0x30;
    }
    mon = Party_GetMonByIndex(work->party, slot);
    if (mode == 0) {
        levelX = slot * 64 + baseX;
        y = 1;
        genderX = genderX + slot * 64;
    } else {
        levelX = 4;
        y = 0;
        genderX = 0x30;
    }
    sub_0200CE7C(work->printer, 1, GetMonData(mon, MON_DATA_LEVEL, NULL), 3, PRINTING_MODE_LEFT_ALIGN, window, levelX, y);
    ov83_02240D64(work, window, genderX, y, 0, GetMonData(mon, MON_DATA_GENDER, NULL));
}
