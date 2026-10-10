#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

void *WL_GetThreadStruct(void);

void *WL_GetThreadStruct(void) {
    return *(u8 *volatile *)0x380fff4 + 24;
}
