#include "to50_overlay15_bag_interpolation_private.h"
void ov15_021FDAF4(void *work, s32 next, s32 duration) {
    if (U16(work, 0x128) == U16(work, 0x12A)) {
        U32(work, 0x11C) = U32(work, 0x120);
        U32(work, 0x120) = next;
        U16(work, 0x128) = 0;
        U16(work, 0x12A) = duration;
    } else {
        U32(work, 0x124) = next;
    }
}
void ov15_021FDB2C(void *work, s32 index) {
    const u8 *table = ov15_02200790 + index * 0x90;
    u32 oldValue;
    u32 newValue;
    s32 delta;
    s32 direction;
    s32 outAngle;
    s32 outB;
    s32 outC;
    s32 outD;
    s32 oldOffset;
    s32 newOffset;
    s32 duration;
    s32 step;

    if (U16(work, 0x128) != U16(work, 0x12A)) {
        U16(work, 0x128)
        ++;
        oldOffset = U32(work, 0x11C) << 4;
        oldValue = U16(table, oldOffset);
        newOffset = U32(work, 0x120) << 4;
        newValue = U16(table, newOffset);
        delta = newValue - oldValue;
        if (delta < 0) {
            delta = -delta;
        }
        direction = newValue < oldValue ? -1 : 1;
        if (delta > 0x8000) {
            delta = 0x10000 - delta;
            direction *= -1;
        }
        if (direction > 0) {
            duration = U16(work, 0x12A);
            step = U16(work, 0x128);
            outAngle = oldValue + delta / duration * step;
        } else {
            duration = U16(work, 0x12A);
            step = U16(work, 0x128);
            outAngle = oldValue - delta / duration * step;
        }
        outB = U16(table, oldOffset + 2) + (U16(table, newOffset + 2) - U16(table, oldOffset + 2)) / duration * step;
        outC = S32(table, oldOffset + 8) + (S32(table, newOffset + 8) - S32(table, oldOffset + 8)) / duration * step;
        outD = S32(table, oldOffset + 12) + (S32(table, newOffset + 12) - S32(table, oldOffset + 12)) / duration * step;
        U16(work, 0x10C) = outAngle;
        U16(work, 0x10E) = outB;
        U32(work, 0x108) = outC;
        U32(work, 0x130) = outD;
    }
    if (U16(work, 0x128) == U16(work, 0x12A) && U32(work, 0x124) != -1) {
        U32(work, 0x11C) = U32(work, 0x120);
        U32(work, 0x120) = U32(work, 0x124);
        U32(work, 0x124) = -1;
        U16(work, 0x128) = 0;
    }
}
