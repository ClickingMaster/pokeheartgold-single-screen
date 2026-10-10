#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void EXIi_SelectRcnt(u32);
void RtcGpioTransfer(u32, u32, void *, u32);
void RTC_ReadDateTime(void *);

void RTC_ReadDateTime(void *p) {
    EXIi_SelectRcnt(0x8000);
    RtcGpioTransfer(0x86, 32, p, 7);
}
