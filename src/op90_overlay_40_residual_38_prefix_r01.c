#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"

typedef struct Ov40Src {
    u8 unk0[0x4B0];
    u64 value;
} Ov40Src;

typedef struct Ov40Unk {
    u8 unk0[0x48];
    MsgData *msgData;
    u8 unk4C[0x858];
    Window window;
} Ov40Unk;

MessageFormat *VsRecorder_NewMessageFormat(enum HeapID heapID);
void ov40_02242E4C(Ov40Src *src, Ov40Unk *data);

// @fn ov40_02242E4C
void ov40_02242E4C(Ov40Src *src, Ov40Unk *data) {
    MessageFormat *msgFmt = VsRecorder_NewMessageFormat(HEAP_ID_109);
    u64 value = src->value;
    u64 upper = value;
    String *low = String_New(255, HEAP_ID_109);
    String *mid = String_New(255, HEAP_ID_109);
    String *high = String_New(255, HEAP_ID_109);
    upper /= 100000;
    String *msg = NewString_ReadMsgData(data->msgData, 0x127);
    String *out = String_New(255, HEAP_ID_109);

    String16_FormatInteger(low, value % 100000, 5, PRINTING_MODE_LEADING_ZEROS, 1);
    String16_FormatInteger(mid, upper % 100000, 5, PRINTING_MODE_LEADING_ZEROS, 1);
    String16_FormatInteger(high, upper / 100000, 2, PRINTING_MODE_LEADING_ZEROS, 1);
    BufferString(msgFmt, 2, low, 0, 1, 2);
    BufferString(msgFmt, 1, mid, 0, 1, 2);
    BufferString(msgFmt, 0, high, 0, 1, 2);
    StringExpandPlaceholders(msgFmt, out, msg);
    FillWindowPixelBuffer(&data->window, 0xCC);
    AddTextPrinterParameterizedWithColor(&data->window, 0, out, 0, 0, 0xFF, 0xF0D00, NULL);
    ScheduleWindowCopyToVram(&data->window);
    String_Delete(low);
    String_Delete(mid);
    String_Delete(high);
    String_Delete(msg);
    String_Delete(out);
    MessageFormat_ResetBuffers(msgFmt);
    MessageFormat_Delete(msgFmt);
}
