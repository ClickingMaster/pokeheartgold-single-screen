#pragma force_active on
#include "global.h"
extern void *VsRecorder_NewMessageFormat(int),*NewString_ReadMsgData(void *,int),*String_New(int,int);
extern void String_Delete(void *),InitWindow(void *),FillWindowPixelBuffer(void *,int),ScheduleWindowCopyToVram(void *),MessageFormat_Delete(void *),MessageFormat_ResetBuffers(void *),StringExpandPlaceholders(void *,void *,void *),BufferString(void *,int,void *,int,int,int);
extern int ov40_022306C0(void *,void *);
extern void AddWindowParameterized(void *,void *,u8,u8,u8,u8,u8,u8,u16);
extern void AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,u32,void *);
extern void String16_FormatInteger(void *,int,int,int,int);
#define PRINT(W,S) do { FillWindowPixelBuffer(W,0);AddTextPrinterParameterizedWithColor(W,0,S,ov40_022306C0(W,S),0,255,0xf0d00,0);ScheduleWindowCopyToVram(W); } while(0)
typedef struct {u32 unknown;int value;u64 time;} Entry;
typedef struct {Entry *entries;int count,spacing,bg,x,y,width,base,layer,rows;} Config;
typedef struct {u32 words[4];} Window;
typedef struct {int state,count;s16 unk8;s16 pad8;int unknownC,rows,pages;Window window;Config *config;u32 pad[2];void *messages;u32 pad2[2];int top,bottom;} State;
typedef struct {u8 pad[0x24];void *bg;u8 pad2[0x20];void *msg;} Context;
typedef struct {int v[3];} Limits;
extern Limits ov40_02244E1C;
extern void BufferMonthNameAbbr(void *,int,int),GetSpeciesNameIntoArray(int,int,u16 *),CopyU16ArrayToString(void *,const u16 *),String16_FormatUnsignedLongLong(void *,u64,int,int,int);
extern int ov40_022307B0(u64);
int ov40_0222F09C(State *w, Context *ctx, void *unused, u32 type, void *extra);
// @fn ov40_0222F09C
int ov40_0222F09C(State *w, Context *ctx, void *unused, u32 type, void *extra) {
    Window *win;
    void *label;
    void *number;
    void *fieldString;
    void *valueString;
    int row;
    int lastGroup;
    u64 previous;
    int start;
    int end;
    int month;
    int spec;
    u64 value;
    int i;
    void *fmt;
    void *expanded;

    if (w->unk8 == w->unknownC) {
        return 0;
    }
    if (w->rows == w->count) {
        return 0;
    }
    win = &w->window;
    fmt = VsRecorder_NewMessageFormat(109);
    FillWindowPixelBuffer(win, 0);
    row = 0;
    lastGroup = 0;
    start = w->unk8;
    end = start + w->rows;
    {
        int groups[30] = { 0 };
        u16 species[256];
        Limits limits;

        if (end >= w->count) {
            end = w->count;
            start = w->count - w->rows;
        }
        limits = ov40_02244E1C;
        previous = w->config->entries[0].time;
        groups[0] = 0;
        for (i = 1; i < limits.v[type]; i++) {
            u64 current = w->config->entries[i].time;
            if (current == previous) {
                groups[i] = lastGroup;
            } else {
                previous = current;
                groups[i] = i;
                lastGroup = i;
            }
        }
        for (i = start; i < end; i++) {
            label = NewString_ReadMsgData(ctx->msg, type + 88);
            number = String_New(255, 109);
            expanded = String_New(255, 109);
            valueString = String_New(255, 109);
            switch (type) {
            case 0:
                fieldString = NewString_ReadMsgData(w->messages, w->config->entries[i].value);
                BufferString(fmt, 1, fieldString, 0, 1, 2);
                break;
            case 1:
                month = w->config->entries[i].value;
                fieldString = String_New(255, 109);
                BufferMonthNameAbbr(fmt, 1, month);
                break;
            case 2:
                spec = w->config->entries[i].value;
                fieldString = String_New(255, 109);
                GetSpeciesNameIntoArray(spec, 109, species);
                CopyU16ArrayToString(fieldString, species);
                BufferString(fmt, 1, fieldString, 0, 1, 2);
                break;
            }
            value = w->config->entries[i].time;
            String16_FormatUnsignedLongLong(valueString, value, ov40_022307B0(value), 0, 1);
            String16_FormatInteger(number, groups[i] + 1, 2, 1, 1);
            BufferString(fmt, 0, number, 0, 1, 2);
            StringExpandPlaceholders(fmt, expanded, label);
            AddTextPrinterParameterizedWithColor(win, 0, expanded, 0, (s32)((u32)w->config->spacing * 16U * (u32)row), 255, 0xf0d00, 0);
            if (w->config->spacing == 2 && extra) {
                BufferString(fmt, 2, valueString, 0, 1, 2);
                StringExpandPlaceholders(fmt, expanded, extra);
                AddTextPrinterParameterizedWithColor(win, 0, expanded, 16, (s32)((u32)w->config->spacing * 16U * (u32)row + 16U), 255, 0xf0d00, 0);
            }
            row++;
            String_Delete(label);
            String_Delete(number);
            String_Delete(fieldString);
            String_Delete(expanded);
            String_Delete(valueString);
            MessageFormat_ResetBuffers(fmt);
        }
    }
    ScheduleWindowCopyToVram(win);
    MessageFormat_Delete(fmt);
    w->unknownC = w->unk8;
    return 0;
}
