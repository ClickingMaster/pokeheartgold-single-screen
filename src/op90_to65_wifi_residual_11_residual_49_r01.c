#pragma force_active on
#include <nitro/types.h>
u32 WcmCountLeadingZero(u32 x);

#include <nitro/code32.h>

// @fn WcmCountLeadingZero
asm u32 WcmCountLeadingZero(u32 x) {
    clz r0, r0
    bx lr
}
