#include "to57_r3_02_private.h"

u8 ov13_02222708(const void *data, s32 size) {
    return ov13_02222730(~0u, data, size, 0, ov13_0224D258) ^ ~0u;
}
u32 ov13_02222730(u32 crc, const u8 *data, s32 size, s32 init, u32 *table) {
    s32 i;
    if (init == 0) {
        ov13_02222768(init, table);
    }
    for (i = 0; i < size; i++) {
        crc = (crc >> 8) ^ table[(u8)(data[i] ^ crc)];
    }
    return crc;
}
void ov13_02222768(s32 unused, u32 *table) {
    u32 value;
    s32 i, j;
    u32 *t = table;
    for (i = 0; i < 256; i++) {
        value = i;
        for (j = 0; j < 8; j++) {
            if (value & 1) {
                value = (value >> 1) ^ 0xEDB88320;
            } else {
                value >>= 1;
            }
        }
        t[i] = value;
    }
}
s32 ov13_022227A0(void *dst, s32 size, const s8 *key, s32 keySize) {
    s32 i;
    u8 *a = ov13_022208E8((s32)(size + ((u32)size >> 31)) >> 1);
    u8 *b;
    if (a == NULL) {
        return -1;
    }
    b = ov13_022208E8(size);
    if (b == NULL) {
        ov13_022208F8(a);
        return -1;
    }
    for (i = 0; i < 2; i++) {
        ov13_0222281C(i, (s8 *)a, size, key, keySize);
        ov13_02222850((s8 *)a, dst, size);
        ov13_02222874(dst, size, b);
    }
    ov13_022208F8(a);
    ov13_022208F8(b);
    return 0;
}
