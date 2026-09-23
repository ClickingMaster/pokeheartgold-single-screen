#include "global.h"
extern u32 ov74_0223D33C[];
extern s32 sPmAgbCartridgeSpec[];
extern void ov74_02233DBC(int);
BOOL ov74_02233E8C(void);
BOOL ov74_02233E8C(void) {
    int state = ov74_0223D33C[1];
    if (state == 0) {
        if (ov74_0223D33C[0] == 1) sPmAgbCartridgeSpec[1] = 0;
        else sPmAgbCartridgeSpec[1] = 1;
        ov74_0223D33C[4]++;
    } else {
        return FALSE;
    }
    if (sPmAgbCartridgeSpec[1] < 0) return FALSE;
    ov74_02233DBC(state);
    return TRUE;
}
