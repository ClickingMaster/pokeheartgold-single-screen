#include "constants/sndseq.h"

#include "msgdata/msg/msg_0196.h"

#include "filesystem_files_def.h"
#include "slop_r1_overlay32_module02_internal.h"
#include "sys_task_api.h"

void ov32_0225DD24(NamePickerData *data, int pos) {
    const DpadMenuBox *box;
    VecFx32 vec;

    box = GridInputHandler_GetDpadBox(data->gridInput, pos);
    vec.x = box->left << FX32_SHIFT;
    vec.y = (box->top << FX32_SHIFT) + FX32_CONST(256);
    Sprite_SetMatrix(data->sprites[NAME_PICKER_SPRITE_CURSOR], &vec);
    if (pos == NAME_PICKER_INPUT_CANCEL) {
        Sprite_SetAnimCtrlSeq(data->sprites[NAME_PICKER_SPRITE_CURSOR], 1);
    } else {
        Sprite_SetAnimCtrlSeq(data->sprites[NAME_PICKER_SPRITE_CURSOR], 0);
    }
}

void ov32_0225DD74(NamePickerData *data) {
    data->gridInput = GridInputHandler_Create(ov32_0225E1DC, ov32_0225E204, &ov32_0225E168, data, TRUE, 0, HEAP_ID_8);
    ov32_0225DD24(data, 0);
}
