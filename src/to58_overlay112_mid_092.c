#include "to58_overlay112_mid_private.h"

int ov112_021EC640(Ov112State *p) {
    BgClearTilemapBufferAndCommit((void *)W(p,0x18),1);
    PokewalkerApp_LoadTilemap(p,2,0xe);
    PokewalkerApp_PrintExit(p,1);
    p->textPrinter = PokewalkerApp_StartMessageBox(p,0,5);
    ov112_021ED2F4(p);
    return 2;
}

int ov112_021EC67C(Ov112State *p) {
    if (TextPrinterCheckActive((u8)p->textPrinter) == 0) return 26;
    return 2;
}
