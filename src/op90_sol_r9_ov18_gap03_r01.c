#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "message_printer.h"
#include "heap.h"
typedef struct { u8 p[0xc]; Window windows[101]; u8 p2[0x854 - 0x65C]; NARC *narc; } Dex;
void ov18_021EE520(Dex *d, u32 win, u32 value);
// @fn ov18_021EE520
void ov18_021EE520(Dex *d, u32 win, u32 value) {
    NNSG2dCharacterData *chr;
    void *buf;
    u8 *pix;
    MessagePrinter *mp;
    u32 i;
    buf = GfGfxLoader_GetCharDataFromOpenNarc(d->narc, 1, TRUE, &chr, HEAP_ID_POKEDEX_APP);
    pix = chr->pRawData;
    for (i = 0; i < 3; i++) {
        BlitBitmapRect(&d->windows[win], pix + 0x300, 0, 0, 8, 8, i * 8, 0, 8, 8, 0xFF);
        BlitBitmapRect(&d->windows[win], pix + 0x20, 0, 0, 8, 8, i * 8, 8, 8, 8, 0xFF);
    }
    Heap_Free(buf);
    mp = MessagePrinter_New(15, 8, 7, HEAP_ID_POKEDEX_APP);
    PrintUIntOnWindow(mp, value, 3, PRINTING_MODE_LEADING_ZEROS, &d->windows[win], 0, 4);
    MessagePrinter_Delete(mp);
}
