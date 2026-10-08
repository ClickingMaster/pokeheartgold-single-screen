#pragma force_active on
#include "global.h"
typedef struct {u8 pad0[0x2c];u8 ids[4];u8 count;} Config;
typedef struct {Config *config;} Work;
int sub_0203769C(void);
extern u8 ov93_02262A54[][4];
int ov93_0225E3C4(Work *work, int id);
// @fn ov93_0225E3C4
#pragma optimization_level 2
#pragma opt_common_subs off
#pragma opt_rebuildlogicals off
int ov93_0225E3C4(Work *work, int id) {
    int myId;
    int them;
    int me;
    int i;
    int themPos;
    int mePos;
    int diff;

    myId = sub_0203769C();
    them = 0xFF;
    me = them;
    for (i = 0; i < work->config->count; i++) {
        if (myId == work->config->ids[i]) {
            me = i;
        }
        if (id == work->config->ids[i]) {
            them = i;
        }
    }
    GF_ASSERT(them != 0xFF && me != 0xFF);
    for (i = 0; i < work->config->count; i++) {
        if (me == ov93_02262A54[work->config->count][i]) {
            mePos = i;
        }
        if (them == ov93_02262A54[work->config->count][i]) {
            themPos = i;
        }
    }
    diff = themPos - mePos;
    if (diff < 0) {
        diff += work->config->count;
    }
    return diff;
}
#pragma opt_rebuildlogicals reset
#pragma opt_common_subs reset
#pragma optimization_level reset
