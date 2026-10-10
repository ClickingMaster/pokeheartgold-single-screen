#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void ov00_02212124(void *, void *, int);
void ov00_022149A4(char *, signed char *, int);

void ov00_022149A4(char *p, signed char *data, int count) {
    int i, length;
    char *key;
    length = strlen(p + 0x54);
    key = p + 0x54;
    for (i = 0; i < count; i++) {
        int first = i % 8;
        int second = (i * (signed char)key[i % length]) % 8;
        signed char value = (signed char)p[first + 0x74] ^ data[i];
        p[second + 0x74] = (signed char)p[second + 0x74] ^ value;
    }
    ov00_02212124(p + 0x4bc, p + 0x74, 8);
}
