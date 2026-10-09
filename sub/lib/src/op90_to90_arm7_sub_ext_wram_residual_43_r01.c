#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define WL (*(u8 **)0x0380fff4)
#define R16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define R32(p,o) (*(u32 *)((u8 *)(p)+(o)))
extern void SetFatalErr(u32);
void WlIntrStartTx(void);
// @fn WlIntrStartTx
void WlIntrStartTx(void)
{
    u8 *p = WL + 0x42c;
    H(0x04808010) = 0x80;
    if (R16(WL, 0x690) & 0x20) {
        u32 st = H(0x04808214) & 0xff;
        u16 t = H(0x04808268);
        if (st >= 3 && st <= 5 && t >= ((R32(p, 0x58) >> 1) & 0xfff) && t <= ((R32(p, 0x30) >> 1) & 0xfff)) {
            H(0x04808244) |= 0x80;
            H(0x04808244) &= ~0x80;
        }
    }
    if (H(0x04808000) != 0x1440 && (H(0x0480819c) & 0x42) == 0x42) {
        u16 v = H(0x048082b8);
        if (v != 0) {
            u32 i = 0;
            while (v == H(0x048082b8)) {
                if (i++ > 1000) {
                    SetFatalErr(0x40);
                    return;
                }
            }
        }
    }
}
