#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { void *unk0; } Ov13Inner_9288;
typedef struct { Ov13Inner_9288 *inner; u32 unk4; u32 unk8; } Ov13Ctx_9288;
Ov13Ctx_9288 ov13_0224F590;
void WifiSetupTask_Destroy(int a, void *b);
void WifiSetupHeap_FreeAndClear(void *p);
void ov13_02239288(void);
// @fn ov13_02239288
void ov13_02239288(void) {
    WifiSetupTask_Destroy(1, ov13_0224F590.inner->unk0);
    WifiSetupHeap_FreeAndClear(&ov13_0224F590);
}
