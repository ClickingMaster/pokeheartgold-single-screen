#include "overlay02_port_private.h"

WIP_LOCAL void ov02_02249584(SysTask *task, void *sm) {
    ov02_StateMachineFunc const *table = ov02_02253550;
    while (table[*(int *)sm](sm) == 1) {
    }
    if (*(int *)((u8 *)sm + 0x10) == 1) {
        if (*(void **)((u8 *)sm + 0x1e0) != NULL) {
            sub_02068BAC(*(void **)((u8 *)sm + 0x1e0));
        }
        ov02_0224A32C(sm);
    }
}
