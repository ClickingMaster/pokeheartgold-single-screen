// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern void CPSi_sha1_block(void *, void *);
void CPSi_sha1_calc(void *, void *, int);
void s1(void *storage, void *input, int size);
// @fn CPSi_sha1_calc
void CPSi_sha1_calc(void *storage, void *input, int size) {
    u32 *words = storage;
    u8 *data = input;
    u32 length = (u32)size;
    u32 saved = words[6];
    u32 index = (saved >> 3) & 63U;
    u32 part;
    u32 i;
    words[6] += length << 3;
    if (words[6] < (length << 3)) {
        words[5]++;
    }
    words[5] += length >> 29;
    part = 64 - index;
    if (length >= part) {
        MI_CpuCopy8(data, (u8 *)words + 28 + index, part);
        index = 0;
        CPSi_sha1_block(words, (u8 *)words + 28);
        for (i = part; i + 63 < length; i += 64) {
            CPSi_sha1_block(words, data + i);
        }
    } else {
        i = 0;
    }
    MI_CpuCopy8(data + i, (u8 *)words + 28 + index, length - i);
}
