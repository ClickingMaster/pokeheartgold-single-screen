#include "global.h"

#include "to56_w08_private.h"

void ov07_022248F4(void *task, WorkBlink *w) {
    if (w->timer >= w->limit) {
        if (w->count == 0) {
            Pokepic_SetAttr(w->pic, 6, 0);
            Heap_Free(w);
            BattleAnimSystem_DestroyTask(w->owner, task);
        } else {
            Pokepic_SetAttr(w->pic, 6, 1 ^ Pokepic_GetAttr(w->pic, 6));
            w->count--;
            w->timer = 0;
        }
    } else {
        w->timer++;
    }
}

void ov07_02224948(void *ctx) {
    WorkBlink *w = BattleAnimUtil_AllocWork(ctx, sizeof(WorkBlink));
    w->owner = ctx;
    w->count = ((s32)BattleAnimSystem_GetArg(ctx, 0)) * 2;
    w->limit = ((s32)BattleAnimSystem_GetArg(ctx, 1));
    BattleAnimUtil_InitCommonWork(ctx, &w->motion);
    w->pic = BattleAnimSystem_GetBattlerPokepic(ctx, BattleAnimSystem_GetAttacker(ctx));
    BattleAnimSystem_CreateTaskDefaultPriority(w->owner, (void (*)(void *, void *))ov07_022248F4, w);
}
