#include "unk_02011130_residual_7_internal.h"

void sub_02011DEC(FadeWork11DEC *work, const FadeConfig11DEC *config, s32 steps, s32 interval,
                  u32 arg4, u32 arg5, u32 arg6, u32 arg7)
{
    s16 *buffer;
    s32 step = (config->end - config->start) / steps;

    sub_02010E64(work, config->direction, arg4, arg7);
    work->phase = 2 << 18;
    work->current = config->start;
    work->step = step;
    work->steps = steps;
    work->interval = interval;
    work->counter = 0;
    work->arg5 = arg5;
    work->arg6 = arg6;
    work->arg7 = arg7;
    work->value = config->value;
    sub_02011F10(work);
    SysTask_CreateOnVWaitQueue(sub_02010F00, work, 0x3FF);
    buffer = sub_02010EE0(work, 0);
    sub_02010F84(arg5, config->mode1, config->mode2, config->direction, arg4,
                  buffer[0x1E0], 0, buffer[0x2A0], 0xC0, work->value);
    if (config->direction == 0) {
        sub_02011068(arg5, 1, arg4, work->value);
    } else {
        sub_02011068(arg5, 2, arg4, work->value);
    }
    sub_0200FF88(work->arg6, (u32)work, (u32)sub_02010C38, arg4, arg7);
}
