#include "private_w20.h"

void ov91_02261B10(void) {
    GF_SndHandleSetTempoRatio(7, 0x100);
}

const u8 *ov91_02261B20(void) {
    return ov91_02262754;
}

int ov91_02261B28(void) {
    return 8;
}

void ov91_02261B2C(int value) {
    sub_0203410C(ov91_02261B20(), ov91_02261B28(), value);
}

int ov91_02261B48(void) {
    return 0;
}

u32 ov91_02261B4C(u32 a0, u32 a1, u32 a2, u32 a3) {
    return ov91_0225CA64(a3, a1, a2);
}

u32 ov91_02261B58(u32 a0, u32 a1, u32 a2, u32 a3) {
    return ov91_0225CA6C(a3, a1, a2);
}

void ov91_02261B64(u32 a0, u32 a1, u32 a2, u32 a3) {
    ov91_0225CA90(a3, a2, a0);
    if (!sub_0203769C()) {
        ov91_0225CA74(a3, a2, a0);
    }
}

void ov91_02261B8C(u32 a0, u32 a1, const u32 *a2, u32 a3) {
    ov91_0225CA9C(a3, *a2, a0);
}

void ov91_02261B9C(u32 a0, u32 a1, u32 a2, u32 a3) {
    ov91_0225CABC(a3, a2, a0);
}

void ov91_02261BAC(u32 a0, u32 a1, const u32 *a2, u32 a3) {
    ov91_0225CAEC(a3, *a2, a0);
}

u32 ov91_02261BBC(u32 a0, u32 a1, u32 a2, u32 a3) {
    return ov91_0225CB50(a3, a1, a2);
}

void ov91_02261BC8(u32 a0, u32 a1, const u32 *a2, u32 a3) {
    u32 value = *a2;
    if (a0 == 0) {
        ov91_0225CAE0(a3, value);
    }
}

int ov91_02261BD8(void) { return 0x14; }

int ov91_02261BDC(void) { return 0x10; }

int ov91_02261BE0(void) { return 4; }

int ov91_02261BE4(void) { return 4; }
