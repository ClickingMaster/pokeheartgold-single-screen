#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a)      (*(volatile u16 *)(a))
#define WL        (*(u8 **)0x0380fff4)
#define R16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define R32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o)   (*(u8 **)((u8 *)(p) + (o)))
extern u32 __VENEER_OS_DisableIrqMask(u32);
extern u32 __VENEER_OS_EnableIrqMask(u32);
extern void WlIntrMpEnd(int);
void MacBugTxMp(void);

void MacBugTxMp(void) {
    u32 mask = __VENEER_OS_DisableIrqMask(0x1000000);
    u16 original = H(0x04808210);
    int limit = 100;
    H(0x04808244) = 0x1000;
    while (limit) {
        if (original != H(0x04808210)) {
            break;
        }
        limit--;
    }
    H(0x04808244) = 0;
    WlIntrMpEnd(0);
    __VENEER_OS_EnableIrqMask(mask);
}
