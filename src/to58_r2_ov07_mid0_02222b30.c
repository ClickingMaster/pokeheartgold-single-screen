#include "to58_r2_ov07_mid0_private.h"
extern void ov07_02222AF8(void *,void *);
extern void ov07_02222B14(void *,void *);
void ov07_02222B30(R2TaskWork *w, void *a, void *b, void *c);

void ov07_02222B30(R2TaskWork *w, void *a, void *b, void *c) {
    if (w == NULL) GF_AssertFail();
    w->enabled = 1;
    w->state = 0;
    w->arg18 = a;
    w->arg10 = b;
    w->arg14 = c;
    w->vwaitTask = SysTask_CreateOnVWaitQueue(ov07_02222AF8, w, 0);
    w->vblankTask = SysTask_CreateOnVBlankQueue(ov07_02222B14, w, 0);
}
