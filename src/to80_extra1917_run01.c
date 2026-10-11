#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 chars[42];
} Record;

typedef struct {
    Record *records;
    void *a, *b, *buffer, *first[5], *second[5], *spare, *task;
    u8 p[17];
    u8 count, index, pad, mode;
} Work;

typedef struct {
    u8 state, p;
    u16 position;
    Work *work;
} Global;

extern Global ov13_0224F538;
extern void *ov13_0224F53C;
extern int FX_DivS32(int, int);
extern int WifiSetupFade_IsActive(int);
extern int ov13_022272A0(void);
extern int WifiSetup_StrNLen(void *, int);
extern void WifiSetupTask_Destroy(int, void *);
extern void WifiSetupOam_Release(void *);
extern void WifiSetupText_FreeCanvas(void *);
extern void ov13_02239514(void);
extern void WifiSetupCursor_Hide(void);
extern void WifiSetupMessage_Hide(void);
extern void ov13_02239288(void);
extern void WifiSetupFile_Free(void *);
extern void WifiSetupDisplay_HidePlanes(int, int);
extern void ov13_02239B34(void);
extern void ov13_0223A414(int);
extern void ov13_0222B8F8(int, int);
extern void WifiSetupApp_SetStateFunc(void *);
extern void ov13_0223A02C(void *);
extern void ov13_0222B934(int, int);
extern void WifiSetupHeap_FreeAndClear(void *);
extern void WifiSetupText_Clear(void *, int);
extern void WifiSetupText_MarkDirty(void *);
extern void ov13_02230A58(void);
extern void ov13_022309CC(int, int);
extern void WifiSetupText_DrawMonospace(void *, int, int, int, int, u16 *, int);
extern void ov13_02237618(void);
extern void ov13_022363F4(void);
extern void ov13_02230FD8(void);
extern void ov13_02238440(void);
extern u8 *ov13_02239FF0(void);
void ov13_022305B0(void), ov13_022307AC(void), ov13_02230854(int, int);
#pragma force_active on

void ov13_022307AC(void) {
    int start = FX_DivS32(ov13_0224F538.position, 28), count = ov13_0224F538.work->count, row, i, second;
    WifiSetupText_Clear(ov13_0224F538.work->buffer, 0);
    if (count > 5) {
        count = 5;
    }
    row = start;
    i = 0;
    if (count > 0) {
        do {
            ov13_02230854(row, i);
            ++i;
            ++row;
        } while (i < count);
    }
    second = start;
    row = 0;
    if (count > 0) {
        do {
            ov13_022309CC(second, row);
            ++row;
            ++second;
        } while (row < count);
    }
    WifiSetupText_MarkDirty(ov13_0224F538.work->buffer);
    ov13_02230A58();
}
