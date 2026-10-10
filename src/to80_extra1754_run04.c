#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 flags, offset;
} Track;

typedef struct {
    u32 p;
    u16 frames, p6;
    u32 flags, small, large;
} Header;

int getRotDataByIdx_(MtxFx33 *, s16 *, s16 *, u32);
void getRotDataEx_(MtxFx33 *, fx32, Track *, Header *);
#define CROSS()                                                \
    {                                                          \
        fx32 x, y, z;                                          \
        x = (out->_01 * out->_12 - out->_02 * out->_11) >> 12; \
        y = (out->_02 * out->_10 - out->_00 * out->_12) >> 12; \
        z = (out->_00 * out->_11 - out->_01 * out->_10) >> 12; \
        out->_20 = x;                                          \
        out->_21 = y;                                          \
        out->_22 = z;                                          \
    }

void getRotDataEx_(MtxFx33 *out, fx32 time, Track *track, Header *base) {
    MtxFx33 a, b;
    u32 large;
    u32 small, next;
    int fraction, factor;
    u16 *indices;
    u32 frame, flags;
    int bits;
    small = track->offset;
    indices = (u16 *)((u8 *)base + small);
    frame = time >> 12;
    small = base->small;
    large = base->large;
    flags = track->flags;
    if (base->frames - 1 == (time >> 12)) {
        if (flags & 0xC0000000) {
            if (flags & 0x40000000) {
                frame = (frame & 1) + (frame >> 1);
            } else {
                frame = (frame & 3) + (frame >> 2);
            }
        }
        if (base->flags & 2) {
            next = 0;
        } else {
            bits = getRotDataByIdx_(out, (s16 *)((u8 *)base + small), (s16 *)((u8 *)base + large), indices[frame]);
            if (bits) {
                CROSS();
            } else {
                VEC_Normalize((VecFx32 *)&out->_20, (VecFx32 *)&out->_20);
            }
            return;
        }
        goto normal;
    }
    if (flags & 0xC0000000) {
        u32 last = (flags & 0x1FFF0000) >> 16;
        if (flags & 0x40000000) {
            if (frame >= last) {
                frame = last >> 1;
                next = frame + 1;
                goto normal;
            }
            frame >>= 1;
            next = frame + 1;
            fraction = time & 0x1FFF;
            factor = 2;
            goto blend;
        } else {
            if (frame >= last) {
                frame = (frame & 3) + (frame >> 2);
                next = frame + 1;
                goto normal;
            }
            frame >>= 2;
            next = frame + 1;
            fraction = time & 0x3FFF;
            factor = 4;
            goto blend;
        }
    }
    next = frame + 1;
normal:
    fraction = time & 0xFFF;
    factor = 1;
blend:
    bits = 0;
    bits |= getRotDataByIdx_(&a, (s16 *)((u8 *)base + small), (s16 *)((u8 *)base + large), indices[frame]);
    bits |= getRotDataByIdx_(&b, (s16 *)((u8 *)base + small), (s16 *)((u8 *)base + large), indices[next]);
    out->_00 = a._00 * factor + ((fraction * (b._00 - a._00)) >> 12);
    out->_01 = a._01 * factor + ((fraction * (b._01 - a._01)) >> 12);
    out->_02 = a._02 * factor + ((fraction * (b._02 - a._02)) >> 12);
    out->_10 = a._10 * factor + ((fraction * (b._10 - a._10)) >> 12);
    out->_11 = a._11 * factor + ((fraction * (b._11 - a._11)) >> 12);
    out->_12 = a._12 * factor + ((fraction * (b._12 - a._12)) >> 12);
    VEC_Normalize((VecFx32 *)out, (VecFx32 *)out);
    VEC_Normalize((VecFx32 *)&out->_10, (VecFx32 *)&out->_10);
    if (!bits) {
        out->_20 = a._20 * factor + ((fraction * (b._20 - a._20)) >> 12);
        out->_21 = a._21 * factor + ((fraction * (b._21 - a._21)) >> 12);
        out->_22 = a._22 * factor + ((fraction * (b._22 - a._22)) >> 12);
        VEC_Normalize((VecFx32 *)&out->_20, (VecFx32 *)&out->_20);
        return;
    }
    CROSS();
}
