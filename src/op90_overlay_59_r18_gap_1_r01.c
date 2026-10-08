#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "message_format.h"
#include "text.h"
typedef struct Ov59Work {
    u8 unk00[0x1A];
    u8 values[0x33];
    u8 selection;
    u8 unk4E[0x12];
    MessageFormat *msgFormat;
    String *string;
    u8 unk68[0xC];
    String *template;
    u8 unk78[0xB0];
    Window windows[10];
} Ov59Work;
extern u8 ov59_0223C624[];
void ov59_02238A04(Ov59Work *work, int value);

// @fn ov59_02238A04
void ov59_02238A04(Ov59Work *work, int value) {
    u8 digits[2];
    int i;
    digits[0] = value % 10;
    digits[1] = value / 10;
    for (i = 0; i < 2; i++) {
        BufferIntegerAsString(work->msgFormat, 0, digits[i], 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
        StringExpandPlaceholders(work->msgFormat, work->string, work->template);
        FillWindowPixelBuffer(&work->windows[i + 8], 0);
        AddTextPrinterParameterizedWithColor(&work->windows[i + 8], 0, work->string, ov59_0223C624[i], 3, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ScheduleWindowCopyToVram(&work->windows[i + 8]);
        if (work->values[work->selection] < 10) {
            break;
        }
    }
}
