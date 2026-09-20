#include "to56_battle_r23_private.h"
#pragma force_active on

void ov12_022609F8(SysTask *task, R2Work *work) {
    PaletteData *palette = BattleSystem_GetPaletteData(work->battleSystem);
    switch (work->state) {
    case 0:
        PaletteData_BeginPaletteFade(palette, 0xA, 0xFFFF, -8, 0, 0x10, 0);
        work->state++;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(palette) == 0) {
            ov12_02237B0C(work->battleSystem);
            work->child = Heap_Alloc(HEAP_ID_BATTLE, sizeof(R2Child));
            work->child->party = BattleSystem_GetParty(work->battleSystem, work->battler);
            work->child->battleSystem = work->battleSystem;
            work->child->unk0C = 5;
            work->child->unk11 = work->param;
            work->child->move = work->move;
            work->child->done = 0;
            work->child->unk12 = 0;
            work->child->unk35 = 3;
            work->child->unk22 = 0;
            work->child->unk28 = work->battler;
            work->child->unk32 = 0;
            ov10_0221BE20(work->child);
            work->state++;
        }
        break;
    case 2:
        if (work->child->done != 0) {
            ov12_02237BB8(work->battleSystem);
            PaletteData_BeginPaletteFade(palette, 0xA, 0xFFFF, -8, 0x10, 0, 0);
            work->state++;
        }
        break;
    case 3:
        if (PaletteData_GetSelectedBuffersBitmask(palette) == 0) {
            if (work->child->result == 4) {
                ov12_02263360(work->battleSystem, work->battler, 0xFF);
            } else {
                ov12_02263360(work->battleSystem, work->battler, work->child->result + 1);
            }
            ov12_0226430C(work->battleSystem, work->battler, work->command);
            Heap_Free(work->child);
            Heap_Free(work);
            SysTask_Destroy(task);
        }
        break;
    }
}
