#include "constants/sndseq.h"

#include "msgdata/msg/msg_0196.h"

#include "filesystem_files_def.h"
#include "slop_r1_overlay32_module02_internal.h"
#include "sys_task_api.h"

void ov32_0225D788(NamePickerData *data) {
    String *string;
    u16 i;
    u16 center;
    u16 x;

    for (i = 0; i < NAME_PICKER_NUM_WINDOWS; i++) {
        AddWindow(data->bgConfig, &data->windows[i], &ov32_0225E24C[i]);
    }
    FillWindowPixelBuffer(&data->windows[0], 0);
    string = NewString_ReadMsgData(data->msgData, msg_0196_00029);
    center = GetWindowWidth(&data->windows[0]) * 8 / 2;
    x = center - FontID_String_GetWidth(4, string, 0) / 2;
    AddTextPrinterParameterizedWithColor(&data->windows[0], 4, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
    String_Delete(string);
    CopyWindowPixelsToVram_TextMode(&data->windows[0]);
    ScheduleWindowCopyToVram(&data->windows[0]);
    ov32_0225D84C(data);
    ov32_0225D988(data);
}
