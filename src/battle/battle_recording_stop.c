#include "battle/battle_recording_stop.h"

#include "global.h"

#include "battle/battle_input.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "sys_task.h"
#include "sound_playback.h"

static void Task_BattleRecordingStop(SysTask *task, void *data);

SysTask *BattleRecordingStop_CreateTask(BattleSystem *battleSystem) {
    BattleRecordingStopWork *unk;
    unk = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleRecordingStopWork));
    MI_CpuFill8(unk, 0, sizeof(BattleRecordingStopWork));
    unk->battleSystem = battleSystem;
    return SysTask_CreateOnMainQueue(Task_BattleRecordingStop, unk, 1000);
}

void BattleRecordingStop_DestroyTask(SysTask *task) {
    Heap_Free(SysTask_GetData(task));
    SysTask_Destroy(task);
}

static void Task_BattleRecordingStop(SysTask *task, void *_data) {
    BattleRecordingStopWork *data = _data;
    BattleInput *battleInput = BattleSystem_GetBattleInput(data->battleSystem);

    switch (data->state) {
    case 0:
        GF_ASSERT(battleInput != NULL);
        {
            NARC *bgHandleNarc = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
            NARC *objHandleNarc = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
            BattleInput_ChangeMenu(bgHandleNarc, objHandleNarc, battleInput, 18, FALSE, NULL);
            NARC_Delete(bgHandleNarc);
            NARC_Delete(objHandleNarc);
        }
        data->state++;
        break;
    case 1:
        if (BattleInput_CheckFeedbackDone(battleInput) == TRUE) {
            data->state++;
        }
        break;
    case 2:
        if (BattleSystem_CanPauseRecording(data->battleSystem) == TRUE && BattleInput_CheckTouch(battleInput) == TRUE) {
            PlaySE(1501);
            data->state++;
        }
        break;
    case 3:
        data->delay++;
        if (data->delay > 8) {
            BattleSystem_PauseRecording(data->battleSystem, 0);
            data->state++;
        }
        break;
    case 4:
        break;
    }
}
