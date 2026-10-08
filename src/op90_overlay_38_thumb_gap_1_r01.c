#pragma force_active on
#include "global.h"

int ov38_0221BB44(int len);
u8 *ov00_021EC2A8(int a0, int size);
void ov00_021EC2EC(int a0, void *ptr);
void ov38_0221BA00(u32 seed);
u32 ov38_0221BA10(void);
void ov00_021FFC18(const void *src, char *dst, int len, int a3);
int ov38_0221BA38(u32 key, const u8 *data, int len, char *out, int outSize);

// @fn ov38_0221BA38
int ov38_0221BA38(u32 key, const u8 *data, int len, char *out, int outSize) {
    u32 sum = 0;
    int total = len + 8;
    int b1;
    int b2;
    int b3;
    int b0;
    u8 *buf;
    int i;
    u32 x;

    if (outSize < ov38_0221BB44(total) + 1) {
        return 2;
    }
    buf = ov00_021EC2A8(10, total);
    if (buf == NULL) {
        return 1;
    }
    b3 = (u8)(key >> 24);
    sum += (u8)b3;
    b2 = (u8)(key >> 16);
    sum += (u8)b2;
    b1 = (u8)(key >> 8);
    sum += (u8)b1;
    b0 = (u8)key;
    sum += (u8)b0;
    for (i = 0; i < len; i++) {
        sum += data[i];
    }
    ov38_0221BA00(sum);
    buf[4] = ov38_0221BA10() ^ b0;
    buf[5] = ov38_0221BA10() ^ b1;
    buf[6] = ov38_0221BA10() ^ b2;
    buf[7] = ov38_0221BA10() ^ b3;
    for (i = 0; i < len; i++) {
        buf[8 + i] = data[i] ^ ov38_0221BA10();
    }
    x = sum ^ 0x4A3B2C1D;
    buf[0] = x >> 24;
    buf[1] = x >> 16;
    buf[2] = x >> 8;
    buf[3] = x;
    ov00_021FFC18(buf, out, len + 8, 2);
    out[ov38_0221BB44(total)] = 0;
    ov00_021EC2EC(10, buf);
    return 0;
}
