#pragma force_active on
#include "global.h"
typedef struct Ov112Entry {
    u16 offset;
    u8 size;
    u8 sum;
} Ov112Entry;
extern const u8 *ov112_021FF244[];
extern const u8 ov112_021FF2CC[];
extern void MI_CpuCopy8(const void *src, void *dst, u32 size);
void ov112_021F0D48(Ov112Entry *entries);

// @fn ov112_021F0D48
void ov112_021F0D48(Ov112Entry *entries) {
    u16 offset = 0;
    int i;
    u8 *data = (u8 *)entries;
    data += 0x40;
    for (i = 0; i < 16; i++) {
        u8 sum;
        u32 j;
        const u8 *src;
        entries[i].offset = offset;
        entries[i].size = ov112_021FF2CC[i];
        sum = 0;
        src = ov112_021FF244[i];
        for (j = 0; j < entries[i].size / 2; j++) {
            sum += src[0];
            sum += src[1];
            src += 2;
        }
        entries[i].sum = sum;
        MI_CpuCopy8(ov112_021FF244[i], data + offset, entries[i].size);
        offset += entries[i].size;
    }
}
