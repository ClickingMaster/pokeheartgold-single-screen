#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
extern int ov00_021FD4F8(u8);
int ov00_021FD568(u8 *input, u8 *output, u32 count, u32 capacity);
// @fn ov00_021FD568
int ov00_021FD568(u8 *input, u8 *output, u32 count, u32 capacity) {
    u32 tmp;
    u32 tail;
    int k;
    u32 acc;
    int i;
    int remain;
    int whole;
    int size = (count * 3) >> 2;
    int j;
    if (capacity >= (count * 3) >> 2) {
        remain = count & 3;
        whole = count - remain;
    } else {
        return -1;
    }
    for (i = 0; i < whole; i += 4) {
        acc = 0;
        for (j = 0; j < 4; j++) {
            acc |= ov00_021FD4F8(input[i + j]) << ((3 - j) * 6);
        }
        tmp = acc;
        for (k = 0; k < 3; k++) {
            output[(i / 4) * 3 + k] = ((u8 *)&tmp)[2 - k];
        }
    }
    if (remain) {
        tail = 0;
        acc = 0;
        for (j = 0; j < remain; j++) {
            acc |= ov00_021FD4F8(input[whole + j]) << ((3 - j) * 6);
            tail |= acc;
        }
        for (k = 0; k < remain; k++) {
            output[(whole * 3) / 4 + k] = ((u8 *)&tail)[2 - k];
        }
    }
    return size;
}
