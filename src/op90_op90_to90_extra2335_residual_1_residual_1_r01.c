#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct {
    void *sprites[4];
    void *res[10];
    void *cells[2];
    void *chars[2];
    void *data[4];
} Ov13Ctx_B194;
Ov13Ctx_B194 *ov13_0224F4C0;
void WifiSetupTask_DestroyLater(int a, void *b);
void WifiSetupOamBlock_Free(void *p);
void WifiSetupText_FreeObjCanvas(void *p);
void WifiSetupOam_Release(void *p);
void WifiSetupHeap_FreeAndClear(void *p);
void ov13_0222B194(void *task);
// @fn ov13_0222B194
void ov13_0222B194(void *task) {
    int i;
    WifiSetupTask_DestroyLater(0, task);
    for (i = 0; i < 4; i++) {
        WifiSetupOamBlock_Free(ov13_0224F4C0->data[i]);
        WifiSetupText_FreeObjCanvas(ov13_0224F4C0->sprites[i]);
    }
    for (i = 0; i < 2; i++) {
        WifiSetupOamBlock_Free(ov13_0224F4C0->chars[i]);
    }
    for (i = 0; i < 2; i++) {
        WifiSetupOam_Release(ov13_0224F4C0->cells[i]);
    }
    for (i = 0; i < 10; i++) {
        WifiSetupOam_Release(ov13_0224F4C0->res[i]);
    }
    WifiSetupHeap_FreeAndClear(&ov13_0224F4C0);
}
