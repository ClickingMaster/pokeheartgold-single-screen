#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define I(p, o) (*(int *)((u8 *)(p) + (o)))
#define U(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*((u8 *)(p) + (o)))
#define V(p, o) (*(void **)((u8 *)(p) + (o)))
extern void ov38_02222138(void *, int, int *, int *);
extern int ov38_02222194(void *, int *, int *);
extern void ov38_0222336C(void *, void *, int);
int ov38_02222350(void *, int, int, char *, int), ov38_02222420(u8 *, u8 *, int, int);

static inline int fold(int ch) {
    int upper = 0;
    if (ch >= 65 && ch <= 90) {
        upper = 1;
    }
    return upper ? ch + 32 : ch;
}

int ov38_02222420(u8 *p, u8 *out, int off, int size) {
    int n, index;
    u8 *q;
    if (off + size <= I(p, 0)) {
        if (size) {
            if (off < 1024) {
                n = 1024 - off;
                if (size <= n) {
                    n = size;
                }
                ov38_0222336C(out, p + 32 + off, n);
                off += n;
                size -= n;
                out += n;
            }
            if (size) {
                off -= 1024;
                index = off >> 9;
                off &= 511;
                int blocks = index;
                index--;
                q = V(p, 28);
                if (blocks) {
                    do {
                        q = V(q, 0);
                    } while (index-- != 0);
                }
                while (size) {
                    n = 512 - off;
                    if (size <= n) {
                        n = size;
                    }
                    ov38_0222336C(out, q + 4 + off, n);
                    off = (off + n) & 511;
                    size -= n;
                    out += n;
                    q = V(q, 0);
                }
            }
        }
        return 1;
    }
    return 0;
}
