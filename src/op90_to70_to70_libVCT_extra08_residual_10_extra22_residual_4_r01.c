#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nitro/hw/ARM9/io_reg.h>

typedef struct {
    int state;
    u32 energy;
    int limit;
} VAD;

typedef struct {
    u32 threshold, index;
    int quiet;
} History;

typedef struct {
    u16 control, pad;
    u32 result, param;
} Sqrt;

extern VAD sVADInfo;
u32 E2scales[4];
u32 _021D6FEC;
static u32 sVadIndex;
static int sVadQuiet;
#define SQRT ((volatile Sqrt *)0x40002b0)

static inline u32 RootAvg(void) {
    int i;
    u32 sum = 0;
    for (i = 0; i < 4; i++) {
        sum += E2scales[i];
    }
    SQRT->control = 0;
    SQRT->param = sum >> 2;
    while (SQRT->control & 0x8000) {}
    return SQRT->result;
}

static inline u32 RootAvgU(void) {
    u32 i;
    u32 sum = 0;
    for (i = 0; i < 4; i++) {
        sum += E2scales[i];
    }
    SQRT->control = 0;
    SQRT->param = sum >> 2;
    while (SQRT->control & 0x8000) {}
    return SQRT->result;
}

static inline void PushScale(u32 scale) {
    E2scales[sVadIndex] = scale;
    sVadIndex = (sVadIndex + 1) & 3;
}

int VCTi_GetVADStatus(u32, u32, u8 *);

int VCTi_GetVADStatus(u32 scale, u32 energy, u8 *out) {
    int status;
    sVADInfo.energy = energy >> 8;
    if (out) {
        *out = energy >> 8;
    }
    if (!sVADInfo.state) {
        u32 avg = RootAvg();
        if (energy && energy >= avg * 2) {
            u32 sum = 0;
            u32 i;
            u32 r;
            for (i = 0; i < 4; i++) {
                sum += E2scales[i];
            }
            SQRT->control = 0;
            SQRT->param = sum >> 2;
            while (SQRT->control & 0x8000) {}
            r = SQRT->result;
            status = 1;
            _021D6FEC = (r * 3) >> 1;
            sVadIndex = 0;
            sVADInfo.state = 1;
        } else {
            status = 0;
        }
        PushScale(scale);
    } else {
        u32 avg;
        PushScale(scale);
        avg = RootAvg();
        if (avg <= _021D6FEC) {
            if (++sVadQuiet > sVADInfo.limit) {
                sVADInfo.state = 0;
                sVadQuiet = 0;
                sVadIndex = 0;
                return 3;
            }
        } else {
            sVadQuiet = 0;
        }
        status = 2;
    }
    return status;
}
