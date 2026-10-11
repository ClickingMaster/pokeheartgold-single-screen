#include "to56_r2_w13_private.h"
#pragma force_active on

void ov07_0222D050(void *arg0, R2W13Transition *w) {
    switch (w->state) {
    case 0:
        ov07_0222CFCC(w);
        if (ManagedSprite_GetDrawFlag(w->sprite) == TRUE) {
            int value = BattleAnimSystem_GetDefender(w->owner);
            if (BattleAnimSystem_IsBattlerSemiInvulnerable(w->owner, value) == 0) {
                Pokepic_SetAttr(w->pokepic, 6, 1);
            }
        }
        w->state++;
        break;
    case 1:
        if (ov07_0222CFCC(w)) {
            w->state++;
        }
        break;
    case 2: {
        int bg = BattleAnimSystem_GetBgLayer(w->owner, 1);
        int priority = BattleAnimSystem_GetBgPriority(w->owner, 1);
        SetBgPriority((u8)bg, (u8)priority);
        SetBgPriority(0, (u8)ov07_0221FAE8(w->owner));
        int value = BattleAnimSystem_GetDefender(w->owner);
        if (BattleAnimSystem_IsBattlerSemiInvulnerable(w->owner, value) == 0) {
            Pokepic_SetAttr(w->pokepic, 6, 0);
        }
        bg = BattleAnimSystem_GetBgLayer(w->owner, 1);
        BG_ClearCharDataRange((u8)bg, 1 << 14, 0, BattleAnimSystem_GetHeapID(w->owner));
        BgClearTilemapBufferAndCommit(BattleAnimSystem_GetBgConfig(w->owner), (u8)BattleAnimSystem_GetBgLayer(w->owner, 1));
        BattleAnimSystem_DestroyTask(w->owner, arg0);
        Heap_Free(w);
        break;
    }
    }
}
