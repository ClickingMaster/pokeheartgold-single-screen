#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
#define HW(a)   (*(volatile u16 *)(a))
void FLASH_Read(u32, u32, void *);
int WCalcManRate(void);
void WSetTxTimeStampOffset(void);

void WSetTxTimeStampOffset(void) {
    u32 value = 0xe2e2;
    FLASH_Read(0x58, 2, &value);
    value += 514;
    if (WCalcManRate() == 20) {
        value -= 0x6161;
        if (HW(0x048080bc) & 2) {
            value -= 0x6060;
        }
    }
    HW(0x04808140) = value;
}
