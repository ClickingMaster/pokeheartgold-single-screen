#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { void *unk0; } Ov13Inner_9288;
typedef struct { Ov13Inner_9288 *inner; u32 unk4; u32 unk8; } Ov13Ctx_9288;
Ov13Ctx_9288 ov13_0224F590;
void ov13_022425F0(int a, void *b);
void ov13_022412B0(void *p);
void ov13_02239288(void);
// @fn ov13_02239288
void ov13_02239288(void) {
    ov13_022425F0(1, ov13_0224F590.inner->unk0);
    ov13_022412B0(&ov13_0224F590);
}
