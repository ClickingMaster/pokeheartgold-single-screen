#pragma force_active on
#include "global.h"
void ov83_022477EC(int index, int value, u32 *word);
// @fn ov83_022477EC
void ov83_022477EC(int index, int value, u32 *word) {
    int shift;
    u32 v;
    u32 mask;
    u32 bits;
    shift = index * 4;
    v = value + 1;
    mask = 0xF;
    bits = v << shift;
    *word = (*word & ((mask << shift) ^ 0xFFFFFFFF)) | bits;
}
