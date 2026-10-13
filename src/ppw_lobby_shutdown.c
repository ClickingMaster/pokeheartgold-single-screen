#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 *_022577C4;
extern void ov45_02236CB0(void *);
extern int ov45_02240F9C(void);
int PPW_LobbyShutdownAsync(void);

int PPW_LobbyShutdownAsync(void) {
    int result, error;
    volatile u8 unused;
    u8 *p;
    if (!_022577C4) {
        return 0;
    }
    if (*(int *)_022577C4 == 9) {
        return 0;
    }
    ov45_02236CB0(_022577C4 + 0x134);
    *(int *)_022577C4 = 9;
    result = ov45_02240F9C();
    p = _022577C4;
    unused = 0;
    if (result != 1) {
        if (result == 2) {
            error = 3;
        } else {
            error = 0;
        }
    } else {
        error = 2;
    }
    if (error) {
        *(int *)(p + 4) = error;
        *(int *)p = 10;
    }
    return result;
}
