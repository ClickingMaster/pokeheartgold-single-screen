#include "private.h"

BOOL ov89_0225A41C(void *opaque, s32 amount)
{
    Ov89WorkR7 *work = opaque;
    s32 delta;

    if (work->target < amount) {
        return FALSE;
    }
    if (work->current < work->target) {
        GF_ASSERT(FALSE);
        work->current = work->target;
    }
    work->target -= amount;
    delta = work->current - work->target;
    work->step = delta / 4;
    return TRUE;
}
