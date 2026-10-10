#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma dollar_identifiers on
extern volatile u32 skipCheck$3668[];
extern volatile u32 nextCount$3668[];
int CARD_IsCardIreqLo(void);

int CARD_IsCardIreqLo(void) {
    int low = 1;
    if (*(volatile u32 *)0x04000214 & 0x100000) {
        low = 0;
        skipCheck$3668[3] = 1;
    }
    return low;
}
