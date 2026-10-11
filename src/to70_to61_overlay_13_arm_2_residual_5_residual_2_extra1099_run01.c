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
extern void ov13_0222B8E8(void *);
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

void ov13_022305B0(void) {
    int i;
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (!ov13_022272A0()) {
        return;
    }
    WifiSetupTask_Destroy(1, ov13_0224F538.work->task);
    for (i = 0; i < 5; i++) {
        if (ov13_0224F538.work->first[i]) {
            WifiSetupOam_Release(ov13_0224F538.work->first[i]);
        }
        if (ov13_0224F538.work->second[i]) {
            WifiSetupOam_Release(ov13_0224F538.work->second[i]);
        }
    }
    WifiSetupText_FreeCanvas(ov13_0224F538.work->buffer);
    ov13_02239514();
    WifiSetupCursor_Hide();
    WifiSetupMessage_Hide();
    ov13_02239288();
    WifiSetupFile_Free(ov13_0224F538.work->a);
    WifiSetupFile_Free(ov13_0224F538.work->b);
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 29);
    *(volatile u32 *)0x04000010 = 0;
    *(volatile u32 *)0x04000018 = 0;
    if (!ov13_0224F538.work->mode) {
        ov13_02239B34();
        ov13_0223A414(ov13_02239FF0()[244]);
        ov13_0222B8F8(2, 0);
        ov13_0222B8E8(ov13_02237618);
    } else if (ov13_0224F538.state == 4) {
        ov13_02239B34();
        ov13_0222B8F8(0, 1);
        ov13_0222B8E8(ov13_022363F4);
    } else {
        ov13_0223A02C(&ov13_0224F538.work->records[ov13_0224F538.work->index]);
        ov13_0222B8F8(0, 0);
        if (ov13_0224F538.work->records[ov13_0224F538.work->index].chars[40]) {
            ov13_0222B8F8(0, 1);
            ov13_0222B934(1, 1);
            ov13_0222B8E8(ov13_02230FD8);
        } else {
            ov13_0222B8F8(0, 1);
            ov13_0222B934(0, 1);
            ov13_0222B8E8(ov13_02238440);
        }
    }
    WifiSetupHeap_FreeAndClear(&ov13_0224F53C);
}
