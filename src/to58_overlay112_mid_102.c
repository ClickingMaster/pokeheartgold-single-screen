#include "to58_overlay112_mid_private.h"

int ov112_021ED1A0(Ov112State *p) {
    BgClearTilemapBufferAndCommit((void *)W(p,0x18),4);
    BgClearTilemapBufferAndCommit((void *)W(p,0x18),5);
    BgClearTilemapBufferAndCommit((void *)W(p,0x18),1);
    BgClearTilemapBufferAndCommit((void *)W(p,0x18),2);
    ov112_021E9C10(p,2);
    ov112_021E9C10(p,3);
    ov112_021E9A78(p,0);
    ov112_021E9A78(p,1);
    PokewalkerApp_LoadTilemap(p,2,0xe);
    ov112_021E9F40(p,1);
    ov112_021EA688(p,3);
    ov112_021EA688(p,4);
    ov112_021ED2F4(p);
    p->textPrinter = ov112_021EA08C(p,0,5);
    return 0x17;
}
