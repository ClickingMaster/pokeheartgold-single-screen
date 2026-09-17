#include "to50_overlay99_r9_gap_private.h"
void ov99_021E8224(void *work) {
    MessageFormat *format = (MessageFormat *)ov98_0221EEFC(PTR(work, 0x400));
    int i;
    ov98_0221EC24(PTR(work, 0x400), 0xb, 0x7e, 3);
    ov98_0221EC08(PTR(work, 0x400), 0, 0x7f, 1, 0);
    ov98_0221EBEC(PTR(work, 0x400), 4, 0, 1, 0, 4);
    for (i = 0; i < 5; i++) {
        BufferPokeathlonMedalName(format, 0, (u8)i);
        ov98_0221EBD8(PTR(work, 0x400), i + 6, 4, 0);
    }
    BufferIntegerAsString(format, 0, ov99_021E718C(work), 3, PRINTING_MODE_RIGHT_ALIGN, 1);
    ov98_0221EC08(PTR(work, 0x400), 1, 0x80, 1, 0);
    if (ov99_021E7150(work) != NULL) {
        int value = ov99_021E724C(work);
        ov98_0221EC08(PTR(work, 0x400), 2, value + 0x71, 1, 0);
    }
}
