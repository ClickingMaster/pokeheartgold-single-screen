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
void ov13_022425D0(int a, void *b);
void ov13_0223FD60(void *p);
void ov13_02240874(void *p);
void ov13_02241D98(void *p);
void ov13_022412B0(void *p);
void ov13_0222B194(void *task);
// @fn ov13_0222B194
void ov13_0222B194(void *task) {
    int i;
    ov13_022425D0(0, task);
    for (i = 0; i < 4; i++) {
        ov13_0223FD60(ov13_0224F4C0->data[i]);
        ov13_02240874(ov13_0224F4C0->sprites[i]);
    }
    for (i = 0; i < 2; i++) {
        ov13_0223FD60(ov13_0224F4C0->chars[i]);
    }
    for (i = 0; i < 2; i++) {
        ov13_02241D98(ov13_0224F4C0->cells[i]);
    }
    for (i = 0; i < 10; i++) {
        ov13_02241D98(ov13_0224F4C0->res[i]);
    }
    ov13_022412B0(&ov13_0224F4C0);
}
