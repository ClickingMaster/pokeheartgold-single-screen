// libVCT FIR translation unit (HPFFilter .. vct_process_fir) as one object.
// Owns the TU's pooled .bss 0x021D7008..0x021D848C (MWCC -ipa file pools it, heap-sorted by size):
// DoFIRFilter's gain shift (function-local static _021D7008) +0, sPrevProduct +4, sRead +8, sWrite +0xC,
// sPrevOutput +0x10, sH +0x14, sTap +0x2C, sSpkBuffer +0x44, sDelayLine +0x484.
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nitro/hw/ARM9/io_reg.h>

extern s16 sImpulseResponse[];

s32 sTap[6];
s32 sH[6];
u32 sSpkBuffer[0x110];
static u32 sRead;
static u32 sWrite;
static s32 sPrevProduct;
static s32 sPrevOutput;
s16 sDelayLine[0x800];

u32 HPFFilter(s16 *buffer, u32 bytes);
void InitFIRFilter(void);
void vct_set_speaker_samples(void *samples, u32 size);
void DoFIRFilter(s16 *src, s16 *dst, u32 length, u32 threshold);
void vct_process_fir(void *samples, u32 size, u32 energy);

u32 HPFFilter(s16 *buffer, u32 bytes) {
    s32 product = buffer[0] * 0xE9F;
    s32 output = (product + sPrevProduct + sPrevOutput * 0xD3E) >> 12;
    s32 previous = -product;
    u32 i = 1;
    s64 energy = (s32)((u32)output * (u32)output);

    bytes >>= 1;
    sPrevOutput = output;
    sPrevProduct = previous;
    if (bytes > 1) {
        do {
            product = buffer[i] * 0xE9F;
            {
                s32 next = (s32)((u32)product + (u32)previous + (u32)output * 0xD3E);
                buffer[i - 1] = (s16)output;
                output = next >> 12;
            }
            energy += (s32)((u32)output * (u32)output);
            previous = -product;
            i++;
        } while (i < bytes);
        sPrevProduct = previous;
        sPrevOutput = output;
    }
    buffer[bytes - 1] = (s16)output;

    reg_CP_DIVCNT = 1;
    *(s64 *)REG_DIV_NUMER_ADDR = energy;
    *(s64 *)REG_DIV_DENOM_ADDR = bytes;
    while (reg_CP_DIVCNT & 0x8000) {
    }
    return reg_CP_DIV_RESULT_L;
}

#pragma push
#pragma opt_strength_reduction off

void InitFIRFilter(void) {
    s16 *impulse;
    int i, j;

    sWrite = 0;
    sRead = 413;
    impulse = sImpulseResponse;
    for (i = 0; i < 2048; i++) {
        sDelayLine[i] = 0;
    }
    for (j = 0; j < 6; j++) {
        sTap[j] = 0;
        sH[j] = impulse[j];
    }
}

#pragma pop

void vct_set_speaker_samples(void *samples, u32 size) {
    MIi_CpuCopyFast(samples, sSpkBuffer, size);
}

void DoFIRFilter(s16 *src, s16 *dst, u32 length, u32 threshold) {
    static s16 _021D7008;
    s64 energy;
    u32 read;
    u32 root;
    int write;
    u32 i;
    s32 acc;
    s16 sample;

    energy = 0;
    length >>= 1;
    for (i = 0; i < length; i++) {
        write = sWrite;
        sDelayLine[write] = dst[i];
        read = sRead;
        sTap[0] = sDelayLine[read];
        acc = sTap[5] * sH[5];
        acc += sTap[4] * sH[4];
        sTap[5] = sTap[4];
        acc += sTap[3] * sH[3];
        sTap[4] = sTap[3];
        acc += sTap[2] * sH[2];
        sTap[3] = sTap[2];
        acc += sTap[1] * sH[1];
        sTap[2] = sTap[1];
        acc += sTap[0] * sH[0];
        sTap[1] = sTap[0];
        if (acc > 0x3FFF8000) {
            acc = 0x3FFF8000;
        } else if (acc < (s32)0xC0000000) {
            acc = (s32)0xC0000000;
        }
        sWrite = (write + 1) & 0x7FF;
        sRead = (read + 1) & 0x7FF;
        dst[i] = (s16)(acc >> 15);
        sample = dst[i];
        dst[i] = src[i];
        energy += sample * sample;
    }

    reg_CP_DIVCNT = 1;
    *(s64 *)REG_DIV_NUMER_ADDR = energy;
    *(s64 *)REG_DIV_DENOM_ADDR = length;
    while (reg_CP_DIVCNT & 0x8000) {
    }
    {
        u32 avg = reg_CP_DIV_RESULT_L;
        reg_CP_SQRTCNT = 0;
        reg_CP_SQRT_PARAM_L = avg;
    }
    while (reg_CP_SQRTCNT & 0x8000) {
    }
    root = reg_CP_SQRT_RESULT;

    if (root * 3 < threshold * 2) {
        _021D7008 = 0;
        return;
    }
    if (threshold < root) {
        _021D7008 = 4;
    }
    if (_021D7008 < 4) {
        _021D7008++;
    }
    for (i = 0; i < length; i++) {
        src[i] = src[i] >> _021D7008;
    }
}

void vct_process_fir(void *samples, u32 size, u32 energy) {
    DoFIRFilter(samples, (s16 *)sSpkBuffer, size, energy);
}
