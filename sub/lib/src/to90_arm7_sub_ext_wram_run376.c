#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define ROOT    (*(u8 *volatile *)0x0380FFF4)
#define H(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define V(p, n) (*(volatile u16 *)((u8 *)(p) + (n)))
#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))
extern u16 *AdjustRingPointer(u16 *);
void WlIntrStartRx(void);

void WlIntrStartRx(void) {
    u16 *cursor = 0;
    u32 startTime = 0, start = 0, index = 0;
    u16 distance = 0;
    u8 *root = ROOT;
    u8 *state = root + 0x344;
    u8 *q = root + 0x42c;
    V((void *)0x04808010, 0) = 0x40;
    if (!(H(ROOT, 0x690) & 0x20)) {
        goto done;
    }
    if (H(q, 0xa4) != 0xffff || (V((void *)0x0480819C, 0) & 3) != 3) {
        goto done;
    }
    if ((int)V((void *)0x04808268, 0) < (int)(((u32)V((void *)0x04808050, 0) >> 1) & 0xfff)) {
        goto done;
    }
    start = V((void *)0x04808054, 0);
    cursor = ((u16 *)0x04804000) + start;
    cursor += 4;
    cursor = AdjustRingPointer(cursor);
    cursor = AdjustRingPointer(cursor + 2);
    if ((*cursor & 0xe7ff) != 0x228) {
        goto done;
    }
    cursor = AdjustRingPointer(cursor + 1);
    startTime = (u32)V((void *)0x048080F8, 0) - 0x10000u;
    for (;;) {
        distance = (u16)(V((void *)0x04808268, 0) - start);
        if (distance & 0x8000) {
            distance = (u16)(distance + ((u32)H(state, 0x9a) >> 1));
        }
        if (distance > 14) {
            break;
        }
        if ((u16)(V((void *)0x048080F8, 0) - startTime) > 64) {
            goto done;
        }
    }
    cursor += 4;
    for (index = 0; index < 3; index++) {
        cursor = AdjustRingPointer(cursor);
        if (*cursor++ != H(state, 0x64 + 2 * index)) {
            goto done;
        }
    }
    cursor = AdjustRingPointer(cursor + 5);
    for (;;) {
        distance = (u16)(V((void *)0x04808268, 0) - start);
        if (distance & 0x8000) {
            distance = (u16)(distance + ((u32)H(state, 0x9a) >> 1));
        }
        if (distance > 20) {
            break;
        }
        if ((u16)(V((void *)0x048080F8, 0) - startTime) > 112) {
            goto done;
        }
    } /* Hardware key station ID is configured in the 0..15 range. */
    if (*cursor & (1u << V((void *)0x04808028, 0))) {
        goto done;
    }
    H(q, 0xa4) = V((void *)0x04808098, 0);
    V((void *)0x048080B4, 0) = 0x40;
    H(state, 0xbe)
    ++;
    while ((V((void *)0x0480819C, 0) & 3) == 3) {}
    V((void *)0x04808244, 0) |= 0x40;
    V((void *)0x04808244, 0) &= ~0x40;
    V((void *)0x04808228, 0) = 8;
    V((void *)0x04808228, 0) = 0;
    {
        u8 *newq = ROOT + 0x42c;
        u16 saved = H(newq, 0xa4);
        if (saved != 0xffff) {
            u16 old = V((void *)0x04808094, 0);
            V((void *)0x04808094, 0) = saved;
            V((void *)0x04808030, 0) |= 0x80;
            V((void *)0x04808094, 0) = old;
            H(newq, 0xa4) = 0xffff;
        }
    }
done:;
}
