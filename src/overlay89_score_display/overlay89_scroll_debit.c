#include "private.h"

BOOL ov89_0225A41C(Ov89WorkR7 *work, s32 amount)
{
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
