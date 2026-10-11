#include "to56_w01_private.h"

void ov07_0222C918(int task, To56W01Work *work) {
    int bg;
    switch (work->state) {
    case 0:
        if (!ov07_0222C870(work)) {
            work->state++;
            BattleAnimUtil_SetBlendBgOverBattleBg(work->owner, 31, 0);
            BattleAnimUtil_StartBlendAlphaMotion(&work->motion, 31, 0, 0, 31, 16);
            return;
        }
        break;
    case 1:
        ov07_0222C870(work);
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->motion)) {
            work->state++;
            BattleAnimScanline_DisableWave(work->handle);
            work->counter = 0;
            return;
        }
        break;
    case 2:
        BgSetPosTextAndCommit(BattleAnimSystem_GetBgConfig(work->owner), 2, BG_POS_OP_SET_X, -work->x);
        BgSetPosTextAndCommit(BattleAnimSystem_GetBgConfig(work->owner), 2, BG_POS_OP_SET_Y, -work->y);
        work->counter++;
        if (work->counter > 8) {
            BattleAnimUtil_StartBlendAlphaMotion(&work->motion, 0, 31, 31, 0, 16);
            work->state++;
            return;
        }
        break;
    case 3:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->motion)) {
            Pokepic_SetAttr(work->pic, 6, 0);
            work->state++;
            return;
        }
        break;
    case 4:
        BattleAnimScanline_FreeWave(work->handle);
        bg = BattleAnimSystem_GetBgLayer(work->owner, 1);
        SetBgPriority((u8)bg, (u8)BattleAnimSystem_GetBgPriority(work->owner, 1));
        SetBgPriority(0, (u8)ov07_0221FAE8(work->owner));
        bg = BattleAnimSystem_GetBgLayer(work->owner, 1);
        BG_ClearCharDataRange((u8)bg, 0x4000, 0, (enum HeapID)BattleAnimSystem_GetHeapID(work->owner));
        BgClearTilemapBufferAndCommit(BattleAnimSystem_GetBgConfig(work->owner), BattleAnimSystem_GetBgLayer(work->owner, 1));
        BattleAnimSystem_DestroyTask(work->owner, task);
        Heap_Free(work);
        break;
    }
}
