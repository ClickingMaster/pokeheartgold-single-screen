#include "to58_overlay112_mid_private.h"

int ov112_021EC208(Ov112State *p) {
    FillBgTilemapRect((void *)W(p,0x18),2,0,0,0,0x20,0x18,0x10);
    FillBgTilemapRect((void *)W(p,0x18),5,0,0,0,0x40,0x18,0x10);
    BgCommitTilemapBufferToVram((void *)W(p,0x18),2);
    BgCommitTilemapBufferToVram((void *)W(p,0x18),5);
    p->textPrinter = ov112_021EA08C(p,2,0x1f);
    FillBgTilemapRect((void *)W(p,0x18),2,0,0,0,0x20,0x18,0x10);
    return 0x15;
}
