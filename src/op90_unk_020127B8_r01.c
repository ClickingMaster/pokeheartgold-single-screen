#pragma force_active on
#include "global.h"
typedef struct { u8 pad[0xC]; u32 phase; } Work;
extern s16 *sub_02010EE0(Work *, int);
extern int sub_020109D8(int, int);
void sub_020127B8(Work *w);
// @fn sub_020127B8
void sub_020127B8(Work *w) {
int i; s16 *a, *b, *pa, *pb; u32 inv; u32 phase = (u16)w->phase; a = sub_02010EE0(w, 0); b = sub_02010EE0(w, 1); inv = 0x3FFF - phase; pa = a; pb = b;
for (i = 0; i < 96; i++) { u32 n = 96 - i; int x = sub_020109D8(phase, n); int y = sub_020109D8(inv, n); s16 ly, lx, rx, ry; int m; if (x > 127) x = 127; if (y > 127) y = 127; ly = 128 - y; pa[0x180] = ly; lx = 128 - x; pa[0x240] = lx; m = 191 - i; a[m + 0x180] = ly; a[m + 0x240] = lx; rx = x + 128; pb[0x180] = rx; ry = y + 128; pb[0x240] = ry; b[m + 0x180] = rx; b[m + 0x240] = ry; pa++; pb++; }
}
