#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u8 pad[0x10]; void *obj; } Sub;
typedef struct { u32 a; Sub *sub; } Glob;
extern Glob ov13_0224F5AC;
extern void WifiSetupOamBlock_GetPosition(void *, int, int *, int *);
extern void WifiSetupOamBlock_SetPosition(void *, int, int, int);
extern void WifiSetupTask_SetFunc(void *, void (*)(void *));
void ov13_0223B2C8(void *), ov13_0223B35C(void *);
#pragma push
#pragma opt_propagation off
// @fn ov13_0223B2C8
void ov13_0223B2C8(void *w) {
    int y, x;
    WifiSetupOamBlock_GetPosition(ov13_0224F5AC.sub->obj, 0, &y, &x);
    y -= 8;
    if (y > 0x98) {
        WifiSetupOamBlock_SetPosition(ov13_0224F5AC.sub->obj, -1, y, 0x21);
        return;
    }
    {
        Glob *g = &ov13_0224F5AC;
        Sub *s = g->sub;
        y = 0x98;
        WifiSetupOamBlock_SetPosition(s->obj, -1, y, 0x21);
    }
    WifiSetupTask_SetFunc(w, ov13_0223B35C);
}

#pragma pop
