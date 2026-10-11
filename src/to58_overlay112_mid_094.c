#include "to58_overlay112_mid_private.h"

int ov112_021EC70C(Ov112State *p) {
    FillBgTilemapRect((void *)W(p,0x18),1,0,0,0,0x20,0x18,0x10);
    FillBgTilemapRect((void *)W(p,0x18),2,0,0,0,0x20,0x18,0x10);
    BgCommitTilemapBufferToVram((void *)W(p,0x18),2);
    PokewalkerApp_HideSprite(p,3);
    PokewalkerApp_HideSprite(p,4);
    p->textPrinter = PokewalkerApp_StartMessageBox(p,0,0xc);
    return 8;
}

int ov112_021EC778(Ov112State *p) {
    if (TextPrinterCheckActive((u8)p->textPrinter) == 0) {
        PokewalkerApp_RemoveWindowBank(p, 1);
        PokewalkerApp_RemoveWindowBank(p, 0);
        PokewalkerApp_AddWindowBank(p, 3);
        PokewalkerApp_AddWindowBank(p, 2);
        return 9;
    }
    return 8;
}
