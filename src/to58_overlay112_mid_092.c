#include "to58_overlay112_mid_private.h"

int ov112_021EC640(Ov112State *p) {
    BgClearTilemapBufferAndCommit((void *)W(p,0x18),1);
    ov112_021E7CA4(p,2,0xe);
    ov112_021E9F40(p,1);
    p->textPrinter = ov112_021EA08C(p,0,5);
    ov112_021ED2F4(p);
    return 2;
}

int ov112_021EC67C(Ov112State *p) {
    if (TextPrinterCheckActive((u8)p->textPrinter) == 0) return 26;
    return 2;
}
