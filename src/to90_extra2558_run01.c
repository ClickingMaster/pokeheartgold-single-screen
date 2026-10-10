#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern int ov00_021F35E4(void **);
extern int ov00_021F3398(int, int);
int ov00_021EE820(int);

int ov00_021EE820(int value) {
    void *p;
    int count = ov00_021F35E4(&p);
    int i = 0;
    if (count > 0) {
        u8 *cursor = p;
        do {
            if (value == *cursor) {
                break;
            }
            i++;
            cursor++;
        } while (i < count);
    }
    if (i == count) {
        return 0;
    }
    return ov00_021F3398(i, count);
}
