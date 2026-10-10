#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma dollar_identifiers on
extern volatile u32 skipCheck$3668[];
extern volatile u32 nextCount$3668[];
extern int CARD_CompareCardID(void);
extern int CARD_IsCardIreqLo(void);
int CARD_IsPulledOut(void);

int CARD_IsPulledOut(void) {
    if (!skipCheck$3668[3]) {
        if (*(volatile u8 *)0x027FFE1F & 128) {
            CARD_CompareCardID();
        } else {
            CARD_IsCardIreqLo();
        }
    }
    return skipCheck$3668[3];
}
