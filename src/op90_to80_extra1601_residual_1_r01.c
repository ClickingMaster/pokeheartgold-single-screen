#pragma force_active on
#include "global.h"
#include <string.h>
typedef struct MacPair {
    u8 first[6];
    u8 second[6];
    u8 prefix[4];
} MacPair;
typedef struct State58 {
    u32 unk0;
    u32 debug;
} State58;
extern u8 ov13_02245B18[4];
extern u8 ov13_0224E3E0[6];
extern u8 ov13_0224E3D8[6];
extern State58 ov13_02245A58;
BOOL ov13_02224A24(u8 *mac);
void ov13_02226D40(char *buf, const u8 *mac);
BOOL ov13_02224A30(MacPair *pair);
// @fn ov13_02224A30
BOOL ov13_02224A30(MacPair *pair) {
    u8 other[6];
    u8 own[6];
    char buf1[32];
    char buf2[32];
    u8 *src;

    memcpy(pair->prefix, ov13_02245B18, 4);
    memcpy(other, ov13_0224E3E0, 6);
    other[0] &= 0xFD;
    ov13_02224A24(own);
    memcpy(ov13_0224E3D8, own, 6);
    if (memcmp(other, own, 6) <= 0) {
        memcpy(pair->first, own, 6);
        src = other;
    } else {
        memcpy(pair->first, other, 6);
        src = own;
    }
    memcpy(pair->second, src, 6);
    if (ov13_02245A58.debug) {
        ov13_02226D40(buf1, own);
        ov13_02226D40(buf2, other);
    }
    return TRUE;
}
