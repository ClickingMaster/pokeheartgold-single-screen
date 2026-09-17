#include "overlay80_frontier_task_command_r26_private.h"

int FrtCmd_076(void *ctx) {
    u16 first;
    u16 second;
    u16 third;
    u16 fourth;
    u8 *taskData = (u8 *)sub_0209680C(PTR_AT(PTR_AT(ctx, 0), 0)) + 0xa4;

    if (PTR_AT(taskData, 0) != NULL) {
        GF_AssertFail();
        SysTask_Destroy(PTR_AT(taskData, 0));
    }
    first = FrontierScript_ReadVar(ctx);
    second = FrontierScript_ReadVar(ctx);
    third = FrontierScript_ReadVar(ctx);
    fourth = FrontierScript_ReadVar(ctx);
    MI_CpuFill8(taskData, 0, 0xc);
    U16_AT(taskData, 4) = first;
    U16_AT(taskData, 6) = second;
    U8_AT(taskData, 9) = third;
    U8_AT(taskData, 0xa) = fourth;
    PTR_AT(taskData, 0) = SysTask_CreateOnMainQueue(ov80_0222F4F0, taskData, 0x11170);
    return 0;
}
