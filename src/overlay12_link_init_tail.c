#include "global.h"

#include "heap.h"
#include "to46_overlay12_link_init_private.h"
#include "unk_0200FA24.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"

extern BOOL sub_02075074(void *, u32, ...);
extern BOOL sub_020750E0(void *);
extern BOOL sub_02075108(void *);
extern BOOL sub_0207514C(void *);
extern BOOL sub_02075178(void *);
extern BOOL sub_020751B8(void *);
extern BOOL sub_020751DC(void *);
extern BOOL sub_02075220(void *);
extern BOOL sub_02075248(void *);
extern BOOL sub_0207527C(void *);
extern BOOL sub_020752D8(void *);
extern BOOL sub_0207531C(void *, int);
extern BOOL sub_02075350(void *, int, int);
extern BOOL sub_020753A8(void *, int);
extern BOOL sub_020753D4(void *, int, int);
extern void sub_020399FC(int, BgConfig *);
extern void sub_02037930(int);
extern int Comm_CountConnected(void);
extern int Comm_GetLocalNetId(void);

BOOL ov12_02239C28(OverlayManager *man) {
    LinkInitWork *work = OverlayManager_GetData(man);
    BOOL done;
    int i;

    sub_020399FC(5, work->bg);
    done = FALSE;
    switch (work->state1) {
    case 0:
        SetMasterBrightnessNeutral(0);
        sub_02037930(1);
        work->state1++;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(work->palette) == 0) {
            work->state1++;
        }
        break;
    case 2:
        sub_02037AC0(0x32);
        work->state1++;
        break;
    case 3:
        if (sub_02037B38(0x32)) {
            sub_02037AC0(0x33);
            work->field1022 = 0;
            work->state1++;
        } else {
            work->field1022++;
            if (work->field1022 > 0x708) {
                sub_02039AD8(1);
            }
        }
        break;
    case 4:
        if (sub_02075074(work, 0x140) == TRUE) {
            work->state1++;
        }
        break;
    case 5:
        goto common;
    case 6:
        if (sub_020750E0(work) == TRUE) {
            sub_02037AC0(0x34);
            work->state1++;
        }
        break;
    case 7:
        if (sub_02075108(work) == TRUE) {
            work->state1++;
        }
        break;
    case 8:
        goto common;
    case 9:
        if (sub_0207514C(work) == TRUE) {
            sub_02037AC0(0x35);
            work->state1++;
        }
        break;
    case 10:
        if (sub_02075178(work) == TRUE) {
            work->state1++;
        }
        break;
    case 11:
        goto common;
    case 12:
        if (sub_020751B8(work) == TRUE) {
            sub_02037AC0(0x36);
            work->state1++;
        }
        break;
    case 13:
        if (sub_020751DC(work) == TRUE) {
            work->state1++;
        }
        break;
    case 14:
        goto common;
    case 15:
        if (sub_02075220(work) == TRUE) {
            sub_02037AC0(0x37);
            work->state1++;
        }
        break;
    case 16:
        if (sub_02075248(work) == TRUE) {
            work->state1++;
        }
        break;
    case 17:
        goto common;
    case 18:
        if (sub_0207527C(work) == TRUE) {
            sub_02037AC0(0x38);
            work->state1++;
        }
        break;
    case 19:
        if (sub_020752D8(work) == TRUE) {
            work->state1++;
        }
        break;
    case 20:
        goto common;
    case 21:
        if (!(work->setup->battleType & 0x80)) {
            work->state1 = 33;
        } else if (Comm_GetLocalNetId() != 0) {
            sub_02037AC0(0x39);
            work->state1++;
        } else if (sub_0207531C(work, 1) == TRUE) {
            sub_02037AC0(0x39);
            work->state1++;
        }
        break;
    case 22:
        work->state0 = 1;
        if (Comm_GetLocalNetId() != 0) {
            if (sub_02037B38(0x39) == TRUE) {
                work->state1++;
            }
        } else if (sub_02075350(work, 1, 0x39) == TRUE) {
            work->state1++;
        }
        break;
    case 23:
        goto common;
    case 24:
        if (Comm_GetLocalNetId() != 0) {
            sub_02037AC0(0x3A);
            work->state1++;
        } else if (sub_0207531C(work, 3) == TRUE) {
            sub_02037AC0(0x3A);
            work->state1++;
        }
        break;
    case 25:
        work->state0 = 1;
        if (Comm_GetLocalNetId() != 0) {
            if (sub_02037B38(0x3A) == TRUE) {
                work->state1++;
            }
        } else if (sub_02075350(work, 3, 0x3A) == TRUE) {
            work->state1++;
        }
        break;
    case 26:
        goto common;
    case 27:
        if (Comm_GetLocalNetId() != 0) {
            sub_02037AC0(0x3B);
            work->state1++;
        } else if (sub_020753A8(work, 1) == TRUE) {
            sub_02037AC0(0x3B);
            work->state1++;
        }
        break;
    case 28:
        work->state0 = 1;
        if (Comm_GetLocalNetId() != 0) {
            if (sub_02037B38(0x3B) == TRUE) {
                work->state1++;
            }
        } else if (sub_020753D4(work, 1, 0x3B) == TRUE) {
            work->state1++;
        }
        break;
    case 29:
        goto common;
    case 30:
        if (Comm_GetLocalNetId() != 0) {
            sub_02037AC0(0x3C);
            work->state1++;
        } else if (sub_020753A8(work, 3) == TRUE) {
            sub_02037AC0(0x3C);
            work->state1++;
        }
        break;
    case 31:
        work->state0 = 1;
        if (Comm_GetLocalNetId() != 0) {
            if (sub_02037B38(0x3C) == TRUE) {
                work->state1++;
            }
        } else if (sub_020753D4(work, 3, 0x3C) == TRUE) {
            work->state1++;
        }
        break;
    case 32:
common:
        if (work->state0 == Comm_CountConnected()) {
            if (work->state1 == 20) {
                for (i = 0; i < 4; i++) {
                    Heap_Free(((void **)work)[i + 4]);
                }
            }
            work->state0 = 0;
            work->field1022 = 0;
            work->state1++;
            if (work->state1 == 33) {
                PaletteData_BeginPaletteFade(work->palette, 5, 0xFFFF, 0, 0, 0x10, 0);
            }
        } else {
            work->field1022++;
            if (work->field1022 > 0x708) {
                sub_02039AD8(1);
            }
        }
        break;
    case 33:
        if (PaletteData_GetSelectedBuffersBitmask(work->palette) == 0) {
            done = TRUE;
            WaitingIcon_Delete(work->waiting);
            sub_02037930(0);
        }
        break;
    default:
        goto finish;
    }
finish:
    return done;
}
