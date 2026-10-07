#pragma force_active on
#include "global.h"
#include "bg_window.h"

typedef struct Ov83Work {
    u8 pad0[0x50];
    Window windows[35];
    u8 pad[0x544 - 0x50 - 35 * 0x10];
    void *cursor;
} Ov83Work;

void ov83_0224755C(void *cursor, int draw);
void ov83_02246938(Ov83Work *work, int show);

// @fn ov83_02246938
void ov83_02246938(Ov83Work *work, int show) {
    u32 i;

    if (show == TRUE) {
        for (i = 12; i <= 34; i++) {
            ScheduleWindowCopyToVram(&work->windows[i]);
        }
        ov83_0224755C(work->cursor, TRUE);
    } else {
        for (i = 12; i <= 34; i++) {
            ClearWindowTilemapAndScheduleTransfer(&work->windows[i]);
        }
        ov83_0224755C(work->cursor, FALSE);
    }
}
