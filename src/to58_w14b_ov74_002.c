#include "global.h"
extern int ov74_022310AC(void);
extern void ov74_02231448(void);
extern void ov74_02230018(void);
extern int WM_SetMPDataToPortEx(void *, int, void *, u16, void *, u16, int);
BOOL ov74_02231460(void *data, int size, void *callback);
BOOL ov74_02231460(void *data, int size, void *callback) {
    u16 id = ov74_022310AC();
    if (WM_SetMPDataToPortEx(ov74_02230018, 0, data, (u16)size, callback, id, 2) != 2) {
        ov74_02231448();
        return FALSE;
    }
    return TRUE;
}
