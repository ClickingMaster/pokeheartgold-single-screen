#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a) (*(volatile u16 *)(a))

typedef struct {
    s8 bias8;
    u8 pad;
    s16 bias12;
    u32 sum12, count12, count8, sum8;
} MICCalib;

extern MICCalib micCalib;
extern u8 SPI_DummyWaitReceive(void);
u16 MIC_ExecSampling8(void);

u16 MIC_ExecSampling8(void) {
    u32 first, sample;
    int value;
    while (H(0x040001c0) & 128) {}
    H(0x040001c0) = 0x8a01;
    H(0x040001c2) = 0xec;
    while (H(0x040001c0) & 128) {}
    first = (SPI_DummyWaitReceive() & 0xff) << 8;
    H(0x040001c0) = 0x8201;
    sample = SPI_DummyWaitReceive() | first;
    sample = (u16)((sample & 0x7f80) >> 7);
    micCalib.sum8 += sample;
    micCalib.count8++;
    if (micCalib.count8 >= 0x1000) {
        int target = (s8)((micCalib.sum8 >> 12) - 0x80);
        int current = micCalib.bias8;
        if (current < target && current < 12) {
            micCalib.bias8 = current + 1;
        } else if (target < current && current > -12) {
            micCalib.bias8--;
        }
        micCalib.count8 = 0;
        micCalib.sum8 = 0;
    }
    value = (int)sample - micCalib.bias8;
    if (value > 0xff) {
        value = 0xff;
    } else if (value < 0) {
        value = 0;
    }
    return (u16)value;
}
