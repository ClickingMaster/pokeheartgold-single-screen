#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int DWC_CheckUserData(void *);
extern int DWC_CheckHasProfile(void *);
extern int ov45_02231144(int, int, int, void *, int, int, int, int);
int PPW_LobbyInitializeAsync(int a, int b, int c, void *d, u8 *user, int e, int f);
#pragma force_active on
#pragma opt_pointer_analysis off

int PPW_LobbyInitializeAsync(int a, int b, int c, void *d, u8 *user, int e, int f) {
    u8 *p = (u8 *)(u32)user;
    if (!DWC_CheckUserData(p) || !DWC_CheckHasProfile(p)) {
        return 3;
    }
    return ov45_02231144(a, b, c, d, *(int *)(p + 28), e, f, 1);
}
