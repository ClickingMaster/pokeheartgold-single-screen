#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u32 __VENEER_SVC_GetPitchTable(int);
u16 SND_CalcTimer(int base, int pitch);

u16 SND_CalcTimer(int base, int pitch) {
    int octave = 0;
    int rem = (s32)(0u - (u32)pitch);
    u64 timer, rate;
    while (rem < 0) {
        octave--;
        rem += 768;
    }
    while (rem >= 768) {
        octave++;
        rem -= 768;
    }
    rate = (u64)__VENEER_SVC_GetPitchTable(rem) + 0x10000u;
    timer = rate * (u64)(s64)base;
    octave -= 16;
    if (octave <= 0) {
        u32 shift = 0u - (u32)octave;
        timer >>= shift;
    } else {
        if (octave < 32) {
            if (timer & (~(u64)0 << (32 - octave))) {
                return 65535;
            }
            timer <<= octave;
        } else {
            return 65535;
        }
    }
    if (timer < 16) {
        timer = 16;
    } else if (timer > 65535) {
        timer = 65535;
    }
    return (u16)timer;
}
