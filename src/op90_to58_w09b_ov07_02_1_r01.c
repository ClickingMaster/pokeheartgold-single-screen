#pragma force_active on
#include "global.h"
int ov07_0221FAF8(void *owner, int index);
void ov07_02231EC0(void *owner, int window, BOOL effect);

// @fn ov07_02231EC0
void ov07_02231EC0(void *owner, int window, BOOL effect) {
    if (window == 0) {
        G2_SetWnd0InsidePlane((1 << ov07_0221FAF8(owner, 2)) | (1 << ov07_0221FAF8(owner, 0)) | (1 << ov07_0221FAF8(owner, 1)) | GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_OBJ, effect);
    } else {
        G2_SetWnd1InsidePlane((1 << ov07_0221FAF8(owner, 2)) | (1 << ov07_0221FAF8(owner, 0)) | (1 << ov07_0221FAF8(owner, 1)) | GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_OBJ, effect);
    }
    G2_SetWndOutsidePlane((1 << ov07_0221FAF8(owner, 0)) | (1 << ov07_0221FAF8(owner, 1)) | GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_OBJ, TRUE);
}
