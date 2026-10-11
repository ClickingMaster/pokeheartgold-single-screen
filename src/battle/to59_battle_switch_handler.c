#include "global.h"
#include "battle/battle_system.h"
#include "heap.h"
#include "sys_task_api.h"

void ov12_02261EB8(BattleSystem *);
void ov12_02261ED4(BattleSystem *);
void BattleController_ReplyTargetSelection(BattleSystem *, int, u32);
void BattleController_EmitCommandComplete(BattleSystem *, int, int);
void ov12_0225E760(SysTask *, void *);

void ov12_0225E760(SysTask *task, void *data)
{
    u8 *work = data;
    u8 value;
    if (ov12_0223BE0C(*(BattleSystem **)work, work[0xD], &value) == 1) {
        ov12_02261ED4(*(BattleSystem **)work);
    }
    if (value == 0 || value > 4) {
        ov12_02261EB8(*(BattleSystem **)work);
    } else {
        int battler = value - 1;
        switch (*(u16 *)(work + 0x30)) {
        case 0:
            if (work[0xD] == battler) {
                ov12_02261EB8(*(BattleSystem **)work);
            }
            break;
        case 0x200:
            if (BattleSystem_GetFieldSide(*(BattleSystem **)work, work[0xD]) != BattleSystem_GetFieldSide(*(BattleSystem **)work, battler)) {
                ov12_02261EB8(*(BattleSystem **)work);
            }
            break;
        case 1: case 2: case 4: case 8:
        case 0x10: case 0x20: case 0x40: case 0x80: case 0x100: case 0x400:
            break;
        }
    }
    BattleController_ReplyTargetSelection(*(BattleSystem **)work, work[0xD], value);
    BattleController_EmitCommandComplete(*(BattleSystem **)work, work[0xD], work[0xC]);
    Heap_Free(work);
    SysTask_Destroy(task);
}
