#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 ov00_02210F88(u8);
extern void ov00_02210F74(u8 *, u8 *);
void ov00_02210FD0(const u8 *, int, u8 *), ov00_02211094(const u8 *, int, u8 *, int);

void ov00_02210FD0(const u8 *src, int len, u8 *dst) {
    u8 input[3], output[4];
    int used = 0, i;
    while (used < len) {
        for (i = 0; i <= 2; i++) {
            if (used < len) {
                input[i] = *src++;
            } else {
                input[i] = 0;
            }
            used++;
        }
        output[0] = input[0] >> 2;
        output[1] = ((input[0] & 3) << 4) + (input[1] >> 4);
        output[2] = ((input[1] & 15) << 2) + (input[2] >> 6);
        output[3] = input[2] & 63;
        for (i = 0; i <= 3; i++) {
            *dst++ = ov00_02210F88(output[i]);
        }
    }
    *dst = 0;
}

void ov00_02211094(const u8 *key, int keylen, u8 *data, int len) {
    u8 table[256];
    u8 i, j;
    s16 n;
    for (n = 0; n < 256; n++) {
        table[n] = n;
    }
    i = 0;
    j = 0;
    for (n = 0; n < 256; n++) {
        j = (table[n] + key[i] + j) % 256;
        i = (i + 1) % keylen;
        ov00_02210F74(&table[n], &table[j]);
    }
    i = 0;
    j = 0;
    for (n = 0; n < len; n++) {
        i = (i + data[n] + 1) % 256;
        j = (table[i] + j) % 256;
        ov00_02210F74(&table[i], &table[j]);
        data[n] ^= table[(u8)((table[i] + table[j]) % 256)];
    }
}
