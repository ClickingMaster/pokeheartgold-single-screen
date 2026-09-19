#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "string.h"
#include "text.h"

typedef struct Ov51State {
    u8 raw[0x4000];
} Ov51State;

typedef struct Ov51Card {
    u8 raw[0x48];
} Ov51Card;

#define PTR_AT(p, o, T) (*(T **)((u8 *)(p) + (o)))
#define U8_AT(p, o)     (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o)    (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o)    (*(u32 *)((u8 *)(p) + (o)))

extern void ov51_021E74F4(Window *, u32, u32, u32, String *, u32, u32, u32, u32);
extern u32 FontID_String_GetWidth(u32, String *, u32);
void ov51_021E7208(Ov51State *, Window *, Ov51Card *);

void ov51_021E7208(Ov51State *state, Window *windows, Ov51Card *card) {
    String *string;
    String *formatted;
    MessageFormat *format;
    MsgData *msgData;
    u8 i;
    int x;

    msgData = PTR_AT(state, 0x33C4, MsgData);
    for (i = 7; i < 11; i++) {
        FillWindowPixelBuffer(&windows[i], 0);
        AddTextPrinterParameterizedWithColor(&windows[i], 0, *(String **)((u8 *)state + i * 4 + 0x33EC), 0, 0, 0xFF, 0x00010200, NULL);
    }

    string = String_New(0x20, (enum HeapID)0x19);
    formatted = PTR_AT(state, 0x33D0, String);
    format = MessageFormat_New_Custom(6, 0x20, (enum HeapID)0x19);

    if (U8_AT(card, 0x33) != 0) {
        BufferIntegerAsString(format, 2, U8_AT(card, 0x32), 2, (PrintingMode)2, TRUE);
        BufferMonthNameAbbr(format, 3, U8_AT(card, 0x33));
        BufferIntegerAsString(format, 4, U8_AT(card, 0x34), 2, (PrintingMode)2, TRUE);
        BufferIntegerAsString(format, 0, U16_AT(card, 0x2C), 3, (PrintingMode)1, TRUE);
        BufferIntegerAsString(format, 1, U8_AT(card, 0x35), 2, (PrintingMode)2, TRUE);
        ReadMsgDataIntoString(msgData, 0x16, string);
        StringExpandPlaceholders(format, formatted, string);
    } else {
        ReadMsgDataIntoString(msgData, 0x0C, string);
        BufferString(format, 0, string, 0, 0, 2);
        BufferString(format, 1, string, 0, 0, 2);
        ReadMsgDataIntoString(msgData, 0x19, formatted);
    }

    x = 0xE0 - FontID_String_GetWidth(0, formatted, 0);
    AddTextPrinterParameterizedWithColor(&windows[7], 0, formatted, x, 0, 0xFF, 0x00010200, NULL);

    ReadMsgDataIntoString(msgData, 0x14, string);
    StringExpandPlaceholders(format, formatted, string);
    x = 0xE0 - FontID_String_GetWidth(0, formatted, 0);
    AddTextPrinterParameterizedWithColor(&windows[7], 0, formatted, x, 0x10, 0xFF, 0x00010200, NULL);

    BufferIntegerAsString(format, 5, U32_AT(card, 0x38), 6, (PrintingMode)0, TRUE);
    ReadMsgDataIntoString(msgData, 0x1B, string);
    StringExpandPlaceholders(format, formatted, string);
    x = 0xE0 - FontID_String_GetWidth(0, formatted, 0);
    AddTextPrinterParameterizedWithColor(&windows[8], 0, formatted, x, 0, 0xFF, 0x00010200, NULL);

    ReadMsgDataIntoString(msgData, 0x17, formatted);
    AddTextPrinterParameterizedWithColor(&windows[9], 0, formatted, 0x70, 0, 0xFF, 0x00010200, NULL);
    ov51_021E74F4(&windows[9], 0xE0, 0, 0, formatted, U32_AT(card, 0x40), 4, 1, 0xFF);

    ReadMsgDataIntoString(msgData, 0x18, formatted);
    AddTextPrinterParameterizedWithColor(&windows[9], 0, formatted, 0xB0, 0, 0xFF, 0x00010200, NULL);
    ov51_021E74F4(&windows[9], 0xE0, 0x40, 0, formatted, U32_AT(card, 0x3C), 4, 1, 0xFF);

    BufferIntegerAsString(format, 5, U32_AT(card, 0x44), 6, (PrintingMode)0, TRUE);
    ReadMsgDataIntoString(msgData, 0x1B, string);
    StringExpandPlaceholders(format, formatted, string);
    x = 0xE0 - FontID_String_GetWidth(0, formatted, 0);
    AddTextPrinterParameterizedWithColor(&windows[10], 0, formatted, x, 0, 0xFF, 0x00010200, NULL);

    String_Delete(string);
    MessageFormat_Delete(format);
}
