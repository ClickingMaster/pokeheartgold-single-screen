#include "global.h"
extern u8 *ov74_02231054(void);
extern void *ov74_022311DC(void);
extern u8 *ov74_0223115C(void);
extern int ov74_022311D8(void);
extern void ov74_02230110(void);
extern int WM_SetParentParameter(void *, void *);
extern void ov74_02231448(void);
BOOL ov74_02231508(void);
BOOL ov74_02231508(void) {
    u8 *state = ov74_02231054();
    void *parent = ov74_022311DC();
    u8 *source = ov74_0223115C();
    *(u16 *)(state + 0x32) = source[0x64];
    *(void **)state = parent;
    *(u16 *)(state + 4) = ov74_022311D8();
    if (WM_SetParentParameter(ov74_02230110, state) != 2) {
        ov74_02231448();
        return FALSE;
    }
    return TRUE;
}
