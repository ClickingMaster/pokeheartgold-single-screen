#include "global.h"

void ov01_021FB788(u16 *table, int num, int denom);

void ov01_021FB788(u16 *table, int num, int denom) {
    int toggle = 1;
    int i;
    s32 value = num * 3 / denom;
    for (i = 0; i < 0xC0; i++) {
        if (i % 10 == 0) {
            toggle ^= 1;
        }
        if (toggle != 0) {
            table[i] = value;
        } else {
            table[i] = -value;
        }
    }
}
