#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Parent {
    char pad[0x508];
    void *callback;
    u8 flag;
    char gap[0x17];
    u16 mode, error, ready, pad52a, delay;
} Parent;

extern struct {
    char pad[8];
    Parent *parent;
} ov13_0224CF40;

extern void MBi_CheckWmErrcode(int, int);
extern void MBi_ChildCallback(void);
extern int WM_SetMPDataToPortEx(void *, int, void *, u16, u16, int, int);
int MBi_SetMPData(void *, void *, u16, u16, u16);
int MBi_SendMP(void *, u32, u32);

int MBi_SetMPData(void *callback, void *data, u16 size, u16 unused, u16 mask) {
    int result = WM_SetMPDataToPortEx(callback, 0, data, size, mask, 1, 3);
    MBi_CheckWmErrcode(15, result);
    return result;
}

int MBi_SendMP(void *data, u32 size, u32 mask) {
    Parent *p = ov13_0224CF40.parent;
    u16 length = size, bitmap = mask;
    int result;
    if (!p->ready || p->error == 1) {
        return 1;
    }
    switch (p->mode) {
    case 1:
        result = MBi_SetMPData(p->callback, data, length, (u16)(p->delay == 0 ? 1000 : 0), bitmap);
        if (result == 2) {
            ov13_0224CF40.parent->flag = 1;
        }
        if (result == 2) {
            result = 0;
        }
        return result;
    case 2:
        result = MBi_SetMPData(MBi_ChildCallback, data, length, 0, bitmap);
        if (result == 2) {
            ov13_0224CF40.parent->flag = 1;
        }
        if (result == 2) {
            result = 0;
        }
        return result;
    default:
        return 1;
    }
}
