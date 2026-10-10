#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void __VENEER_NVRAM_ReadStatusRegister(u8 *out);
void __VENEER_NVRAM_SoftwareReset(void);
void FLASH_Wait(void);

void FLASH_Wait(void) {
    volatile u32 status;
    for (;;) {
        __VENEER_NVRAM_ReadStatusRegister((u8 *)&status);
        if (status & 0x20) {
            __VENEER_NVRAM_SoftwareReset();
            continue;
        }
        if (!(status & 1)) {
            break;
        }
    }
}
