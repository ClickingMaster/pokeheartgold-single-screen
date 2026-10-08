#pragma force_active on

#include "global.h"

#define PTR(p, off)  (*(void **)((u8 *)(p) + (off)))
#define WORD(p, off) (*(u32 *)((u8 *)(p) + (off)))

typedef struct Ov40WindowTemplate {
    s16 x;
    s16 y;
    s16 width;
    s16 height;
} Ov40WindowTemplate;

typedef struct Ov40WindowTemplateSet {
    Ov40WindowTemplate templates[5];
} Ov40WindowTemplateSet;

typedef struct Ov40MsgIdSet {
    u32 msgIds[6];
} Ov40MsgIdSet;

extern const Ov40WindowTemplateSet ov40_02244EE8;
extern const Ov40MsgIdSet ov40_02244E28;

void InitWindow(void *window);
void AddWindowParameterized(void *bgConfig, void *window, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 paletteNum, u16 baseTile);
void FillWindowPixelBuffer(void *window, u8 fillValue);
void AddTextPrinterParameterizedWithColor(void *window, int fontId, void *str, int x, int y, int speed, u32 color, void *callback);
void ScheduleWindowCopyToVram(void *window);

void *String_New(u32 size, u32 heapId);
void String_Delete(void *str);
void String16_FormatInteger(void *str, int value, u32 digits, int mode, BOOL charset);
void StringExpandPlaceholders(void *format, void *dst, void *src);
void *NewString_ReadMsgData(void *msgData, int msgId);

void *ov40_0222DAB0(u32 heapId);
void BufferString(void *format, u32 index, void *str, u32 a3, u32 a4, u32 a5);
void MessageFormat_ResetBuffers(void *format);
void MessageFormat_Delete(void *format);

void *sub_020315B8(u32 value, int heapId);
u64 sub_0203088C(void *summary, int field, int index);
void ov40_02230DCC(void *p, void *str);
int ov40_022306C0(void *window, void *str);

void ov40_0222FF74(void *work, void *p);


// @fn ov40_0222FF74
void ov40_0222FF74(void *work, void *p) {
    Ov40WindowTemplateSet templates = ov40_02244EE8;
    Ov40MsgIdSet msgIds = ov40_02244E28;
    void *unitStr;
    void *numberStr;
    u32 value;
    void *bodyStr;
    void *nameStr;
    int i;
    int baseTile;
    void *digitsLo;
    void *digitsMid;
    void *digitsHi;
    u32 lo;
    u32 mid;
    u32 hi;
    const u32 *msgId = msgIds.msgIds;
    const Ov40WindowTemplate *tmpl;
    void *window;
    void *fmt;
    void *str;
    void *out;
    u64 total;
    u64 rest;
    u32 trainerId;
    int msgId2;

    baseTile = 1;
    for (i = 0, tmpl = templates.templates, window = (u8 *)work + 0x3C; i < 5; i++) {
        if (tmpl->x == 0xFF) {
            break;
        }
        str = NewString_ReadMsgData(PTR(p, 0x48), *msgId);
        InitWindow(window);
        AddWindowParameterized(PTR(p, 0x24), window, 2, tmpl->x, tmpl->y, tmpl->width, tmpl->height, 0xE, baseTile);
        FillWindowPixelBuffer(window, 0);
        AddTextPrinterParameterizedWithColor(window, 0, str, ov40_022306C0(window, str), 0, 0xFF, 0xF0D00, NULL);
        ScheduleWindowCopyToVram(window);
        baseTile += tmpl->width * tmpl->height;
        String_Delete(str);
        msgId++;
        tmpl++;
        window = (u8 *)window + 0x10;
    }

    trainerId = WORD(work, 0);
    fmt = ov40_0222DAB0(0x6D);
    window = (u8 *)work + 0x3C;
    nameStr = sub_020315B8(trainerId, 0x6D);
    ov40_02230DCC(p, nameStr);
    bodyStr = NewString_ReadMsgData(PTR(p, 0x48), 7);
    out = String_New(0xFF, 0x6D);
    BufferString(fmt, 0, nameStr, 0, 1, 2);
    StringExpandPlaceholders(fmt, out, bodyStr);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, out, ov40_022306C0(window, out), 0, 0xFF, 0xF0D00, NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(nameStr);
    String_Delete(bodyStr);
    String_Delete(out);
    MessageFormat_ResetBuffers(fmt);

    msgId2 = (int)sub_0203088C(PTR(work, 4), 3, 0);
    window = (u8 *)work + 0x4C;
    str = NewString_ReadMsgData(PTR(p, 0x48), msgId2 + 0x84);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, 0xFF, 0xF0D00, NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
    MessageFormat_ResetBuffers(fmt);

    value = (u32)sub_0203088C(PTR(work, 4), 2, 0);
    if (value != 0) {
        window = (u8 *)work + 0x5C;
        numberStr = String_New(0xFF, 0x6D);
        unitStr = NewString_ReadMsgData(PTR(p, 0x48), 9);
        out = String_New(0xFF, 0x6D);
        String16_FormatInteger(numberStr, value, 4, 0, 1);
        BufferString(fmt, 0, numberStr, 0, 1, 2);
        StringExpandPlaceholders(fmt, out, unitStr);
        FillWindowPixelBuffer(window, 0);
        AddTextPrinterParameterizedWithColor(window, 0, out, ov40_022306C0(window, out), 0, 0xFF, 0xF0D00, NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(numberStr);
        String_Delete(unitStr);
        String_Delete(out);
        MessageFormat_ResetBuffers(fmt);
    } else {
        window = (u8 *)work + 0x6C;
        FillWindowPixelBuffer(window, 0);
        ScheduleWindowCopyToVram(window);
    }

    total = sub_0203088C(PTR(work, 4), 4, 0);
    rest = total;
    digitsLo = String_New(0xFF, 0x6D);
    digitsMid = String_New(0xFF, 0x6D);
    digitsHi = String_New(0xFF, 0x6D);
    window = (u8 *)work + 0x7C;
    lo = (u32)(rest % 100000);
    rest /= 100000;
    mid = (u32)(rest % 100000);
    rest /= 100000;
    hi = (u32)rest;
    if (hi / 10 % 10 == 0 && total != 0) {
        str = NewString_ReadMsgData(PTR(p, 0x48), 12);
    } else {
        str = NewString_ReadMsgData(PTR(p, 0x48), 11);
    }
    out = String_New(0xFF, 0x6D);
    String16_FormatInteger(digitsLo, lo, 5, 2, 1);
    String16_FormatInteger(digitsMid, mid, 5, 2, 1);
    String16_FormatInteger(digitsHi, hi, 2, 2, 1);
    BufferString(fmt, 2, digitsLo, 0, 1, 2);
    BufferString(fmt, 1, digitsMid, 0, 1, 2);
    BufferString(fmt, 0, digitsHi, 0, 1, 2);
    StringExpandPlaceholders(fmt, out, str);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, out, ov40_022306C0(window, out), 0, 0xFF, 0xF0D00, NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(digitsLo);
    String_Delete(digitsMid);
    String_Delete(digitsHi);
    String_Delete(str);
    String_Delete(out);
    MessageFormat_ResetBuffers(fmt);
    MessageFormat_Delete(fmt);
}
