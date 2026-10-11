#include "global.h"

#include "application/vs_recorder/vs_recorder_internal.h"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_text.h"
#include "sound_playback.h"
#include "text.h"
#include "touchscreen.h"

// VS Recorder list windows: message lists, ranking lists, two-column lists and name lists, with cursor / scroll.

typedef struct VsRecorderRankLimits {
    int v[3];
} VsRecorderRankLimits;

// GetSpeciesNameIntoArray called through an int prototype, as compiled (no u16 truncation of the species)
#define GetSpeciesNameIntoArrayInt ((void (*)(int, enum HeapID, u16 *))GetSpeciesNameIntoArray)

extern const VsRecorderRankLimits ov40_02244E10;
extern const VsRecorderRankLimits ov40_02244E1C;

// "country / region" of the shown trainer in the list's second row
void VsRecorder_PrintListLocation(VsRecorderListWindow *list, VsRecorderWork *work) {
    MessageFormat *fmt;
    String *template;
    String *str;
    u32 country = FrontierCard_GetCountry(work->videoOwners[0]);
    u32 city = FrontierCard_GetRegion(work->videoOwners[0]);
    Window *window = &list->window;

    if (country == 0 && city == 0) {
        str = NewString_ReadMsgData(work->msgData, 0x7D);
    } else {
        fmt = VsRecorder_NewMessageFormat(HEAP_ID_109);
        if (city != 0) {
            str = String_New(0xFF, HEAP_ID_109);
            template = NewString_ReadMsgData(work->msgData, 0x17);
            BufferCityName(fmt, 0, country, city);
            StringExpandPlaceholders(fmt, str, template);
        } else {
            str = String_New(0xFF, HEAP_ID_109);
            template = NewString_ReadMsgData(work->msgData, 0x16);
            BufferCountryName(fmt, 0, country);
            StringExpandPlaceholders(fmt, str, template);
        }
        String_Delete(template);
        MessageFormat_Delete(fmt);
    }

    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0x10, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
}

// one message per entry; entry msg 0x10 is followed by the trainer's location
void VsRecorder_InitMessageList(VsRecorderListWindow *list, VsRecorderWork *work, VsRecorderListConfig *config) {
    Window *window = &list->window;
    int i;
    int count;
    list->count = config->count;
    list->state = 0;
    list->drawnPos = 0;
    list->rows = config->rows;
    list->config = config;
    list->msgData = work->msgData;
    list->pages = list->count / list->rows + 1;
    count = list->count;
    if (count < list->rows) {
        list->rows = count;
        list->topMargin = count - 1;
        list->bottomMargin = list->rows - 1;
    }
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, (u8)config->bgId, (u8)config->x, (u8)config->y, (u8)config->width, (u8)config->height, 14, (u16)config->baseTile);
    FillWindowPixelBuffer(window, 0);
    for (i = 0; i < list->rows; i++) {
        String *str = NewString_ReadMsgData(list->msgData, config->entries[i].msgId);
        AddTextPrinterParameterizedWithColor(window, 0, str, 0, config->lineHeight * 16 * i, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        String_Delete(str);
        if (config->entries[i].msgId == 0x10) {
            VsRecorder_PrintListLocation(list, work);
        }
    }
    ScheduleWindowCopyToVram(window);
}

// message list (msgData, or the app's msg_0013 if NULL); without entries: numbered names of work->net.videoList.headers
void VsRecorder_InitList(VsRecorderListWindow *list, VsRecorderWork *work, MsgData *msgData, VsRecorderListConfig *config) {
    Window *window = &list->window;
    int i;
    int count;

    list->count = config->count;
    list->state = 0;
    list->drawnPos = 0;
    list->rows = config->rows;
    list->config = config;
    if (msgData != NULL) {
        list->msgData = msgData;
    } else {
        list->msgData = work->msgData;
    }
    list->pages = list->count / list->rows + 1;
    list->topMargin = list->rows / 2;
    list->bottomMargin = list->rows / 2;
    count = list->count;
    if (count < list->rows) {
        list->rows = count;
        list->topMargin = count - 1;
        list->bottomMargin = list->rows - 1;
    }

    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, (u8)config->bgId, (u8)config->x, (u8)config->y, (u8)config->width, (u8)config->height, 14, (u16)config->baseTile);
    FillWindowPixelBuffer(window, 0);

    if (config->entries != NULL) {
        for (i = 0; i < list->rows; i++) {
            String *str = NewString_ReadMsgData(list->msgData, config->entries[i].msgId);
            AddTextPrinterParameterizedWithColor(window, 0, str, 0, config->lineHeight * 16 * i, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            String_Delete(str);
        }
    } else {
        i = 0;
        if (list->rows > 0) {
            VsRecorderWork *cursor = work;
            int y = 4;
            do {
                String *name = String_New(0xFF, HEAP_ID_109);
                String *number = String_New(0xFF, HEAP_ID_109);
                String *template = NewString_ReadMsgData(work->msgData, 0x63);
                u32 width;

                String16_FormatInteger(number, i + 1, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
                CopyU16ArrayToString(name, (const u16 *)cursor->net.videoList.headers[0]);
                VsRecorder_SanitizeName(work, name);
                width = FontID_String_GetWidth(0, number, 0);
                AddTextPrinterParameterizedWithColor(window, 0, number, 16 - width, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
                AddTextPrinterParameterizedWithColor(window, 0, template, 16, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
                AddTextPrinterParameterizedWithColor(window, 0, name, 22, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
                String_Delete(name);
                String_Delete(number);
                String_Delete(template);
                cursor = (VsRecorderWork *)((u8 *)cursor + sizeof(GlobalTerminalNetVideoHeader *));
                y += 0x18;
                i++;
            } while (i < list->rows);
        }
    }
    ScheduleWindowCopyToVram(window);
}

#pragma force_active on

// ranking list (type 0 message value, 1 month, 2 species); equal times share a rank
void VsRecorder_InitRankingList(VsRecorderListWindow *list, VsRecorderWork *work, MsgData *msgData, VsRecorderListConfig *config, int unused, int type, String *valueTemplate) {
    Window *win;
    String *label;
    String *number;
    String *fieldString;
    int lastGroup = 0;
    u64 previous;
    String *valueString;
    int month;
    int species;
    u64 value;
    int *grp;
    int msgId;
    int i;
    MessageFormat *fmt;
    String *expanded;
    int groups[30] = { 0 };
    u16 speciesName[256];
    VsRecorderRankLimits limits;

    win = &list->window;
    list->count = config->count;
    list->state = 0;
    list->drawnPos = 0;
    list->rows = config->rows;
    list->config = config;
    if (msgData) {
        list->msgData = msgData;
    } else {
        list->msgData = work->msgData;
    }
    list->pages = list->count / list->rows + 1;
    {
        int count = list->count;
        if (count < list->rows) {
            list->rows = count;
            list->topMargin = count - 1;
            list->bottomMargin = list->rows - 1;
        }
    }
    list->topMargin = list->rows / 2;
    list->bottomMargin = list->rows / 2;
    InitWindow(win);
    AddWindowParameterized(work->bgConfig, win, (u8)config->bgId, (u8)config->x, (u8)config->y, (u8)config->width, (u8)config->height, 14, (u16)config->baseTile);
    FillWindowPixelBuffer(win, 0);
    fmt = VsRecorder_NewMessageFormat(HEAP_ID_109);
    limits = ov40_02244E10;
    previous = list->config->entries[0].time;
    groups[0] = 0;
    {
        for (i = 1; i < limits.v[type]; i++) {
            u64 current = list->config->entries[i].time;
            if (current == previous) {
                groups[i] = lastGroup;
            } else {
                previous = current;
                groups[i] = i;
                lastGroup = i;
            }
        }
    }
    i = 0;
    if (i < list->rows) {
        grp = groups;
        msgId = type + 88;
        do {
            label = NewString_ReadMsgData(work->msgData, msgId);
            number = String_New(255, HEAP_ID_109);
            expanded = String_New(255, HEAP_ID_109);
            valueString = String_New(255, HEAP_ID_109);
            switch (type) {
            case 0:
                fieldString = NewString_ReadMsgData(list->msgData, list->config->entries[i].value);
                BufferString(fmt, 1, fieldString, 0, 1, 2);
                break;
            case 1:
                month = list->config->entries[i].value;
                fieldString = String_New(255, HEAP_ID_109);
                BufferMonthNameAbbr(fmt, 1, month);
                break;
            case 2:
                species = list->config->entries[i].value;
                fieldString = String_New(255, HEAP_ID_109);
                GetSpeciesNameIntoArrayInt(species, HEAP_ID_109, speciesName);
                CopyU16ArrayToString(fieldString, speciesName);
                BufferString(fmt, 1, fieldString, 0, 1, 2);
                break;
            }
            value = list->config->entries[i].time;
            String16_FormatUnsignedLongLong(valueString, value, VsRecorder_CountDigits(value), PRINTING_MODE_LEFT_ALIGN, TRUE);
            String16_FormatInteger(number, *grp + 1, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            BufferString(fmt, 0, number, 0, 1, 2);
            StringExpandPlaceholders(fmt, expanded, label);
            AddTextPrinterParameterizedWithColor(win, 0, expanded, 0, (s32)((u32)list->config->lineHeight * 16U * (u32)i), TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            if (list->config->lineHeight == 2 && valueTemplate) {
                BufferString(fmt, 2, valueString, 0, 1, 2);
                StringExpandPlaceholders(fmt, expanded, valueTemplate);
                AddTextPrinterParameterizedWithColor(win, 0, expanded, 16, (s32)((u32)list->config->lineHeight * 16U * (u32)i + 16U), TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            }
            String_Delete(label);
            String_Delete(number);
            String_Delete(fieldString);
            String_Delete(expanded);
            String_Delete(valueString);
            MessageFormat_ResetBuffers(fmt);
            grp++;
        } while (++i < list->rows);
    }
    ScheduleWindowCopyToVram(win);
    MessageFormat_Delete(fmt);
}

#pragma force_active reset

// two columns of prepared strings (items[2 * row], items[2 * row + 1])
void VsRecorder_InitPairList(VsRecorderListWindow *list, VsRecorderWork *work, VsRecorderListConfig *config, String **items) {
    Window *window = &list->window;
    int i;
    int count;
    list->count = config->count;
    list->state = 0;
    list->drawnPos = 0;
    list->rows = config->rows;
    list->config = config;
    list->msgData = work->msgData;
    list->pages = list->count / list->rows + 1;
    list->topMargin = list->rows / 2;
    list->bottomMargin = list->rows / 2;
    count = list->count;
    if (count < list->rows) {
        list->rows = count;
        list->topMargin = count - 1;
        list->bottomMargin = list->rows - 1;
    }
    list->unk_48 = 1;
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, (u8)config->bgId, (u8)config->x, (u8)config->y, (u8)config->width, (u8)config->height, 14, (u16)config->baseTile);
    FillWindowPixelBuffer(window, 0);
    for (i = 0; i < list->rows; i++, items += 2) {
        AddTextPrinterParameterizedWithColor(window, 0, items[0], 0, config->lineHeight * 8 * i + 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        AddTextPrinterParameterizedWithColor(window, 0, items[1], 0x88, config->lineHeight * 8 * i + 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    }
    ScheduleWindowCopyToVram(window);
}

int VsRecorder_RedrawPairList(VsRecorderListWindow *list, VsRecorderWork *work, String **items) {
    int start;
    int i;
    Window *window;

    (void)work;
    if (list->cursor == list->drawnPos) {
        return 0;
    }
    if (list->rows == list->count) {
        return 0;
    }

    window = &list->window;
    FillWindowPixelBuffer(window, 0);
    {
        int edge = list->topMargin;
        int display = list->cursorRow;

        start = 0;
        if (display >= edge) {
            int selected = list->cursor;
            int count = list->count;

            start = selected - edge;
            if (selected >= count - (edge + 1)) {
                start = count - list->rows;
            }
        }
    }
    i = 0;
    if (list->rows > 0) {
        items += start * 2;
        do {
            if (items[0] != NULL) {
                AddTextPrinterParameterizedWithColor(window, 0, items[0], 0, list->config->lineHeight * 8 * i + 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
                AddTextPrinterParameterizedWithColor(window, 0, items[1], 0x88, list->config->lineHeight * 8 * i + 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            }
            i++;
            items += 2;
        } while (i < list->rows);
    }
    CopyWindowToVram(window);
    list->drawnPos = list->cursor;
    return 0;
}

#pragma force_active on

int VsRecorder_RedrawRankingList(VsRecorderListWindow *list, VsRecorderWork *work, void *unused, u32 type, String *valueTemplate) {
    Window *win;
    String *label;
    String *number;
    String *fieldString;
    String *valueString;
    int row;
    int lastGroup;
    u64 previous;
    int start;
    int end;
    int month;
    int species;
    u64 value;
    int i;
    MessageFormat *fmt;
    String *expanded;

    if (list->scroll == list->drawnPos) {
        return 0;
    }
    if (list->rows == list->count) {
        return 0;
    }
    win = &list->window;
    fmt = VsRecorder_NewMessageFormat(HEAP_ID_109);
    FillWindowPixelBuffer(win, 0);
    row = 0;
    lastGroup = 0;
    start = list->scroll;
    end = start + list->rows;
    {
        int groups[30] = { 0 };
        u16 speciesName[256];
        VsRecorderRankLimits limits;

        if (end >= list->count) {
            end = list->count;
            start = list->count - list->rows;
        }
        limits = ov40_02244E1C;
        previous = list->config->entries[0].time;
        groups[0] = 0;
        for (i = 1; i < limits.v[type]; i++) {
            u64 current = list->config->entries[i].time;
            if (current == previous) {
                groups[i] = lastGroup;
            } else {
                previous = current;
                groups[i] = i;
                lastGroup = i;
            }
        }
        for (i = start; i < end; i++) {
            label = NewString_ReadMsgData(work->msgData, type + 88);
            number = String_New(255, HEAP_ID_109);
            expanded = String_New(255, HEAP_ID_109);
            valueString = String_New(255, HEAP_ID_109);
            switch (type) {
            case 0:
                fieldString = NewString_ReadMsgData(list->msgData, list->config->entries[i].value);
                BufferString(fmt, 1, fieldString, 0, 1, 2);
                break;
            case 1:
                month = list->config->entries[i].value;
                fieldString = String_New(255, HEAP_ID_109);
                BufferMonthNameAbbr(fmt, 1, month);
                break;
            case 2:
                species = list->config->entries[i].value;
                fieldString = String_New(255, HEAP_ID_109);
                GetSpeciesNameIntoArrayInt(species, HEAP_ID_109, speciesName);
                CopyU16ArrayToString(fieldString, speciesName);
                BufferString(fmt, 1, fieldString, 0, 1, 2);
                break;
            }
            value = list->config->entries[i].time;
            String16_FormatUnsignedLongLong(valueString, value, VsRecorder_CountDigits(value), PRINTING_MODE_LEFT_ALIGN, TRUE);
            String16_FormatInteger(number, groups[i] + 1, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            BufferString(fmt, 0, number, 0, 1, 2);
            StringExpandPlaceholders(fmt, expanded, label);
            AddTextPrinterParameterizedWithColor(win, 0, expanded, 0, (s32)((u32)list->config->lineHeight * 16U * (u32)row), TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            if (list->config->lineHeight == 2 && valueTemplate) {
                BufferString(fmt, 2, valueString, 0, 1, 2);
                StringExpandPlaceholders(fmt, expanded, valueTemplate);
                AddTextPrinterParameterizedWithColor(win, 0, expanded, 16, (s32)((u32)list->config->lineHeight * 16U * (u32)row + 16U), TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
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
    list->drawnPos = list->scroll;
    return 0;
}

// touch on a row returns its entry value; redraws when scrolled
u32 VsRecorder_UpdateMessageList(VsRecorderListWindow *list, VsRecorderWork *work) {
    u32 result = 0;
    Window *window;
    int i;
    int start, end;
    String *string;
    int j;

    if (list->config->hitboxes != NULL) {
        for (i = 0; i < list->rows; i++) {
            if (TouchscreenHitbox_TouchNewIsIn(&list->config->hitboxes[i])) {
                VsRecorder_OnTouchConfirm(work);
                result = list->config->entries[list->scroll + i].value;
            }
        }
    }
    if (list->scroll == list->drawnPos) {
        return result;
    }
    if (list->rows == list->count) {
        return result;
    }
    window = &list->window;
    FillWindowPixelBuffer(window, 0);
    start = list->scroll;
    j = 0;
    end = start + list->rows;
    if (end >= list->count) {
        start = list->count - list->rows;
        end = list->count;
    }
    for (i = start; i < end; i++) {
        string = NewString_ReadMsgData(list->msgData, list->config->entries[i].msgId);
        AddTextPrinterParameterizedWithColor(window, 0, string, 0, j * (list->config->lineHeight * 16), TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        String_Delete(string);
        j++;
    }
    CopyWindowToVram(window);
    list->drawnPos = list->scroll;
    return result;
}

#pragma force_active reset

int VsRecorder_RedrawNameList(VsRecorderListWindow *list, VsRecorderWork *work) {
    int i;
    int start;
    Window *window;

    if (list->cursor == list->drawnPos) {
        return 0;
    }
    if (list->rows == list->count) {
        return 0;
    }

    window = &list->window;
    FillWindowPixelBuffer(window, 0);
    start = 0;
    if (list->cursorRow >= list->topMargin) {
        start = list->cursor - list->topMargin;
        if (list->cursor >= list->count - (list->topMargin + 1)) {
            start = list->count - list->rows;
        }
    }
    i = 0;
    if (list->rows > 0) {
        VsRecorderWork *item = (VsRecorderWork *)((GlobalTerminalNetVideoHeader **)work + start); // headers stride over the work
        int y = 4;
        do {
            String *nameStr = String_New(0xFF, HEAP_ID_109);
            String *number = String_New(0xFF, HEAP_ID_109);
            String *message = NewString_ReadMsgData(work->msgData, 0x63);
            int width;
            String16_FormatInteger(number, start + i + 1, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            CopyU16ArrayToString(nameStr, (const u16 *)item->net.videoList.headers[0]);
            VsRecorder_SanitizeName(work, nameStr);
            width = FontID_String_GetWidth(0, number, 0);
            AddTextPrinterParameterizedWithColor(window, 0, number, 0x10 - width, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            AddTextPrinterParameterizedWithColor(window, 0, message, 0x10, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            AddTextPrinterParameterizedWithColor(window, 0, nameStr, 0x16, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            String_Delete(nameStr);
            String_Delete(number);
            String_Delete(message);
            item = (VsRecorderWork *)((GlobalTerminalNetVideoHeader **)item + 1);
            i++;
            y += 0x18;
        } while (i < list->rows);
    }
    CopyWindowToVram(window);
    list->drawnPos = list->cursor;
    return 0;
}

// move the cursor one row (delta < 0 up, > 0 down) and keep it inside the visible window
void VsRecorder_ListMoveCursor(VsRecorderListWindow *list, s16 delta) {
    s16 cursorRow = 0;
    int direction = 0xFF;

    if (delta < 0) {
        if (list->cursor > 0) {
            list->cursor--;
            direction = 0;
            PlaySE(SEQ_SE_PL_BUTTON);
        }
    } else if (delta > 0) {
        if (list->cursor < list->count - 1) {
            list->cursor++;
            direction = 1;
            PlaySE(SEQ_SE_PL_BUTTON);
        }
    }

    switch (direction) {
    case 0: {
        int start;
        if (list->cursor < list->bottomMargin) {
            cursorRow = list->cursor;
        } else {
            start = list->count - list->bottomMargin;
            if (list->cursor >= start) {
                cursorRow = list->bottomMargin + 1;
                if (list->cursor >= start) {
                    cursorRow += list->cursor - start;
                }
            } else {
                cursorRow = list->bottomMargin;
                if (list->cursor <= list->bottomMargin) {
                    cursorRow += list->cursor - list->bottomMargin;
                }
            }
        }
        break;
    }
    case 1:
        if (list->cursor <= list->topMargin) {
            cursorRow = list->cursor;
        } else {
            cursorRow = list->topMargin;
            if (list->cursor >= list->count - list->topMargin) {
                cursorRow += list->cursor - (list->count - list->topMargin - 1);
            }
        }
        break;
    }

    if (cursorRow < 0) {
        GF_AssertFail();
    }
    if (direction != 0xFF) {
        list->cursorRow = cursorRow;
    }

    {
        s16 scroll = 0;
        if (list->cursorRow >= list->topMargin) {
            scroll = list->cursor - list->topMargin;
            if (list->cursor >= list->count - (list->topMargin + 1)) {
                scroll = list->count - list->rows;
            }
        }
        list->scroll = scroll;
    }
    VsRecorder_UpdateListArrowsCursor(list);
}

void VsRecorder_ListScroll(VsRecorderListWindow *list, s16 delta) {
    s16 old = list->scroll;
    list->scroll += delta;
    if (list->scroll < 0) {
        list->scroll = 0;
    }
    if (list->rows == list->count) {
        list->scroll = 0;
    }
    if (list->scroll + list->rows > list->count) {
        list->scroll = list->count - list->rows;
    }
    if (old != list->scroll) {
        PlaySE(SEQ_SE_PL_BUTTON);
    }
    VsRecorder_UpdateListArrows(list);
}
