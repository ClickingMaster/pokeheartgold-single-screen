#include "to58_overlay112_mid_private.h"

int ov112_021ED1A0(Ov112State *p) {
    BgClearTilemapBufferAndCommit((void *)W(p,0x18),4);
    BgClearTilemapBufferAndCommit((void *)W(p,0x18),5);
    BgClearTilemapBufferAndCommit((void *)W(p,0x18),1);
    BgClearTilemapBufferAndCommit((void *)W(p,0x18),2);
    PokewalkerApp_RemoveWindowBank(p,2);
    PokewalkerApp_RemoveWindowBank(p,3);
    PokewalkerApp_AddWindowBank(p,0);
    PokewalkerApp_AddWindowBank(p,1);
    PokewalkerApp_LoadTilemap(p,2,0xe);
    PokewalkerApp_PrintExit(p,1);
    PokewalkerApp_HideSprite(p,3);
    PokewalkerApp_HideSprite(p,4);
    ov112_021ED2F4(p);
    p->textPrinter = PokewalkerApp_StartMessageBox(p,0,5);
    return 0x17;
}
