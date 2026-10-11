#include "global.h"

#include "to56_w08_private.h"

void ov07_02224E58(void *ctx) {
    WorkMoveA *w;
    int a;
    w = Heap_Alloc(BattleAnimSystem_GetHeapID(ctx), 0x5c);
    w->pad8 = 0;
    w->state = 0;
    w->owner = ctx;
    w->pic = BattleAnimSystem_GetBattlerPokepic(w->owner, BattleAnimSystem_GetAttacker(ctx));
    w->timer = 0;
    w->delay = ((s32)BattleAnimSystem_GetArg(ctx, 4));
    w->baseY = Pokepic_GetAttr(w->pic, 1);
    w->unk04 = ov07_0221FAA0(w->owner, BattleAnimSystem_GetAttacker(w->owner));
    a = ((s32)BattleAnimSystem_GetArg(ctx, 0));
    ov07_02222508(&w->move, 100, 100, a, ((s32)BattleAnimSystem_GetArg(ctx, 2)));
    ov07_022227A8(&w->bounce, 2, 0, 0, 10);
    BattleAnimSystem_CreateTaskDefaultPriority(w->owner, (void (*)(void *, void *))ov07_02224CB8, w);
}
