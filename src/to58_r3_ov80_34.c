#include "global.h"

extern void *ov80_0222E558(void *, u8, u8, u8, u8, u16 *, void *, void *);
void *ov80_0222E920(void *, u8, u8, u8, u8, u16 *, void *, void *);

void *ov80_0222E920(void *a, u8 b, u8 c, u8 d, u8 e, u16 *f, void *g, void *h) {
    return ov80_0222E558(a, b, c, d, e, f, g, h);
}
