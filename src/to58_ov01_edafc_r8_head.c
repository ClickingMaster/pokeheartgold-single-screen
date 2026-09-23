#include "global.h"
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
extern void FillWindowPixelRect(void *, int, int, int, int, int);
extern void *NewMsgDataFromNarc(int, int, int, int);
extern void *MessageFormat_New(int);
extern void *String_New(int, int);
extern void *NewString_ReadMsgData(void *, int);
extern void *Save_PlayerData_GetProfile(void *);
extern int PlayerProfile_GetMoney(void *);
extern void BufferIntegerAsString(void *, int, int, int, int, int);
extern void StringExpandPlaceholders(void *, void *, void *);
extern int FontID_String_GetWidth(int, void *, int);
extern void AddTextPrinterParameterized(void *, int, void *, int, int, int, int);
extern void String_Delete(void *);
extern void MessageFormat_Delete(void *);
extern void DestroyMsgData(void *);
extern void ScheduleWindowCopyToVram(void *);
void MoneyBoxSys_Update(void *, void *);
void MoneyBoxSys_Update(void *ctx, void *window)
{
    void *msg;
    void *fmt;
    void *text;
    void *raw;
    int x;
    FillWindowPixelRect(window, 15, 0, 16, 80, 16);
    msg = NewMsgDataFromNarc(0, 27, 0x1b3, 4);
    fmt = MessageFormat_New(4);
    text = String_New(16, 4);
    raw = NewString_ReadMsgData(msg, 31);
    BufferIntegerAsString(fmt, 0, PlayerProfile_GetMoney(Save_PlayerData_GetProfile(P(ctx, 0xc))), 6, 1, 1);
    StringExpandPlaceholders(fmt, text, raw);
    x = 80 - FontID_String_GetWidth(0, text, 0);
    AddTextPrinterParameterized(window, 0, text, x, 16, 255, 0);
    String_Delete(raw);
    String_Delete(text);
    MessageFormat_Delete(fmt);
    DestroyMsgData(msg);
    ScheduleWindowCopyToVram(window);
}
