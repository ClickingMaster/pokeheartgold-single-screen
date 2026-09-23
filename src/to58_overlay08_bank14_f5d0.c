#include "../include/to57_overlay08_bank14_private.h"

extern void ov08_0221F658(void *, int);
typedef struct Work { u8 pad[0x2070]; Window *windows; } Work;
typedef struct Entry { u8 pad[0x1b]; u8 flag; u8 rest[0x50 - 0x1c]; } Entry;

void ov08_0221F5D0(void *work) {
    FillWindowPixelBuffer(((Work *)work)->windows, 0);
    FillWindowPixelBuffer(((Work *)work)->windows + 1, 0);
    FillWindowPixelBuffer(((Work *)work)->windows + 2, 0);
    FillWindowPixelBuffer(((Work *)work)->windows + 3, 0);
    ov08_0221F658(work, *((u8 *)*(void **)work + 0x11));
    ov08_0221E3A4(work, 1, 15);
    if (!(((u32)((Entry *)work)[*((u8 *)*(void **)work + 0x11)].flag << 24) >> 31)) {
        ov08_0221E3A4(work, 2, 18);
        ov08_0221E3A4(work, 3, 19);
        return;
    }
    ScheduleWindowCopyToVram(((Work *)work)->windows + 2);
    ScheduleWindowCopyToVram(((Work *)work)->windows + 3);
}
