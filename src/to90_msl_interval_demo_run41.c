#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
char *_Z22__DecodeUnsignedNumberPcPm(char *, u32 *), *_Z16__SkipUnwindInfoPc(char *);

char *_Z16__SkipUnwindInfoPc(char *p) {
    u32 value;
    u8 flag = *p;
    p = _Z22__DecodeUnsignedNumberPcPm(p + 2, &value);
    if (flag & 0x40) {
        p = _Z22__DecodeUnsignedNumberPcPm(p, &value);
    }
    return p;
}
