#include "to52_overlay41_prompt_text_private.h"

u32 ov41_02247B7C(void *work) {
    u32 result = YesNoPrompt_HandleInput(PTR(work, 0x6b8));
    switch (result) {
    case 0:
        return 4;
    case 1:
        result = 8;
        break;
    case 2:
        result = 9;
        break;
    }
    U32(work, 0x6ec) = YesNoPrompt_IsInTouchMode(PTR(work, 0x6b8));
    return result;
}

void ov41_02247BB8(void *work, int msgId, u8 x, u8 y, u8 width, u8 height) {
    MsgData *msgData;
    String *string;
    u32 frame = Options_GetFrame(PTR(work, 0x6dc));
    LoadFontPal1((enum GFPalLoadLocation)0, (enum GFPalSlotOffset)0xe0, HEAP_ID_14);
    AddWindowParameterized(PTR(work, 0x40), PTR(work, 0x6bc), 3, x, y, width, height, 7, 0x5a);
    FillWindowPixelBuffer(PTR(work, 0x6bc), 0xf);
    LoadUserFrameGfx2(PTR(work, 0x40), (GFBgLayer)3, 0x3c, 8, frame, HEAP_ID_14);
    DrawFrameAndWindow2(PTR(work, 0x6bc), FALSE, 0x3c, 8);
    msgData = NewMsgDataFromNarc((MsgDataLoadType)0, (NarcId)0x1b, 0xd7, HEAP_ID_13);
    string = NewString_ReadMsgData(msgData, msgId);
    AddTextPrinterParameterizedWithColor(PTR(work, 0x6bc), (FontID)1, string, 0, 0, 0, 0x1020f, NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    CopyWindowToVram(PTR(work, 0x6bc));
}

void ov41_02247C7C(void *work, int msgId) {
    MsgData *msgData;
    String *raw;
    String *expanded;
    FillWindowPixelBuffer(PTR(work, 0x6bc), 0xf);
    msgData = NewMsgDataFromNarc((MsgDataLoadType)0, (NarcId)0x1b, 0xd7, HEAP_ID_13);
    raw = NewString_ReadMsgData(msgData, msgId);
    expanded = String_New(0x100, HEAP_ID_13);
    StringExpandPlaceholders(PTR(work, 0x6e8), expanded, raw);
    AddTextPrinterParameterizedWithColor(PTR(work, 0x6bc), (FontID)1, expanded, 0, 0, 0, 0x1020f, NULL);
    String_Delete(expanded);
    String_Delete(raw);
    DestroyMsgData(msgData);
    CopyWindowToVram(PTR(work, 0x6bc));
}
