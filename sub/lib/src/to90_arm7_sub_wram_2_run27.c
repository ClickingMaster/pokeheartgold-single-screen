#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void NVRAM_WriteDisable(void);
// @fn NVRAM_WriteDisable
void NVRAM_WriteDisable(void) { volatile u16 *control=(volatile u16 *)0x040001C0; volatile u16 *data; while (*control & 0x80) {} data=(volatile u16 *)0x040001C2; *control=0x8100; *data=4; control=data-1; while (*control & 0x80) {} }
