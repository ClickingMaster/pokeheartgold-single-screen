#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { void *unk0; u32 unk4; u32 unk8; void *unkC; } Ov13Ctx_9514;
Ov13Ctx_9514 *ov13_0224F59C;
void ov13_022425F0(int a, void *b);
void ov13_0223FD60(void *p);
void ov13_022412B0(void *p);
void ov13_02239514(void);
// @fn ov13_02239514
void ov13_02239514(void) {
    ov13_022425F0(0, ov13_0224F59C->unkC);
    ov13_0223FD60(ov13_0224F59C->unk0);
    ov13_022412B0(&ov13_0224F59C);
}
