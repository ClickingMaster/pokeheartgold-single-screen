#include "to58_overlay112_mid_private.h"

int ov112_021EC390(Ov112State *p) {
    if (TextPrinterCheckActive((u8)p->textPrinter) == 0) return 29;
    return 28;
}
