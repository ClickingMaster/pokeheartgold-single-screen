#include "global.h"

extern int ov08_0221D5DC(u8 *work, int index);
extern void ov08_02221E6C(u8 *work, int index, int mode, int flag);
extern void ov08_02221EE8(u8 *work, int index, int mode);
extern void ov08_02221F98(u8 *work, int index, int mode);
extern u8 ov08_02222564(void *work);
void ov08_022220FC(u8 *work);
void ov08_022221CC(u8 *work, unsigned int state);

typedef struct B20State {
    u8 pad[0x1FA0];
    u8 state;
    u8 pad1;
    u8 index;
    u8 low:4;
    u8 mode:3;
    u8 active:1;
} B20State;
void ov08_022220FC(u8 *work) {
    B20State *view = (B20State *)work;
    if (view->active) {
        switch (view->state) {
        case 0:
            ov08_02221E6C(work, view->index, 1, view->mode);
            ov08_02221EE8(work, view->index, 1);
            ov08_02221F98(work, view->index, 1);
            work[0x1FA1] = 0;
            work[0x1FA0] = 1;
            break;
        case 1:
            ov08_02221E6C(work, view->index, 2, view->mode);
            ov08_02221EE8(work, view->index, 2);
            ov08_02221F98(work, view->index, 2);
            work[0x1FA1] = 0;
            work[0x1FA0] = 2;
            break;
        case 2:
            ov08_02221E6C(work, view->index, 0, view->mode);
            ov08_02221EE8(work, view->index, 0);
            ov08_02221F98(work, view->index, 0);
            work[0x1FA1] = 0;
            view->active = 0;
            break;
        }
    }
}

typedef struct B20Entry {
    u8 pad[0x1B];
    u8 flags:7;
    u8 disabled:1;
    u8 pad1[0x50 - 0x1C];
} B20Entry;
typedef struct B20Tail {
    u8 pad[0x2077];
    u8 low:4;
    u8 high:4;
} B20Tail;
void ov08_022221CC(u8 *work, unsigned int state) {
    u16 i;
    switch (state) {
    case 0:
        for (i = 0; i < 6; i++) {
            int value = ov08_0221D5DC(work, i);
            if (value == 0) {
                ov08_02221E6C(work, (u8)i, 3, 1);
            } else if (value == 1) {
                ov08_02221E6C(work, (u8)i, 0, 0);
            } else if (value == 2) {
                ov08_02221E6C(work, (u8)i, 0, 1);
            }
        }
        if ((*(u8 **)work)[0x35] == 1) {
            ov08_02221E6C(work, 6, 3, 0);
        } else {
            ov08_02221E6C(work, 6, 0, 0);
        }
        break;
    case 1:
        ov08_02221E6C(work, 6, 0, 0);
        ov08_02221E6C(work, 7, 0, 0);
        if (((B20Entry *)work)[(*(u8 **)work)[0x11]].disabled) {
            ov08_02221E6C(work, 8, 3, 0);
            ov08_02221E6C(work, 10, 3, 0);
        } else {
            ov08_02221E6C(work, 8, 0, 0);
            ov08_02221E6C(work, 10, 0, 0);
        }
        break;
    case 2:
        if (ov08_02222564(work) == 1) {
            ov08_02221E6C(work, 12, 0, 0);
            ov08_02221E6C(work, 13, 0, 0);
        } else {
            ov08_02221E6C(work, 12, 3, 0);
            ov08_02221E6C(work, 13, 3, 0);
        }
        ov08_02221E6C(work, 11, 0, 0);
        ov08_02221E6C(work, 6, 0, 0);
        break;
    case 3:
        if (ov08_02222564(work) == 1) {
            ov08_02221E6C(work, 12, 0, 0);
            ov08_02221E6C(work, 13, 0, 0);
        } else {
            ov08_02221E6C(work, 12, 3, 0);
            ov08_02221E6C(work, 13, 3, 0);
        }
        for (i = 0; i < 4; i++) {
            if (*(u16 *)(work + (*(u8 **)work)[0x11] * 0x50 + i * 8 + 0x34) != 0) {
                ov08_02221E6C(work, (u8)(i + 14), 0, 0);
            } else {
                ov08_02221E6C(work, (u8)(i + 14), 3, 0);
            }
        }
        ov08_02221E6C(work, 9, 0, 0);
        ov08_02221E6C(work, 6, 0, 0);
        break;
    case 4:
        ov08_02221E6C(work, 6, 0, 0);
        for (i = 0; i < 4; i++) {
            if ((*(u8 **)work)[0x34] == i) {
                ov08_02221E6C(work, (u8)(i + 30), 2, 0);
            } else {
                ov08_02221E6C(work, (u8)(i + 30), 0, 0);
            }
        }
        break;
    case 5:
        for (i = 0; i < 4; i++) {
            if (*(u16 *)(work + (*(u8 **)work)[0x11] * 0x50 + i * 8 + 0x34) != 0) {
                ov08_02221E6C(work, (u8)(i + 19), 0, 0);
            } else {
                ov08_02221E6C(work, (u8)(i + 19), 3, 0);
            }
        }
        ov08_02221E6C(work, 6, 0, 0);
        break;
    case 6:
    case 8:
        ov08_02221E6C(work, 23, 0, 0);
        ov08_02221E6C(work, 24, 0, 0);
        ov08_02221E6C(work, 25, 0, 0);
        ov08_02221E6C(work, 26, 0, 0);
        ov08_02221E6C(work, 27, 0, 0);
        ov08_02221E6C(work, 6, 0, 0);
        if (((B20Tail *)work)->high == 1) {
            ov08_02221E6C(work, 18, 0, 0);
        }
        break;
    case 7:
        ov08_02221E6C(work, 28, 0, 0);
        ov08_02221E6C(work, 6, 0, 0);
        if (((B20Tail *)work)->high == 1) {
            ov08_02221E6C(work, 18, 0, 0);
        }
        break;
    case 9:
        ov08_02221E6C(work, 29, 0, 0);
        ov08_02221E6C(work, 6, 0, 0);
        if (((B20Tail *)work)->high == 1) {
            ov08_02221E6C(work, 18, 0, 0);
        }
        break;
    }
}
