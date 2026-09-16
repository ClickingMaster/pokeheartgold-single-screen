#include "slop_r1_apricorn_private.h"

int sub_02031C08(SaveApricornBox *box) {
    int i;
    u8 count;
    u16 value;

    count = 0;
    value = box->unk_0A;
    for (i = 0; i < 5; i++) {
        if (value & 7) {
            count++;
        }
        value >>= 3;
    }
    return count;
}
