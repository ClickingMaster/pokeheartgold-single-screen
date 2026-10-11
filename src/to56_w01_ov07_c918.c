#include "to56_w01_private.h"

void ov07_0222C918(int task, To56W01Work *work) {
    int bg;
    switch (work->state) {
    case 0:
        if (!ov07_0222C870(work)) {
            work->state++;
            BattleAnimUtil_SetBlendBgOverBattleBg(work->owner, 31, 0);
            ov07_02222AC4(&work->motion, 31, 0, 0, 31, 16);
            return;
        }
        break;
    case 1:
        ov07_0222C870(work);
        if (ov07_02222AF4(&work->motion)) {
            work->state++;
            ov07_02222D74(work->handle);
            work->counter = 0;
            return;
        }
        break;
    case 2:
        BgSetPosTextAndCommit(ov07_0221C4A0(work->owner), 2, BG_POS_OP_SET_X, -work->x);
        BgSetPosTextAndCommit(ov07_0221C4A0(work->owner), 2, BG_POS_OP_SET_Y, -work->y);
        work->counter++;
        if (work->counter > 8) {
            ov07_02222AC4(&work->motion, 0, 31, 31, 0, 16);
            work->state++;
            return;
        }
        break;
    case 3:
        if (ov07_02222AF4(&work->motion)) {
            Pokepic_SetAttr(work->pic, 6, 0);
            work->state++;
            return;
        }
        break;
    case 4:
        ov07_02222D3C(work->handle);
        bg = ov07_0221FAEC(work->owner, 1);
        SetBgPriority((u8)bg, (u8)ov07_0221FB04(work->owner, 1));
        SetBgPriority(0, (u8)ov07_0221FAE8(work->owner));
        bg = ov07_0221FAEC(work->owner, 1);
        BG_ClearCharDataRange((u8)bg, 0x4000, 0, (enum HeapID)ov07_0221BFD0(work->owner));
        BgClearTilemapBufferAndCommit(ov07_0221C4A0(work->owner), ov07_0221FAEC(work->owner, 1));
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
        break;
    }
}
