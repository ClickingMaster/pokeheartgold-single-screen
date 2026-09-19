#include "to53_overlay112_w09_private.h"


BOOL ov112_021E9338(void *p) {
    u16 a;
    u16 b;
    extern void sub_02032688(void *, u16 *, u16 *);
    sub_02032688(*(void **)((u8 *)p + 0x1E440), &a, &b);
    return a != 0;
}
