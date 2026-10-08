#pragma force_active on
#include "global.h"
typedef struct { u16 value; u8 rest[14]; } Entry;
extern u8 ov65_0221FFB4[][4][6];
u8 ov65_0221DCFC(int category, int set, Entry *entries);

// @fn ov65_0221DCFC
u8 ov65_0221DCFC(int category, int set, Entry *entries) {
    int i;
    u8 ret = 0;
    for (i = 0; i < 6; i++) {
        if (entries[ov65_0221FFB4[category][set][i]].value) {
            ret = ov65_0221FFB4[category][set][i];
            break;
        }
    }
    return ret;
}
