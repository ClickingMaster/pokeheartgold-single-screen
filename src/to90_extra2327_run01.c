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

static inline BOOL sameFold(int c, int ch) {
    int upper = 0;
    int rhs;
    if (ch >= 65 && ch <= 90) {
        upper = 1;
    }
    rhs = upper ? ch + 32 : ch;
    if (c >= 65 && c <= 90) {
        c += 32;
    }
    return c == rhs;
}

#pragma force_active on

int ov38_02222350(void *p, int start, int end, char *s, int stop) {
    int a, b, c;
    s8 ch;
    if (start < end) {
        ov38_02222138(p, start, &a, &b);
        c = ov38_02222194(p, &a, &b);
        end--;
        while (sameFold(c, ch = *s)) {
            if (ch == 0 || ch == ' ' || ch == stop || start == end) {
                return 0;
            }
            c = ov38_02222194(p, &a, &b);
            start++;
            s++;
        }
    }
    return -1;
}
