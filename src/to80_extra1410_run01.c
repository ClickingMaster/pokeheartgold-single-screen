#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))
extern char ov00_02219CB4[];
extern int ov00_0220CC38(void *, void *, int);
extern int ov00_0220BF70(void *);
extern void ov00_0220C03C(void *, void *, int);
extern void ov00_0220C0C4(void *, int, int);
int ov00_0220EBCC(u8 *, u8 *, int);

int ov00_0220EBCC(u8 *c, u8 *p, int len) {
    int pos;
    u8 *buffer;
    int n;
    if (len < 2 || memcmp(p, ov00_02219CB4, 2)) {
        int result = ov00_0220CC38(c, p, len);
        if (result == 0) {
            return 0;
        }
        return 1;
    }
    n = len + 2;
    if (ov00_0220BF70(c + 80) < n) {
        return 1;
    }
    buffer = (u8 *)W(c, 80);
    pos = W(c, 88);
    ov00_0220C03C(c + 80, ov00_02219CB4, 2);
    ov00_0220C03C(c + 80, p, len);
    if (!ov00_0220CC38(c, buffer + pos, n)) {
        return 0;
    }
    ov00_0220C0C4(c + 80, -1, n);
    return 1;
}
