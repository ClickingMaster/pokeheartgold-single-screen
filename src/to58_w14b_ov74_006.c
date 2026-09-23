#include "global.h"
extern u8 *ov74_022310C4(void);
extern volatile u8 *ov74_02231154(void);
extern void ov74_02230520(void);
extern int WM_StartScan(void *, void *);
extern void ov74_02231448(void);
BOOL ov74_02231584(void);
BOOL ov74_02231584(void) {
    u8 *state = ov74_022310C4();
    volatile u8 *source = ov74_02231154();
    state[8] = source[(((u32)source[0x1C3] << 28) >> 28) * 0x38 + 0x34];
    state[9] = source[(((u32)source[0x1C3] << 28) >> 28) * 0x38 + 0x35];
    state[10] = source[(((u32)source[0x1C3] << 28) >> 28) * 0x38 + 0];
    state[11] = source[(((u32)source[0x1C3] << 28) >> 28) * 0x38 + 1];
    state[12] = source[(((u32)source[0x1C3] << 28) >> 28) * 0x38 + 2];
    state[13] = source[(((u32)source[0x1C3] << 28) >> 28) * 0x38 + 3];
    if (WM_StartScan(ov74_02230520, ov74_022310C4()) != 2) {
        ov74_02231448();
        return FALSE;
    }
    return TRUE;
}
