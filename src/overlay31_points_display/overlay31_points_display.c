#include "global.h"
#include "bg_window.h"

typedef struct Overlay31State { u8 data[0x300]; } Overlay31State;
typedef struct Overlay31Work {
    u8 pad_000[0x268];
    u16 *msgNos;
    u8 pad_26C[5];
    u8 first;
    u8 pad_272[0x1E];
    u32 current;
} Overlay31Work;
extern void *NewString_ReadMsgData(void *, int);
extern void String_Delete(void *);
extern int ov31_0225E12C(u8 *, int, int);
extern int ov03_02258120(u8 *, int);
extern void ov31_0225DE00(Overlay31State *, Window *, void *, int);
extern void ov31_0225DE24(void *, void *, Window *, int, int);
void ov31_0225E184(Overlay31State *);

void ov31_0225E184(Overlay31State *state) {
    Overlay31Work *work;
    int index;
    int offset;
    int msgNo;
    void *string;

    work = *(Overlay31Work **)&state->data[0x14];
    index = work->current + work->first;
    offset = index * 2;
    string = NewString_ReadMsgData(*(void **)&state->data[0x15C],
        *(u16 *)((u8 *)work->msgNos + offset));

    ov31_0225DE00(state, (Window *)&state->data[0xE4], string, index);
    String_Delete(string);
    work = *(Overlay31Work **)&state->data[0x14];
    msgNo = *(u16 *)((u8 *)work->msgNos + offset);
    if (ov31_0225E12C((u8 *)work, index, msgNo)) {
        int value;
        work = *(Overlay31Work **)&state->data[0x14];
        msgNo = *(u16 *)((u8 *)work->msgNos + offset);
        value = ov03_02258120((u8 *)work, msgNo);
        ov31_0225DE24(*(void **)&state->data[0x154], *(void **)&state->data[0x158], (Window *)&state->data[0xE4], value, ((u8 *)work)[0x283]);
    }
    ScheduleWindowCopyToVram((Window *)&state->data[0xE4]);
}

extern void *String_New(int, int);
extern int AddTextPrinterParameterizedWithColor(Window *, int, void *, int, int, int, int, int);
extern int BufferIntegerAsString(void *, int, int, int, int, int);
extern void StringExpandPlaceholders(void *, void *, void *);
extern int FontID_String_GetWidth(int, void *, int);
extern int ov03_02257978(u8 *, int);
void ov31_0225E20C(Overlay31State *, int);

void ov31_0225E20C(Overlay31State *state, int unused) {
    void *string;
    void *template;
    int value;
    int width;

    template = NewString_ReadMsgData(*(void **)&state->data[0x158], 0x23);
    AddTextPrinterParameterizedWithColor((Window *)&state->data[0x124], 0, template, 0, 4, 0xFF, 0x10200, 0);
    String_Delete(template);

    string = String_New(5, 0xB);
    template = NewString_ReadMsgData(*(void **)&state->data[0x158], 0x24);
    value = ov03_02257978(*(u8 **)&state->data[0x14], *(u16 *)(*(u8 **)&state->data[0x14] + 0x284));
    BufferIntegerAsString(*(void **)&state->data[0x154], 0, value, 3, 1, 1);
    StringExpandPlaceholders(*(void **)&state->data[0x154], string, template);
    width = FontID_String_GetWidth(0, string, 0);
    AddTextPrinterParameterizedWithColor((Window *)&state->data[0x124], 0, string, 0x40 - width, 0x14, 0xFF, 0x10200, 0);
    String_Delete(string);
    String_Delete(template);
    ScheduleWindowCopyToVram((Window *)&state->data[0x124]);
}

void ov31_0225E2D4(Overlay31State *, int);
void ov31_0225E2D4(Overlay31State *state, int amount) {
    void *string1;
    void *string2;
    void *template1;
    void *template2;
    int width;

    string1 = String_New(2, 0xB);
    string2 = String_New(2, 0xB);
    template1 = NewString_ReadMsgData(*(void **)&state->data[0x158], 0x2C);
    template2 = NewString_ReadMsgData(*(void **)&state->data[0x158], 0x2D);
    BufferIntegerAsString(*(void **)&state->data[0x154], 0, amount / 10, 1, 1, 1);
    BufferIntegerAsString(*(void **)&state->data[0x154], 1, amount % 10, 1, 1, 1);
    StringExpandPlaceholders(*(void **)&state->data[0x154], string1, template1);
    StringExpandPlaceholders(*(void **)&state->data[0x154], string2, template2);
    FillWindowPixelBuffer((Window *)&state->data[0xF4], 0);
    FillWindowPixelBuffer((Window *)&state->data[0x104], 0);
    AddTextPrinterParameterizedWithColor((Window *)&state->data[0xF4], 0, string1, 0, 4, 0xFF, 0x10200, 0);
    AddTextPrinterParameterizedWithColor((Window *)&state->data[0x104], 0, string2, 0, 4, 0xFF, 0x10200, 0);
    String_Delete(string1);
    String_Delete(string2);
    String_Delete(template1);
    String_Delete(template2);
    ScheduleWindowCopyToVram((Window *)&state->data[0xF4]);
    ScheduleWindowCopyToVram((Window *)&state->data[0x104]);

    FillWindowPixelBuffer((Window *)&state->data[0x134], 0);
    string1 = String_New(9, 0xB);
    template1 = NewString_ReadMsgData(*(void **)&state->data[0x158], 0x26);
    BufferIntegerAsString(*(void **)&state->data[0x154], 0,
        *(int *)(*(u8 **)&state->data[0x14] + 0x28C) * amount, 6, 1, 1);
    StringExpandPlaceholders(*(void **)&state->data[0x154], string1, template1);
    width = FontID_String_GetWidth(0, string1, 0);
    AddTextPrinterParameterizedWithColor((Window *)&state->data[0x134], 0, string1,
        0x40 - width, 4, 0xFF, 0x10200, 0);
    String_Delete(string1);
    String_Delete(template1);
    ScheduleWindowCopyToVram((Window *)&state->data[0x134]);
}
