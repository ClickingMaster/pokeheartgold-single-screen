#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef union {
    u32 raw;
    struct {
        u32 kind : 4;
        u32 opts : 28;
    } bits;
} ModeFlags;
typedef struct {
    u8 mode;
    u8 pad1[7];
    ModeFlags flags;
} ModeState;
extern ModeState ov13_0224F4CC;
BOOL ov13_0222B4BC(int mode, u32 flags);
// @fn ov13_0222B4BC
BOOL ov13_0222B4BC(int mode, u32 flags) {
    ov13_0224F4CC.mode = mode;
    ov13_0224F4CC.flags.raw = flags;
    if (mode < 0 || mode > 6) {
        return FALSE;
    }
    if (mode == 1 && ((flags >> 4) & ~2) != 0) {
        return FALSE;
    }
    if (ov13_0224F4CC.flags.bits.kind > 1) {
        return FALSE;
    }
    if (mode != 0 && (ov13_0224F4CC.flags.bits.opts & 1)) {
        return FALSE;
    }
    if (mode == 0 && !(ov13_0224F4CC.flags.bits.opts & 1)) {
        return FALSE;
    }
    return TRUE;
}
