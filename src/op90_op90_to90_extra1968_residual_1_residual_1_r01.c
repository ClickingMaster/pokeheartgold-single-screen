#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct {
    void *sprites[3][4];
    void *res[47];
    void *cells[4];
    void *chars[2];
    void *data[4];
} Ov13Ctx_9C20;
Ov13Ctx_9C20 *ov13_0224F4BC;
void WifiSetupTask_DestroyLater(int a, void *b);
void WifiSetupOamBlock_Free(void *p);
void WifiSetupText_FreeObjCanvas(void *p);
void WifiSetupOam_Release(void *p);
void WifiSetupHeap_FreeAndClear(void *p);
void ov13_02229C20(void *task);
// @fn ov13_02229C20
void ov13_02229C20(void *task) {
    int j, i;
    WifiSetupTask_DestroyLater(0, task);
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 4; i++) {
            WifiSetupText_FreeObjCanvas(ov13_0224F4BC->sprites[j][i]);
            if (j == 0) {
                WifiSetupOamBlock_Free(ov13_0224F4BC->data[i]);
            }
        }
    }
    for (i = 0; i < 2; i++) {
        WifiSetupOamBlock_Free(ov13_0224F4BC->chars[i]);
    }
    for (i = 0; i < 4; i++) {
        WifiSetupOam_Release(ov13_0224F4BC->cells[i]);
    }
    for (i = 0; i < 47; i++) {
        WifiSetupOam_Release(ov13_0224F4BC->res[i]);
    }
    WifiSetupHeap_FreeAndClear(&ov13_0224F4BC);
}
