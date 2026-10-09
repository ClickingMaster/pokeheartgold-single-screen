#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { void *unk0; void *unk4; } Ov13Ctx_7F68;
Ov13Ctx_7F68 *ov13_0224F4B8;
void ov13_022425F0(int a, void *b);
void ov13_02241D98(void *p);
void ov13_022412B0(void *p);
void ov13_02227F68(void);
// @fn ov13_02227F68
void ov13_02227F68(void) {
    if (ov13_0224F4B8 == NULL) {
        return;
    }
    ov13_022425F0(0, ov13_0224F4B8->unk4);
    ov13_02241D98(ov13_0224F4B8->unk0);
    ov13_022412B0(&ov13_0224F4B8);
}
