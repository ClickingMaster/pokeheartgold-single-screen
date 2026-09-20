#include "to55_w05_private.h"
#define W(p, o)  (*(u32 *)((u8 *)(p) + (o)))
#define SW(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define H(p, o)  (*(s16 *)((u8 *)(p) + (o)))
#define P(p, o)  (*(void **)((u8 *)(p) + (o)))

void ov07_0222FBEC(void *p, int mul) {
    int i = 0;
    u8 *q = (u8 *)p;
    u32 angle;
    q += 0x30;
    angle = i;
    do {
        ov07_022220FC(q, 0, 0x7fff, 0, 0, 0x32000, 0, 0x30);
        W(p, 0x38) += angle;
        i++;
        W(p, 0x48) *= mul;
        q += 0x24;
        p = (u8 *)p + 0x24;
        angle += 0x2aaa;
    } while (i < 6);
}
