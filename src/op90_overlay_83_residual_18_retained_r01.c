#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct {
    u8 pad[9];
    u8 unk9;
} Work;
extern void ov83_02240F7C(Work *, u16 *, u16 *, u16 *, u16 *);
extern BOOL ov80_02237D8C(u8);
extern BOOL Comm_GetLocalNetId(void);
extern void ov83_02240C8C(Work *, Window *, int, int, u8);
extern void ov83_02240CFC(Work *, Window *, int, int, int);
void ov83_0224153C(Work *w, Window *win);
// @fn ov83_0224153C
void ov83_0224153C(Work *w, Window *win) {
    u16 x1, y1, x2, y2;
    ov83_02240F7C(w, &x1, &y1, &x2, &y2);
    if (!ov80_02237D8C(w->unk9)) {
        ov83_02240C8C(w, win, (u16)(x1 + 4), (u16)(y1 + 1), 0);
    } else if (!Comm_GetLocalNetId()) {
        int y = y1; y = (u16)(y + 1);
        ov83_02240C8C(w, win, x1, y, 0);
        ov83_02240CFC(w, win, x2, (u16)(y2 + 1), 0);
    } else {
        int y = y1; y = (u16)(y + 1);
        ov83_02240CFC(w, win, x1, y, 0);
        ov83_02240C8C(w, win, x2, (u16)(y2 + 1), 0);
    }
    ScheduleWindowCopyToVram(win);
}
