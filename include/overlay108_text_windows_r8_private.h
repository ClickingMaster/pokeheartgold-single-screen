#ifndef POKEHEARTGOLD_OVERLAY108_TEXT_WINDOWS_R8_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY108_TEXT_WINDOWS_R8_PRIVATE_H
#include "global.h"
typedef struct Ov108TextWork {
    u32 heapId;
    u8 pad04[0x304 - 4];
    void *msgData;
    void *messageFormat;
    void *string;
    void *categoryStrings[5];
    void *optionStrings[4];
    void *specialString1;
    void *specialString2;
} Ov108TextWork;
void FontID_Alloc(u32 fontId, u32 heapId);
void FontID_Release(u32 fontId);
void *NewMsgDataFromNarc(u32 type, u32 narcId, u32 fileId, u32 heapId);
void DestroyMsgData(void *msgData);
void *MessageFormat_New_Custom(u32 maxArgs, u32 heapId, u32 mode);
void MessageFormat_Delete(void *messageFormat);
void *String_New(u32 maxLength, u32 heapId);
void *NewString_ReadMsgData(void *msgData, u32 msgId);
void String_Delete(void *string);
void ov108_021E6F74(Ov108TextWork *work);
void ov108_021E7014(Ov108TextWork *work);
#endif
