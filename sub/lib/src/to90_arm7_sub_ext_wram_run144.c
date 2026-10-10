#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
#define HW(a)   (*(volatile u16 *)(a))
u32 __VENEER__u32_div_f(u32, u32);
int WSetFrameLifeTime(u32 life);

int WSetFrameLifeTime(u32 life) {
    u8 *w = BASE;
    u8 *t = w + 0x344;
    u8 *s = w + 0x31c;
    u32 value;
    if (life == 65535) {
        H(s, 0x1c) = 65535;
        H(t, 0x8c) = 65535;
    } else {
        value = __VENEER__u32_div_f(life * H(t, 0x6e), 100);
        if (value > 65536) {
            return 5;
        }
        H(s, 0x1c) = life;
        H(t, 0x8c) = value;
    }
    return 0;
}
