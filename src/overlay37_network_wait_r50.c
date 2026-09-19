#include "global.h"

#include "overlay_37_helpers_internal.h"

#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))

typedef struct Ov37WorkR5 {
    u8 pad0000[0x4380];
    u8 flags;
    u8 pad4381[0x93B8 - 0x4381];
    u16 player;
    s16 counter;
} Ov37WorkR5;

extern void sub_02037030(u32, const void *, u32);
extern void MI_CpuFill8(void *, u8, u32);

u32 ov37_021E6B64(Ov37Work *work, u32 state);

u32 ov37_021E6B64(Ov37Work *work, u32 state) {
    u8 packet[4];
    Ov37WorkR5 *w = (Ov37WorkR5 *)work;

    w->flags &= ~0x38;
    if (w->player != sub_02037454() || w->player != ov37_021E75C4()) {
        w->counter = 0;
        ov37_021E7844(work, 9);
        ov37_021E68AC(work);
        return state;
    }
    w->counter++;
    if (w->counter > 30) {
        MI_CpuFill8(packet, 0, 4);
        packet[2] = 1;
        packet[0] = (u8)sub_0203769C();
        sub_02037030(0x7E, packet, 4);
        w->counter = 0;
        ov37_021E7844(work, 10);
    }
    ov37_021E68AC(work);
    return state;
}
