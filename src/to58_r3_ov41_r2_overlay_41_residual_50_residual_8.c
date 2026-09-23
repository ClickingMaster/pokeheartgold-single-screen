#include "global.h"

void *ov41_0224A928(void *work, int narc, int group, int message, int width, int height);
void *NewMsgDataFromNarc(int, int, int, int);
void GF_AssertFail(void);
void *NewString_ReadMsgData(void *, int);
void *AllocWindows(int, int);
void InitWindow(void *);
void AddTextWindowTopLeftCorner(void *, void *, u8, u8, int, int);
int FontID_String_GetCenterAlignmentX(int, void *, int, int);
void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, int, int);
void String_Delete(void *);
void DestroyMsgData(void *);

void *ov41_0224A928(void *work, int narc, int group, int message, int width, int height)
{
    void *messages = NewMsgDataFromNarc(0, narc, group, 0xd);
    void *str;
    void *window;
    int center;
    if (messages == NULL) {
        GF_AssertFail();
    }
    str = NewString_ReadMsgData(messages, message);
    window = AllocWindows(0xe, 1);
    InitWindow(window);
    AddTextWindowTopLeftCorner(*(void **)((u8 *)work + 0x40), window,
                               (u8)width, (u8)height, 0, 0);
    center = FontID_String_GetCenterAlignmentX(2, str, 0, width * 8);
    AddTextPrinterParameterizedWithColor(window, 2, str, center, 0, 0xff,
                                         0x00010203, 0);
    String_Delete(str);
    DestroyMsgData(messages);
    return window;
}
