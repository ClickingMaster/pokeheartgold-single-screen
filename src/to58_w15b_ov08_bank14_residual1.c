#include "to57_overlay08_bank14_private.h"
#pragma force_active on

void ov08_0221DD40(void *);

void ov08_0221DD40(void *work) {
    Ov08R2W11Work *w = work;
    u32 i;
    WindowArray_Delete(w->windowArray, w->windowCount);
    for (i = 0; i < 2; i++) RemoveWindow(&w->windows[i]);
}
