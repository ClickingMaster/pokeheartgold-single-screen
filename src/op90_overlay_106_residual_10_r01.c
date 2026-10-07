#pragma force_active on
#include "global.h"
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
int ov106_021E6B30(void *state, void *cmd);
// @fn ov106_021E6B30
int ov106_021E6B30(void *state, void *cmd) {
    if (U32(cmd, 0) != 5) {
        GF_AssertFail();
    }
    U32(state, 0x40c) = 2;
    U32(state, 0x410) = 4;
    return 2;
}
