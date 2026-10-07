#pragma force_active on
#include "global.h"
typedef struct {u8 pad0[0x2c];u8 ids[4];u8 count;} Config;
typedef struct {Config *config;int chosen;u32 pad8;u32 weights[4];} Work;
int ov93_0225E10C(Work*);
// @fn ov93_0225E10C
int ov93_0225E10C(Work *work) {
    int chosen;
    int i;
    u32 max;
    max = work->weights[0];
    chosen = work->config->ids[0];
    for (i = 1; i < work->config->count; i++) {
        if (max < work->weights[i]) {
            max = work->weights[i];
            chosen = work->config->ids[i];
        }
    }
    work->chosen = chosen;
    return chosen;
}
