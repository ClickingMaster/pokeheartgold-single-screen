#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern const s8 _021105F4[];
void DWCi_Acc_U64ToString32(u64, int, char *);

void DWCi_Acc_U64ToString32(u64 key, int bits, char *dst) {
    int count = (bits + 4) / 5;
    char *last;
    s8 digit;
    int i;
    const s8 *table = _021105F4;
    i = 0;
    if (count > 0) {
        last = dst + count;
        last--;
        do {
            digit = table[key & 31];
            *(last - i) = digit;
            key >>= 5;
            i++;
        } while (i < count);
    }
    dst[count] = 0;
}
