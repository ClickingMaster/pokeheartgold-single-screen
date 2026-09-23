#include "to50_overlay99_record_counts_private.h"

u8 ov99_021E7100(int value)
{
    const int *thresholds = ov99_021E9E04;
    u8 i = 0;
    do {
        if (thresholds[i] <= value) {
            break;
        }
        i++;
    } while (i < 10);
    return 10 - i;
}

u8 ov99_021E7124(void **work)
{
    int i;
    u8 total = 0;

    for (i = 0; i < 5; i++) {
        total += ov98_0221EF64(ov99_021E70E8(*work, (u8)i));
    }
    return total;
}
