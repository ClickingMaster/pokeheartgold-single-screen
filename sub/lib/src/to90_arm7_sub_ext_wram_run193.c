#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a)      (*(volatile u16 *)(a))
#define WL        (*(u8 **)0x0380fff4)
#define R16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define R32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o)   (*(u8 **)((u8 *)(p) + (o)))
extern void WCheckTxBuf(void);
void WlIntrRxCntup(void);

void WlIntrRxCntup(void) {
    u8 *base = WL;
    u8 *stats = base + 0x4dc;
    u8 *slots = base + 0x42c;
    u16 status;
    H(0x04808010) = 4;
    status = H(0x048081a8);
    if ((R16(WL, 0x690) & 8) && (status & 0x400)) {
        u16 busy = H(0x048080b0);
        if (!((busy & 1) && R16(slots, 0)) && !((busy & 4) && R16(slots, 0x14)) && !((busy & 8) && R16(slots, 0x28)) && !(H(0x0480819c) & 1)) {
            H(0x04808032) = 0;
            H(0x04808032) = 0x8000;
            R16(stats, 2) = 0;
        } else {
            u32 previous = R16(stats, 2);
            R16(stats, 2) = (u16)(previous + 1);
            if (previous > 12) {
                R16(stats, 2) = 0;
                H(0x04808032) = 0;
                H(0x04808032) = 0x8000;
                R16(WL, 0x3fe)
                ++;
            }
        }
    }
    if ((R16(WL, 0x690) & 1) && (status & 0x60)) {
        s32 position = H(0x04808054);
        if (position >= (s32)(H(0x04808052) - 0x4000) / 2 || position < (s32)(H(0x04808050) - 0x4000) / 2) {
            H(0x04808056) = H(0x0480805a);
            H(0x04808030) = 0x8001;
        }
        WCheckTxBuf();
    }
}
