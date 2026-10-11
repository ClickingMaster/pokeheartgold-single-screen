#include "to58_overlay112_mid_private.h"

int ov112_021EBF70(Ov112State *p) {
    BgClearTilemapBufferAndCommit((void *)W(p,0x18),1);
    FillBgTilemapRect((void *)W(p,0x18),2,0,0,0,0x20,0x18,0x10);
    PokewalkerApp_LoadTilemap(p,2,0xe);
    ov112_021E9F40(p,1);
    ov112_021ED2F4(p);
    p->textPrinter = ov112_021EA08C(p,0,5);
    return 0xe;
}

int ov112_021EBFC8(Ov112State *p) {
    if (TextPrinterCheckActive((u8)p->textPrinter) == 0 && H(p,0x1f2e0) == 0x100) {
        ov112_021EA5A4(p,2);
        return 0xf;
    }
    return 0xe;
}
