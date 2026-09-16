#include "constants/sndseq.h"

#include "msgdata/msg/msg_0196.h"

#include "filesystem_files_def.h"
#include "slop_r1_overlay32_module02_internal.h"
#include "sys_task_api.h"

void ov32_0225D84C(NamePickerData *data) {
    String *string;
    String *buf;
    String *template;
    u16 width;
    u16 center;
    u16 x;

    FillWindowPixelBuffer(&data->windows[1], 0);
    string = NewString_ReadMsgData(data->msgData, msg_0196_00028);
    width = FontID_String_GetWidth(0, string, 0);
    center = (GetWindowWidth(&data->windows[1]) * 8 - width) / 2;
    AddTextPrinterParameterizedWithColor(&data->windows[1], 0, string, center, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
    String_Delete(string);

    buf = String_New(4, HEAP_ID_8);
    template = NewString_ReadMsgData(data->msgData, msg_0196_00027);
    BufferIntegerAsString(data->msgFormat, 0, NAME_PICKER_NUM_PAGES, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(data->msgFormat, buf, template);
    AddTextPrinterParameterizedWithColor(&data->windows[1], 0, buf, center + width, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
    String_Delete(template);

    template = NewString_ReadMsgData(data->msgData, msg_0196_00027);
    BufferIntegerAsString(data->msgFormat, 0, data->page + 1, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(data->msgFormat, buf, template);
    x = center - FontID_String_GetWidth(0, buf, 0);
    AddTextPrinterParameterizedWithColor(&data->windows[1], 0, buf, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
    String_Delete(template);
    String_Delete(buf);
    CopyWindowPixelsToVram_TextMode(&data->windows[1]);
    ScheduleWindowCopyToVram(&data->windows[1]);
}

void ov32_0225D988(NamePickerData *data) {
    PlayerProfile *profile;
    String *template;
    String *buf;
    u32 center;
    Window *windows;
    u16 i;

    if (data->bufferToggle == 0) {
        windows = &data->windows[2];
    } else {
        windows = &data->windows[2 + NAME_PICKER_NAMES_PER_PAGE];
    }
    template = NewString_ReadMsgData(data->msgData, msg_0196_00030);
    buf = String_New(0x40, HEAP_ID_8);
    profile = PlayerProfile_New(HEAP_ID_8);
    center = (u16)(GetWindowWidth(&windows[0]) * 8 / 2);
    for (i = 0; i < NAME_PICKER_NAMES_PER_PAGE; i++) {
        Save_Profile_PlayerName_Set(profile, sub_0202C254(data->names, i + data->page * NAME_PICKER_NAMES_PER_PAGE));
        BufferPlayersName(data->msgFormat, 0, profile);
        StringExpandPlaceholders(data->msgFormat, buf, template);
        FillWindowPixelBuffer(&windows[i], 0);
        AddTextPrinterParameterizedWithColor(&windows[i], 4, buf, center - FontID_String_GetWidth(0, buf, 0) / 2, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
        CopyWindowPixelsToVram_TextMode(&windows[i]);
        ScheduleWindowCopyToVram(&windows[i]);
    }
    Heap_Free(profile);
    String_Delete(buf);
    String_Delete(template);
    data->bufferToggle ^= 1;
}

void ov32_0225DA88(NamePickerData *data, NARC *narc) {
    data->spriteList = G2dRenderer_Init(NAME_PICKER_NUM_SPRITES, &data->renderer, HEAP_ID_8);
    G2dRenderer_SetSubSurfaceCoords(&data->renderer, 0, FX32_CONST(256));
    ov32_0225DADC(data);
    ov32_0225DC68(data, narc);
}
