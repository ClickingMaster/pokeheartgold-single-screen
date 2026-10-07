#pragma force_active on
#include "global.h"
typedef struct {u8 pad0[0x2c];u8 ids[4];u8 count;} Config;
typedef struct {Config *config;int chosen;u32 pad8;u32 weights[4];} Work;
void GF_AssertFail(void);
int ov93_0225E45C(Work*, int);
// @fn ov93_0225E45C
int ov93_0225E45C(Work *work, int id) {
    int i;
    for (i = 0; i < work->config->count; i++) {
        if (id == work->config->ids[i]) {
            return i;
        }
    }
    GF_AssertFail();
    return 0;
}
