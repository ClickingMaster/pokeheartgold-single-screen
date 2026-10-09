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
void ov13_022425D0(int a, void *b);
void ov13_0223FD60(void *p);
void ov13_02240874(void *p);
void ov13_02241D98(void *p);
void ov13_022412B0(void *p);
void ov13_02229C20(void *task);
// @fn ov13_02229C20
void ov13_02229C20(void *task) {
    int j, i;
    ov13_022425D0(0, task);
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 4; i++) {
            ov13_02240874(ov13_0224F4BC->sprites[j][i]);
            if (j == 0) {
                ov13_0223FD60(ov13_0224F4BC->data[i]);
            }
        }
    }
    for (i = 0; i < 2; i++) {
        ov13_0223FD60(ov13_0224F4BC->chars[i]);
    }
    for (i = 0; i < 4; i++) {
        ov13_02241D98(ov13_0224F4BC->cells[i]);
    }
    for (i = 0; i < 47; i++) {
        ov13_02241D98(ov13_0224F4BC->res[i]);
    }
    ov13_022412B0(&ov13_0224F4BC);
}
