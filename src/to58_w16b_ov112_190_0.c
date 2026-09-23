#include "global.h"
extern void ov112_021F2204(void *, int, int);
extern void ov112_021F22D0(void *, int, int, int);
extern void ov112_021F1624(void *, int, int);
void ov112_021F27E0(void *);
void ov112_021F27E0(void *s) {
    ov112_021F2204(s, 0, 1);
    ov112_021F2204(s, 1, 0);
    ov112_021F22D0(s, 0, 9, 0x1a);
    ov112_021F1624(s, 0x2d, 0);
}
