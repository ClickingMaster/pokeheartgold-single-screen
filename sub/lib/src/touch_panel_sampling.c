#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/exi/ARM7/genPort.h>
#include <nitro/os/terminate_proc.h>

#include "spi_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#define REG_EXTKEYIN (*(volatile u16 *)0x04000136)

#define EXTKEYIN_PEN_UP 0x0040

void SPIi_ReturnResult(u16 command, u16 result);

int TPi_DetectTouch(void);
void SPI_DummyWait_03801538(void);
int TPi_DetectPos(u16 *out, int range, int axis, u16 *spread);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void TpVAlarmHandler(void *arg) {
    TpSample data;
    if (SPIi_SetEntry(0, 0x10, 1, arg) == 0) {
        data.bits.validity = 3;
        *(u16 *)0x027fffaa = data.halves[0];
        *(u16 *)0x027fffac = data.halves[1];
        SPIi_ReturnResult(0x10, (u32)arg & 0xff);
    }
}

// 0: not touched, 1: touched, 2: touched but the pen went up while sampling.
int TPi_DetectTouch(void) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = 0x8a01;
    REG_SPIDATA = 0x84;
    while (REG_SPICNT & SPICNT_BUSY) {}
    SPI_DummyWait_03801538();
    REG_SPICNT = 0x8201;
    SPI_DummyWait_03801538();
    if (!last_touch_flg) {
        return !(REG_EXTKEYIN & EXTKEYIN_PEN_UP);
    }
    if (!(REG_EXTKEYIN & EXTKEYIN_PEN_UP)) {
        return 1;
    }
    REG_SPICNT = 0x8a01;
    REG_SPIDATA = 0x84;
    while (REG_SPICNT & SPICNT_BUSY) {}
    SPI_DummyWait_03801538();
    REG_SPICNT = 0x8201;
    SPI_DummyWait_03801538();
    return REG_EXTKEYIN & EXTKEYIN_PEN_UP ? 0 : 2;
}

void SPI_DummyWait_03801538(void) {
    REG_SPIDATA = 0;
    while (REG_SPICNT & SPICNT_BUSY) {}
}

// Reads five samples of one axis; returns the average of three that agree within `range`
// (0 = stable, else 1 for x / 2 for y) and the largest spread between any two samples.
int TPi_DetectPos(u16 *out, int range, int axis, u16 *spread) {
    int command;
    u16 *spreadOut = spread;
    int error;
    s32 i;
    s32 j;
    s32 k;
    int max;
    int values[5];
    int vj;
    int vi;
    if (axis == 0) {
        command = 0xd1;
        error = 1;
    } else {
        command = 0x91;
        error = 2;
    }
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = 0x8a01;
    REG_SPIDATA = command & 0xff;
    while (REG_SPICNT & SPICNT_BUSY) {}
    for (i = 0; i < 5; i++) {
        REG_SPIDATA = 0;
        while (REG_SPICNT & SPICNT_BUSY) {}
        values[i] = (u16)(REG_SPIDATA & 0xff) << 8;
        REG_SPIDATA = ((u16)(command & 0xff));
        while (REG_SPICNT & SPICNT_BUSY) {}
        {
            int low = (u16)(REG_SPIDATA & 0xff);
            low |= values[i];
            values[i] = low;
        }
        values[i] = (values[i] & 0x7ff8) >> 3;
    }
    REG_SPICNT = 0x8201;
    SPI_DummyWait_03801538();
    max = 0;
    for (i = 0; i < 4; i++) {
        for (j = i + 1; j < 5; j++) {
            int d;
            d = values[i] - values[j];
            if (d < 0) {
                d = -d;
            }
            if (d > max) {
                max = d;
            }
        }
    }
    *spreadOut = max;
    for (i = 0; i < 3; i++) {
        vi = values[i];
        for (j = i + 1; j < 4; j++) {
            vj = values[j];
            int d = vi - vj;
            if (d < 0) {
                d = -d;
            }
            if (d > range) {
                continue;
            }
            for (k = j + 1; k < 5; k++) {
                int vk = values[k];
                d = vi - vk;
                if (d < 0) {
                    d = -d;
                }
                if (d > range) {
                    continue;
                }
                *out = ((vj + vi * 2 + vk) >> 2) & ~7;
                return 0;
            }
        }
    }
    *out = ((values[0] + values[4]) >> 1) & ~7;
    return error;
}

void TP_ExecSampling(TpSample *sample, int range, u16 *spread) {
    int touch, i;
    u16 value, dx, dy;
    u32 magnitude = (u32)range;
    *spread = 0;
    if (range < 0) {
        magnitude = 0 - magnitude;
    }
    touch = TPi_DetectTouch();
    if (!touch) {
        sample->bits.x = 0;
        sample->bits.y = 0;
        sample->bits.touch = 0;
        sample->bits.validity = 3;
        last_touch_flg = 0;
        return;
    }
    sample->bits.validity = TPi_DetectPos(&value, (int)magnitude, 0, &dx);
    sample->bits.x = value;
    if (TPi_DetectPos(&value, (int)magnitude, 1, &dy) == 2) {
        sample->bits.validity |= 2;
    }
    sample->bits.y = value;
    REG_SPICNT = 0x8a01;
    for (i = 0; i < 12; i++) {
        SPI_DummyWait_03801538();
    }
    REG_SPICNT = 0x8201;
    SPI_DummyWait_03801538();
    if (touch == 2) {
        sample->bits.validity = 3;
    }
    switch (TPi_DetectTouch()) {
    case 2:
        sample->bits.touch = 1;
        sample->bits.validity = 3;
        last_touch_flg = 0;
        break;
    case 1:
        sample->bits.touch = 1;
        last_touch_flg = 1;
        *spread = dx >= dy ? dx : dy;
        break;
    case 0:
        sample->bits.touch = 0;
        last_touch_flg = 0;
        break;
    default:
        OS_Terminate();
        break;
    }
}

#pragma dont_inline reset
