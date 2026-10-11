#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { void *unk0; u32 unk4; u32 unk8; void *unkC; } Ov13Ctx_9514;
Ov13Ctx_9514 *ov13_0224F59C;
void WifiSetupTask_Destroy(int a, void *b);
void WifiSetupOamBlock_Free(void *p);
void WifiSetupHeap_FreeAndClear(void *p);
void ov13_02239514(void);
// @fn ov13_02239514
void ov13_02239514(void) {
    WifiSetupTask_Destroy(0, ov13_0224F59C->unkC);
    WifiSetupOamBlock_Free(ov13_0224F59C->unk0);
    WifiSetupHeap_FreeAndClear(&ov13_0224F59C);
}
