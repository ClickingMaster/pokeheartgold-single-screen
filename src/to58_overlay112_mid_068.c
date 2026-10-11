#include "to58_overlay112_mid_private.h"

int ov112_021EB864(Ov112State *p) {
    PokewalkerApp_HideCursor(p);
    FillBgTilemapRect((void *)W(p,0x18),2,0,0,0,0x20,0x18,0x10);
    FillBgTilemapRect((void *)W(p,0x18),1,0,0,0,0x20,0x18,0x10);
    BgCommitTilemapBufferToVram((void *)W(p,0x18),2);
    BgCommitTilemapBufferToVram((void *)W(p,0x18),1);
    ov112_021EC460(p,2,0x16,0x16);
    return 0x18;
}
