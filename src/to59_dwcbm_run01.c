// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on

extern u8 _021D43C8[];
extern int readNvram(u32 address, u32 len, void *dst);
extern u8 Wifi[];

int DWCi_BM_GetApInfo(void *dst);

int DWCi_BM_GetApInfo(void *dst) {
    return readNvram(*(u32 *)(_021D43C8 + 8), 0x300, dst) != 0;
}
