#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern void *(*_021D4634)(u32);
extern void (*_021D4660)(void *);
int count_digits(const u16 *, int);
void CPSi_big_sqr(u16 *, const u16 *, int);
void CPSi_big_mult(u16 *, const u16 *, const u16 *, int);
void CPSi_big_div(u16 *, u16 *, const u16 *, const u16 *, int, u16 *);
void CPSi_big_power(u16 *out, const u16 *base, const u16 *power, int digits, const u16 *modulus);

void CPSi_big_power(u16 *out, const u16 *base, const u16 *power, int digits, const u16 *modulus) {
    u16 *buffer = _021D4634(digits * 8), *work;
    int bit;
    if (!buffer) {
        return;
    }
    work = buffer + digits;
    MI_CpuFill8(out + 1, 0, (digits - 1) * 2);
    out[0] = 1;
    bit = (digits - count_digits(power, digits)) * 16;
    for (; bit < (u32)(digits * 16); bit++) {
        if (power[digits - (bit >> 4) - 1] & (0x8000u >> (bit & 15))) {
            MI_CpuCopy8(base, out, digits * 2);
            bit++;
            break;
        }
    }
    for (; bit < (u32)(digits * 16); bit++) {
        CPSi_big_sqr(buffer, out, digits);
        MI_CpuCopy8(buffer, out, digits * 2);
        if (modulus) {
            CPSi_big_div(NULL, out, modulus, out, digits, work);
        }
        if (power[digits - (bit >> 4) - 1] & (0x8000u >> (bit & 15))) {
            CPSi_big_mult(buffer, out, base, digits);
            MI_CpuCopy8(buffer, out, digits * 2);
            if (modulus) {
                CPSi_big_div(NULL, out, modulus, out, digits, work);
            }
        }
    }
    _021D4660(buffer);
}
