#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
#define HW(a)   (*(volatile u16 *)(a))
void WSetTxTimeStampOffset(void);
int WSetRateSet(const u16 *r);

int WSetRateSet(const u16 *r) {
    u8 *w = BASE;
    {
        struct Rate {
            u16 basic, all;
        };
        struct Rate *dest = (struct Rate *)(w + 0x3a4);
        dest->basic = r[0];
        dest->all = r[1] | r[0];
    }
    WSetTxTimeStampOffset();
    return 0;
}
