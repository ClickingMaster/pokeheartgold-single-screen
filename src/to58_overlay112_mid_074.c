#include "to58_overlay112_mid_private.h"

int ov112_021EBAB4(Ov112State *p) {
    FillBgTilemapRect((void *)W(p,0x18),4,0,0,0,0x20,0x18,0x10);
    FillBgTilemapRect((void *)W(p,0x18),5,0,0,0,0x20,0x18,0x10);
    FillBgTilemapRect((void *)W(p,0x18),1,0,0,0,0x20,0x18,0x10);
    FillBgTilemapRect((void *)W(p,0x18),2,0,0,0,0x20,0x18,0x10);
    ov112_021EA64C(p);
    PokewalkerApp_HideSprite(p,7);
    BgCommitTilemapBufferToVram((void *)W(p,0x18),4);
    BgCommitTilemapBufferToVram((void *)W(p,0x18),5);
    BgCommitTilemapBufferToVram((void *)W(p,0x18),1);
    BgCommitTilemapBufferToVram((void *)W(p,0x18),2);
    return 2;
}
