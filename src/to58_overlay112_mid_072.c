#include "to58_overlay112_mid_private.h"

int ov112_021EBA6C(Ov112State *p) {
    if (TextPrinterCheckActive((u8)p->textPrinter) == 0) return 2;
    return 1;
}
