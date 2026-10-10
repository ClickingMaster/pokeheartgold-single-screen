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
u16 MIC_ExecSampling12(void);

u16 MIC_ExecSampling12(void) {
    u16 data, sample;
    int value;
    while (H(0x040001c0) & 128) {}
    H(0x040001c0) = 0x8a01;
    H(0x040001c2) = 0xe4;
    while (H(0x040001c0) & 128) {}
    data = SPI_DummyWaitReceive() << 8;
    H(0x040001c0) = 0x8201;
    data |= SPI_DummyWaitReceive();
    sample = (data & 0x7ff8) << 1;
    micCalib.sum12 += sample;
    micCalib.count12++;
    if (micCalib.count12 >= 0x100) {
        int target = (s16)(((micCalib.sum12 >> 8) & 0xfff0) - 0x8000);
        int current = micCalib.bias12;
        if (current < target && current < 0xc00) {
            micCalib.bias12 = current + 16;
        } else if (target < current && current > -0xc00) {
            micCalib.bias12 -= 16;
        }
        micCalib.count12 = 0;
        micCalib.sum12 = 0;
    }
    value = (int)sample - micCalib.bias12;
    if (value > 0xfff0) {
        value = 0xfff0;
    } else if (value < 0) {
        value = 0;
    }
    return (u16)value;
}
