#include "constants/sndseq.h"

#include "msgdata/msg/msg_0196.h"

#include "filesystem_files_def.h"
#include "slop_r1_overlay32_module02_internal.h"
#include "sys_task_api.h"

SysTask *ov32_0225D520(BgConfig *bgConfig, void *a1, FieldSystem *fieldSystem, int *result) {
    SysTask *task;
    NamePickerData *data;
    NARC *narc;

    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18000);
    G2S_BlendNone();
    task = CreateSysTaskAndEnvironment(ov32_0225DDC4, sizeof(NamePickerData), 10, HEAP_ID_8);
    data = SysTask_GetData(task);
    data->bgConfig = bgConfig;
    data->unk4 = a1;
    data->fieldSystem = fieldSystem;
    data->unkC = result;
    data->task = task;
    data->names = sub_0202C6F4(fieldSystem->saveData);
    data->result = result;
    *data->result = -1;

    FontID_Alloc(4, HEAP_ID_8);
    narc = NARC_New(NARC_a_2_5_9, HEAP_ID_8);
    ov32_0225D60C();
    ov32_0225D634(bgConfig);
    ov32_0225D6E0(data, narc);
    ov32_0225D748(data);
    ov32_0225D788(data);
    ov32_0225DA88(data, narc);
    ov32_0225DD74(data);
    NARC_Delete(narc);
    return task;
}
