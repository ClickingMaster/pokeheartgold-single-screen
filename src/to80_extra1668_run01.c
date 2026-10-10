#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 pad0[0x508];
    void (*reset)(void *);
} Work;

typedef struct {
    u8 pad0[8];
    Work *work;
} Global;

extern Global ov13_0224CF40;
extern int WM_Reset(void (*)(void *));
extern void MBi_CheckWmErrcode(int, int);
int MBi_CallReset(void);

int MBi_CallReset(void) {
    int result = WM_Reset(ov13_0224CF40.work->reset);
    MBi_CheckWmErrcode(1, result);
    if (result == 2) {
        result = 0;
    }
    return result;
}
