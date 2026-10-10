#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
#define HW(a)   (*(volatile u16 *)(a))
void RF_Write(u32);
void WaitLoop_BbpAccess(void);
void BBP_Write(u32, u32);
void WShutdown(void);

void WShutdown(void) {
    if (H(BASE, 0x5f8) == 2) {
        RF_Write(0xc008);
    }
    HW(0x04808158) = 0x601e;
    WaitLoop_BbpAccess();
    BBP_Write(30, HW(0x0480815c) | 0x3f);
    HW(0x04808168) = 0x800d;
    HW(0x04808036) = 1;
}
