#include "to52_r6_overlay96_results_graphics_r43_r3_private.h"
void ov96_021EE644(Ov96R43Work *work) {
    u8 kind = work->kind;
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 0xB, (enum GFPalLoadLocation)6, (enum GFPalSlotOffset)(6 << 12), 0, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, kind + 0xC, work->bgConfig, (GFBgLayer)7, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, kind + 0x16, work->bgConfig, (GFBgLayer)7, 0, 0, TRUE, work->heapId);
}
void ov96_021EE6A0(Ov96R43Work *work) {
    u8 kind = work->kind;
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, (NarcId)0x1B, 0x135, work->heapId);
    String *string = String_New(0x100, work->heapId);
    ov96_021EE700(&work->windowA, msgData, string, kind + 0xA6, 4);
    ov96_021EE700(&work->windowB, msgData, string, kind + 0xBB, 0);
    String_Delete(string);
    DestroyMsgData(msgData);
}
void ov96_021EE700(Window *window, MsgData *msgData, String *string, u32 msgId, u32 fontId) {
    ReadMsgDataIntoString(msgData, msgId, string);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, fontId, string, 0, 0, 0xFF, 0x00010200, NULL);
    ScheduleWindowCopyToVram(window);
}
