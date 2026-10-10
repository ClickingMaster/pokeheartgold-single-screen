#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define ROOT (*(u8 **)0x0380fff4)

typedef struct {
    u16 mul, add, seed;
} RndState;

void RND_Init(u32 mul, u32 add);

void RND_Init(u32 mul, u32 add) {
    u8 *p = ROOT;
    RndState *r = (RndState *)(p + 0x5f0);
    r->mul = (mul & 0xfff8) + 5;
    r->add = add | 1;
}
