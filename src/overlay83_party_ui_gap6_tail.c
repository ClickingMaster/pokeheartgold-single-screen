#include "overlay83_party_ui_gap6_internal.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
void ov83_02240238(u8 *work, void *window, int index, int value) {
    int x = ov80_02237D8C(work[9]) == 0 ? 0x24 : 4;
    x += index << 6;
    FillWindowPixelRect(window, 0, (u16)x, 0, 0x18, 9);
    PrintUIntOnWindow(PTR(work, 0x504), value, 3, 1, window, x, 1);
    ScheduleWindowCopyToVram(window);
}
void ov83_02240290(u8 *work) {
    work[0xa] = ov83_0223FC48(work, work + 0x60, 6, 0, 5, 0xff, 1, 2, 0, 0);
    ov83_02247944(work + 0xc0, Options_GetFrame(PTR(work, 0x508)));
    work[0xa] = ov83_0223FC48(work, work + 0xc0, 5, 1, 1, 0xff, 1, 2, 0xf, 1);
}
