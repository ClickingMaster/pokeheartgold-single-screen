#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "string_util.h"
#include "text.h"
#include "touchscreen.h"

typedef struct Ov40ListItem {
    u32 msgId;
    u32 value;
    u8 unk8[8];
} Ov40ListItem;

typedef struct Ov40ListTemplate {
    Ov40ListItem *items;
    u8 unk4[4];
    int lineHeight;
    u8 unkC[0x1C];
    TouchscreenHitbox *hitboxes;
} Ov40ListTemplate;

typedef struct Ov40List {
    u8 unk0[4];
    int count;
    s16 cursor;
    u8 unkA[2];
    int prevCursor;
    int numVisible;
    u8 unk14[4];
    Window window;
    Ov40ListTemplate *template;
    u8 unk2C[8];
    MsgData *msgData;
} Ov40List;

void ov40_02230944(void *a0);
u32 ov40_0222F38C(Ov40List *list, void *a1);

// @fn ov40_0222F38C
u32 ov40_0222F38C(Ov40List *list, void *a1) {
    u32 result = 0;
    Window *window;
    int i;
    int start, end;
    String *string;
    int j;

    if (list->template->hitboxes != NULL) {
        for (i = 0; i < list->numVisible; i++) {
            if (TouchscreenHitbox_TouchNewIsIn(&list->template->hitboxes[i])) {
                ov40_02230944(a1);
                result = list->template->items[list->cursor + i].value;
            }
        }
    }
    if (list->cursor == list->prevCursor) {
        return result;
    }
    if (list->numVisible == list->count) {
        return result;
    }
    window = &list->window;
    FillWindowPixelBuffer(window, 0);
    start = list->cursor;
    j = 0;
    end = start + list->numVisible;
    if (end >= list->count) {
        start = list->count - list->numVisible;
        end = list->count;
    }
    for (i = start; i < end; i++) {
        string = NewString_ReadMsgData(list->msgData, list->template->items[i].msgId);
        AddTextPrinterParameterizedWithColor(window, 0, string, 0, j * (list->template->lineHeight * 16), 0xFF, 0xF0D00, NULL);
        String_Delete(string);
        j++;
    }
    CopyWindowToVram(window);
    list->prevCursor = list->cursor;
    return result;
}
