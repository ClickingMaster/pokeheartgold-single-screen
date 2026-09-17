#include "overlay108_text_windows_r8_private.h"

void ov108_021E6F74(Ov108TextWork *work) {
    int i;

    FontID_Alloc(4, work->heapId);
    work->msgData = NewMsgDataFromNarc(0, 0x1B, 0x1AD, work->heapId);
    work->messageFormat = MessageFormat_New_Custom(1, 0x11, work->heapId);
    work->string = String_New(0x51, work->heapId);
    work->specialString1 = NewString_ReadMsgData(work->msgData, 9);
    work->specialString2 = NewString_ReadMsgData(work->msgData, 0xF);
    for (i = 0; i < 5; i++) {
        work->categoryStrings[i] = NewString_ReadMsgData(work->msgData, i + 4);
    }
    for (i = 0; i < 4; i++) {
        work->optionStrings[i] = NewString_ReadMsgData(work->msgData, i);
    }
}

void ov108_021E7014(Ov108TextWork *work) {
    int i;

    for (i = 0; i < 4; i++) {
        String_Delete(work->optionStrings[i]);
    }
    for (i = 0; i < 5; i++) {
        String_Delete(work->categoryStrings[i]);
    }
    String_Delete(work->specialString2);
    String_Delete(work->specialString1);
    String_Delete(work->string);
    MessageFormat_Delete(work->messageFormat);
    DestroyMsgData(work->msgData);
    FontID_Release(4);
}
