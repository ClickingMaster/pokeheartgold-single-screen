#include "slop_r1_module_05_private.h"

void ov18_021F8CCC(SlopR1Module05Work *work) {
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *strbuf;
    String *string;
    u32 i;
    int x;
    for (i = 0; i < NELEMS(work->windows); i++) {
        AddWindow(work->bgConfig, &work->windows[i], &ov18_021FBDB4[i]);
        FillWindowPixelBuffer(&work->windows[i], 0);
    }

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0802_bin, work->heapId);
    msgFormat = MessageFormat_New(work->heapId);
    strbuf = String_New(0x400, work->heapId);

    ov18_021F9648(&work->windows[0], msgData, 0x90, 0x70, 0, 4, MAKE_TEXT_COLOR(2, 1, 0), 2);

    BufferIntegerAsString(msgFormat, 0, Pokedex_ConvertToCurrentDexNo(work->natDexEnabled, work->species), 3, PRINTING_MODE_LEADING_ZEROS, TRUE);
    string = NewString_ReadMsgData(msgData, 9);
    StringExpandPlaceholders(msgFormat, strbuf, string);
    ov18_021F95FC(&work->windows[1], strbuf, 1, 0, 4, MAKE_TEXT_COLOR(2, 1, 0), 0);
    String_Delete(string);

    string = ov18_021E590C(work->species, 2, work->heapId);
    ov18_021F95FC(&work->windows[2], string, 0, 0, 4, MAKE_TEXT_COLOR(2, 1, 0), 0);
    String_Delete(string);

    string = ov18_021E595C(work->species, 2, work->heapId);
    x = GetWindowWidth(&work->windows[3]) * 8 - 4;
    ov18_021F95FC(&work->windows[3], string, x, 0, 4, MAKE_TEXT_COLOR(2, 1, 0), 1);
    String_Delete(string);

    string = ov18_021E59A8(work->species, 2, 0, work->heapId);
    x = (GetWindowWidth(&work->windows[4]) * 8 - FontID_String_GetWidthMultiline(0, string, 0)) / 2;
    ov18_021F95FC(&work->windows[4], string, x, 0, 0, MAKE_TEXT_COLOR(2, 1, 0), 0);
    String_Delete(string);

    ov18_021F9648(&work->windows[5], msgData, 0xA, 0x14, 0, 0, MAKE_TEXT_COLOR(2, 1, 0), 2);
    ov18_021F9648(&work->windows[7], msgData, 0xB, 0x14, 0, 0, MAKE_TEXT_COLOR(2, 1, 0), 2);

    MessageFormat_Delete(msgFormat);
    DestroyMsgData(msgData);

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, GetDexHeightMsgBank(), work->heapId);
    ov18_021F9648(&work->windows[6], msgData, work->species, 4, 0, 0, MAKE_TEXT_COLOR(2, 1, 0), 0);
    DestroyMsgData(msgData);

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, GetDexWeightMsgBank(), work->heapId);
    ov18_021F9648(&work->windows[8], msgData, work->species, 4, 0, 0, MAKE_TEXT_COLOR(2, 1, 0), 0);
    DestroyMsgData(msgData);

    String_Delete(strbuf);

    for (i = 0; i < NELEMS(work->windows); i++) {
        ScheduleWindowCopyToVram(&work->windows[i]);
    }
}
