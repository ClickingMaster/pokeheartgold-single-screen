#include "overlay02_port_private.h"

WIP_LOCAL void ov02_02249984(SysTask *task, void *sm) {
    ov02_StateMachineFunc const *table = ov02_02253588;
    while (table[*(int *)sm](sm) == 1) {
    }
    if (*(int *)((u8 *)sm + 0x10) == 1) {
        if (*(void **)((u8 *)sm + 0x1e0) != NULL) {
            FieldEffectPool_DrawAll(*(void **)((u8 *)sm + 0x1e0));
        }
        ov02_0224A32C(sm);
    }
}

WIP_LOCAL void ov02_022499B8(SysTask *task, void *sm) {
    ov02_StateMachineFunc const *table = ov02_022534F0;
    while (table[*(int *)sm](sm) == 1) {
    }
    if (*(int *)((u8 *)sm + 0x10) == 1) {
        if (*(void **)((u8 *)sm + 0x1e0) != NULL) {
            FieldEffectPool_DrawAll(*(void **)((u8 *)sm + 0x1e0));
        }
        ov02_0224A32C(sm);
    }
}

WIP_LOCAL int ov02_022499EC(void *work) {
    VecFx32 vec1;
    VecFx32 vec2;
    if (*(int *)((u8 *)work + 0x214) == 0) {
        return 0;
    }
    vec1 = ov02_022533FC;
    vec2 = ov02_02253414;
    *(Sprite **)((u8 *)work + 0x1e4) = ov02_0224A418(work, &vec1);
    *(Sprite **)((u8 *)work + 0x1e8) = ov02_0224A9B8(work, &vec2);
    ov02_0224A9D8(work, 1);
    ov02_02249D40(work);
    *(int *)((u8 *)work + 0x10) = 1;
    (*(int *)work)++;
    return 1;
}

WIP_LOCAL int ov02_02249A5C(void *work) {
    VecFx32 vec;
    if (*(int *)((u8 *)work + 0x214) == 0) {
        return 0;
    }
    vec = ov02_02253348;
    vec.x = vec.x + *(fx32 *)((u8 *)work + 0x2ec);
    vec.y = vec.y + *(fx32 *)((u8 *)work + 0x2f4);
    *(Sprite **)((u8 *)work + 0x1e4) = ov02_0224A418(work, &vec);
    Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)work + 0x1e4), 1);
    ov02_02249D40(work);
    *(int *)((u8 *)work + 0x10) = 1;
    (*(int *)work)++;
    return 1;
}
