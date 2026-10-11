#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u8 unk0[0x9c]; void *unk9c; } Ov13Ctx_AB8C;
Ov13Ctx_AB8C *ov13_0224F5A8;
void WifiSetupTask_DestroyLater(int a, void *b);
void WifiSetupHeap_FreeAndClear(void *p);
void ov13_0223AB8C(void);
// @fn ov13_0223AB8C
void ov13_0223AB8C(void) {
    WifiSetupTask_DestroyLater(0, ov13_0224F5A8->unk9c);
    WifiSetupHeap_FreeAndClear(&ov13_0224F5A8);
}
