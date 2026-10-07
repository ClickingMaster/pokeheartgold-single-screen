#pragma force_active on
#include "global.h"
typedef struct {u8 pad0[0x2c];u8 ids[4];u8 count;} Config;
typedef struct {Config *config;int chosen;int num;u32 weights[4];} Work;
void GF_AssertFail(void);
void ov93_02262760(int id, int size, const void *data, void *arg);
// @fn ov93_02262760
void ov93_02262760(int id, int size, const void *data, void *arg) {
    Work *work = arg;
    u32 v = *(u32 *)data;
    int i;
    for (i = 0; i < work->config->count; i++) {
        if (id == work->config->ids[i]) {
            work->weights[i] = v;
            break;
        }
    }
    GF_ASSERT(i < work->config->count);
    work->num++;
}
