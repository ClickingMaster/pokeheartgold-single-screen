#include "slop_r1_overlay98_module11_internal.h"

Ov98TextContext *ov98_0221EABC(enum HeapID heapId, BgConfig *bgConfig, int windowCount, const WindowTemplate *templates, s32 msgBank) {
    Ov98TextContext *textSys;
    u32 windowsSize;
    int i;

    FontID_Alloc(4, heapId);
    textSys = Heap_Alloc(heapId, sizeof(Ov98TextContext));
    MI_CpuFill8(textSys, 0, sizeof(Ov98TextContext));
    windowsSize = windowCount * sizeof(Window);
    textSys->windows = Heap_Alloc(heapId, windowsSize);
    MI_CpuFill8(textSys->windows, 0, windowsSize);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, heapId);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, heapId);
    for (i = 0; i < windowCount; i++) {
        AddWindow(bgConfig, &textSys->windows[i], &templates[i]);
        FillWindowPixelBuffer(&textSys->windows[i], 0);
        ClearWindowTilemap(&textSys->windows[i]);
    }
    textSys->msgDataFixed = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0237_bin, heapId);
    textSys->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, msgBank, heapId);
    textSys->msgFormat = MessageFormat_New(heapId);
    textSys->string = String_New(0x400, heapId);
    textSys->heapId = heapId;
    textSys->windowCount = windowCount;
    return textSys;
}
