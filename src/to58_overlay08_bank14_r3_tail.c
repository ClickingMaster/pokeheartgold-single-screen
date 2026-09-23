#include "to57_overlay08_bank14_private.h"
#pragma force_active on
void *String_New(int, int);
void BufferIntegerAsString(void *, int, int, int, int, int);
void ov08_0221F220(void *);
void ov08_0221F284(void *, void *, int);
void ov08_0221F4A4(void *);
int Party_GetCount(void *);
int Pokemon_GetStatusIconId(void *);
void ov08_0221DDCC(void *, int, int, int, int, int);
void ov08_0221ED30(void *, int, int);
void ov08_0221EE60(void *, int, u32);
void ov08_0221F07C(void *, int, int, int);
void ov08_0221EC6C(void *, int);
void ov08_0221EC6C(void *arg, int battler) {
    Ov08R2W11Work *work = arg;
    int selector = *((u8 *)work + 0x2075) * 22;
    void *msgData = NewMsgDataFromNarc(1, 0x1B, 0x2D2, *(int *)((u8 *)work->root + 0xC));
    u16 msgNo = *(u16 *)((u8 *)work + battler * 0x50 + 0x1C);
    void *string = NewString_ReadMsgData(msgData, msgNo);
    selector = (selector + 2) << 4;
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)work->windowArray + selector), 0, string, 0, 0, 0xFF, 0x10200, 0);
    String_Delete(string);
    DestroyMsgData(msgData);
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + selector));
}

void ov08_0221ECE8(void *arg, int window) {
    Ov08R2W11Work *work = arg;
    Window *windows = work->windowArray;
    Window *target = &windows[window];
    void *string = NewString_ReadMsgData(work->msgData, 0x33);
    AddTextPrinterParameterizedWithColor(target, 0, string, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram(target);
}

void ov08_0221ED30(void *, int, int);
void *String_New(int, int);
void BufferIntegerAsString(void *, int, int, int, int, int);
void ov08_0221ED30(void *arg, int windowIndex, int value) {
    Ov08R2W11Work *work = arg;
    Window *windows = work->windowArray;
    Window *target = &windows[windowIndex];
    if (value == 0) {
        void *string = NewString_ReadMsgData(work->msgData, 0x32);
        u16 width = FontID_String_GetWidth(0, string, 0);
        u16 x = GetWindowWidth(target) * 8 - width;
        AddTextPrinterParameterizedWithColor(target, 0, string, x, 0, 0xFF, 0x10200, 0);
        String_Delete(string);
    } else {
        void *template = NewString_ReadMsgData(work->msgData, 0x34);
        void *string = String_New(8, *(int *)((u8 *)work->root + 0xC));
        BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, value, 3, 0, 1);
        StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), string, template);
        u16 width = FontID_String_GetWidth(0, string, 0);
        u16 x = GetWindowWidth(target) * 8 - width;
        AddTextPrinterParameterizedWithColor(target, 0, string, x, 0, 0xFF, 0x10200, 0);
        String_Delete(template);
        String_Delete(string);
    }
    ScheduleWindowCopyToVram(target);
}

void ov08_0221EE18(void *arg, int window) {
    Ov08R2W11Work *work = arg;
    Window *windows = work->windowArray;
    Window *target = &windows[window];
    void *string = NewString_ReadMsgData(work->msgData, 0x30);
    AddTextPrinterParameterizedWithColor(target, 0, string, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram(target);
}

void ov08_0221EE60(void *, int, u32);
void *String_New(int, int);
void BufferIntegerAsString(void *, int, int, int, int, int);
void ov08_0221EE60(void *arg, int windowIndex, u32 value) {
    Ov08R2W11Work *work = arg;
    Window *windows = work->windowArray;
    Window *target = &windows[windowIndex];
    if (value <= 1) {
        void *string = NewString_ReadMsgData(work->msgData, 0x32);
        u16 width = FontID_String_GetWidth(0, string, 0);
        u16 x = GetWindowWidth(target) * 8 - width;
        AddTextPrinterParameterizedWithColor(target, 0, string, x, 0, 0xFF, 0x10200, 0);
        String_Delete(string);
    } else {
        void *template = NewString_ReadMsgData(work->msgData, 0x31);
        void *string = String_New(8, *(int *)((u8 *)work->root + 0xC));
        BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, value, 3, 0, 1);
        StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), string, template);
        u16 width = FontID_String_GetWidth(0, string, 0);
        u16 x = GetWindowWidth(target) * 8 - width;
        AddTextPrinterParameterizedWithColor(target, 0, string, x, 0, 0xFF, 0x10200, 0);
        String_Delete(template);
        String_Delete(string);
    }
    ScheduleWindowCopyToVram(target);
}

void ov08_0221EF48(void *arg, int window, int msgNo) {
    Ov08R2W11Work *work = arg;
    Window *windows = work->windowArray;
    Window *target = &windows[window];
    void *msgData = NewMsgDataFromNarc(1, 0x1B, 0x2ED, *(int *)((u8 *)work->root + 0xC));
    void *string = NewString_ReadMsgData(msgData, msgNo);
    AddTextPrinterParameterizedWithColor(target, 0, string, 0, 0, 0xFF, 0x10200, 0);
    String_Delete(string);
    DestroyMsgData(msgData);
    ScheduleWindowCopyToVram(target);
}


void ov08_0221EFA8(void *arg, int window) {
    Ov08R2W11Work *work = arg;
    Window *windows = work->windowArray;
    Window *target = &windows[window];
    void *string = NewString_ReadMsgData(work->msgData, 0x35);
    u16 width = FontID_String_GetWidth(0, string, 0);
    u16 x = (GetWindowWidth(target) * 8 - width) / 2;
    AddTextPrinterParameterizedWithColor(target, 0, string, x, 0, 0xFF, 0xF0E00, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram(target);
}

void ov08_0221F00C(void *arg, int window, int kind) {
    Ov08R2W11Work *work = arg;
    Window *windows = work->windowArray;
    Window *target = &windows[window];
    void *string;
    switch (kind) {
    case 0: string = NewString_ReadMsgData(work->msgData, 0x36); break;
    case 1: string = NewString_ReadMsgData(work->msgData, 0x38); break;
    case 2: string = NewString_ReadMsgData(work->msgData, 0x37); break;
    }
    AddTextPrinterParameterizedWithColor(target, 0, string, 0, 0, 0xFF, 0x10200, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram(target);
}

void ov08_0221F07C(void *, int, int, int);
void *String_New(int, int);
void BufferIntegerAsString(void *, int, int, int, int, int);
void ov08_0221F07C(void *arg, int windowIndex, int left, int right) {
    Ov08R2W11Work *work = arg;
    Window *windows = work->windowArray;
    Window *target = &windows[windowIndex];
    void *label = NewString_ReadMsgData(work->msgData, 0x2E);
    u32 width = FontID_String_GetWidth(0, label, 0);
    u32 middle = (GetWindowWidth(target) * 8 - width) / 2;
    AddTextPrinterParameterizedWithColor(target, 0, label, middle, 0, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    label = NewString_ReadMsgData(work->msgData, 0x2C);
    void *expanded = String_New(6, *(int *)((u8 *)work->root + 0xC));
    BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, left, 3, 0, 1);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), expanded, label);
    u32 expandedWidth = FontID_String_GetWidth(0, expanded, 0);
    AddTextPrinterParameterizedWithColor(target, 0, expanded, middle - expandedWidth, 0, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    String_Delete(expanded);
    label = NewString_ReadMsgData(work->msgData, 0x2D);
    expanded = String_New(6, *(int *)((u8 *)work->root + 0xC));
    BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, right, 3, 0, 1);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), expanded, label);
    AddTextPrinterParameterizedWithColor(target, 0, expanded, middle + width, 0, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    String_Delete(expanded);
    ScheduleWindowCopyToVram(target);
}

void ov08_0221F1B0(void *arg, int window) {
    Ov08R2W11Work *work = arg;
    Window *windows = work->windowArray;
    Window *target = &windows[window];
    void *string;
    u32 width;
    if (*(u8 *)((u8 *)work->root + 0x34) == 4) string = NewString_ReadMsgData(work->msgData, 0x3B);
    else string = NewString_ReadMsgData(work->msgData, 0x3A);
    width = FontID_String_GetWidth(4, string, 0);
    AddTextPrinterParameterizedWithColor(target, 4, string, (0x60 - width) / 2, 5, 0xFF, 0x70809, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram(target);
}

void ov08_0221F220(void *);
void ov08_0221F220(void *arg) {
    Ov08R2W11Work *work = arg;
    Window *window;
    void *string;
    if (*((u8 *)work + 0x207A) == 7) {
        window = (Window *)((u8 *)work->windowArray + 0x80);
    } else {
        window = (Window *)((u8 *)work->windowArray + 0x50);
    }
    FillWindowPixelBuffer(window, 0);
    string = NewString_ReadMsgData(work->msgData, 0x3C);
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, 0xFF, 0x10200, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}

void ov08_0221F284(void *, void *, int);
void *String_New(int, int);
void BufferIntegerAsString(void *, int, int, int, int, int);
void ov08_0221F284(void *arg, void *value, int windowIndex) {
    Ov08R2W11Work *work = arg;
    Window *windows = work->windowArray;
    Window *target = &windows[windowIndex];
    void *result = String_New(6, *(int *)((u8 *)work->root + 0xC));
    void *label = NewString_ReadMsgData(work->msgData, 0x2B);
    AddTextPrinterParameterizedWithColor(target, 0, label, 0x28, 0x18, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    label = NewString_ReadMsgData(work->msgData, 0x2E);
    u32 width = FontID_String_GetWidth(0, label, 0);
    AddTextPrinterParameterizedWithColor(target, 0, label, 0x50, 0x18, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    label = NewString_ReadMsgData(work->msgData, 0x2D);
    BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, ((u8 *)value)[3], 2, 0, 1);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), result, label);
    width += 0x50;
    AddTextPrinterParameterizedWithColor(target, 0, result, width, 0x18, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    label = NewString_ReadMsgData(work->msgData, 0x2C);
    BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, ((u8 *)value)[2], 2, 0, 1);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), result, label);
    width = FontID_String_GetWidth(0, result, 0);
    AddTextPrinterParameterizedWithColor(target, 0, result, 0x50 - width, 0x18, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    String_Delete(result);
    ScheduleWindowCopyToVram(target);
}

void ov08_0221F3D0(void *, void *, int);
void *String_New(int, int);
void BufferIntegerAsString(void *, int, int, int, int, int);

void ov08_0221F3D0(void *arg, void *value, int windowIndex) {
    Ov08R2W11Work *work = arg;
    Window *windows = work->windowArray;
    Window *window = &windows[windowIndex];
    void *result = String_New(6, *(int *)((u8 *)work->root + 0xC));
    void *label = NewString_ReadMsgData(work->msgData, 0x2B);
    u32 width = FontID_String_GetWidth(0, label, 0);
    String_Delete(label);
    width += 0x28;
    FillWindowPixelRect(window, 0, (u16)width, 0x18, (u16)(0x50 - width), 0x10);
    label = NewString_ReadMsgData(work->msgData, 0x2C);
    BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, ((u8 *)value)[2], 2, 0, 1);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), result, label);
    width = FontID_String_GetWidth(0, result, 0);
    AddTextPrinterParameterizedWithColor(window, 0, result, 0x50 - width, 0x18, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    String_Delete(result);
    ScheduleWindowCopyToVram(window);
}

void ov08_0221F4A4(void *);
int Party_GetCount(void *);
int Pokemon_GetStatusIconId(void *);
void ov08_0221DDCC(void *, int, int, int, int, int);
void ov08_0221F4A4(void *arg) {
    Ov08R2W11Work *work = arg;
    int i = 0;
    if (Party_GetCount(*(void **)work->root) > 0) {
        u32 windowOffset = 0;
        u8 *mon = (u8 *)work;
        u8 *flag = (u8 *)work + 0x1B;
        do {
            FillWindowPixelBuffer((Window *)((u8 *)work->windowArray + windowOffset), 0);
            if (*(u16 *)(mon + 8) != 0) {
                ov08_0221DDCC(work, i, 4, (u16)i, 0x20, 7);
                if ((u32)(*flag << 24) >> 31 == 0) {
                    ov08_0221F550(work, (u8)i);
                }
                if (Pokemon_GetStatusIconId(*(void **)(mon + 4)) == 7) {
                    ov08_0221F5B0(work, (u8)i);
                }
            }
            mon += 0x50;
            flag += 0x50;
            windowOffset += 0x10;
            i++;
        } while (i < Party_GetCount(*(void **)work->root));
    }
    if (*((u8 *)work->root + 0x35) == 2) {
        ov08_0221E340(work, 7);
    } else {
        ov08_0221E340(work, 6);
    }
}
