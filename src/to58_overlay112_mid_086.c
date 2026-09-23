#include "to58_overlay112_mid_private.h"

int ov112_021EC2E4(Ov112State *p) {
    if (TextPrinterCheckActive((u8)p->textPrinter) == 0) return 24;
    return 23;
}

int ov112_021EC304(Ov112State *p) {
    W(p,0x1f2d0) = 0;
    ov112_021EA688(p,8);
    ov112_021E9290((u8 *)p+0x1f2e8,(u8 *)p+0x1d7ac,(u8 *)p+0x1d79c,0);
    return 0x19;
}
