#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// BBP registers DiagBaseBand does not read back (ascending, ended by a value above 104).
extern const u16 BBPDiagSkipAdrsES1[];
extern const u16 BBPDiagSkipAdrsRelease[];

#pragma force_active off

// Three copies of the same comparison: each test loop below has its own.
static inline int BbpValueDiffers1(u32 read, u32 expected) {
    if ((u16)read == expected) {
        return 0;
    } else {
        return 1;
    }
}

static inline int BbpValueDiffers2(u32 read, u32 expected) {
    if ((u16)read == expected) {
        return 0;
    } else {
        return 1;
    }
}

static inline int BbpValueDiffers3(u32 read, u32 expected) {
    if ((u16)read == expected) {
        return 0;
    } else {
        return 1;
    }
}

#pragma force_active on

// Self test of the 105 BBP registers: 0xFF, 0, alternating 0x55/0xAA, a falling count and a walking bit.
// More than 32 mismatches (or a BBP write timeout) end the test; any error sets bit 3 of WlParam.errors.
void DiagBaseBand(void) {
    u32 reg, skipIndex, errors = 0;
    u16 pattern;
    const u16 *skip;
    if (wlWorkPlain->rf.type == 5) {
        skip = BBPDiagSkipAdrsES1;
    } else {
        skip = BBPDiagSkipAdrsRelease;
    }
    {
        reg = 0;
        for (; reg < 105; reg++) {
            if (BBP_Write(reg, 255) == -1) {
                errors = 1;
                goto done;
            }
        }
    }
    {
        reg = 0;
        skipIndex = 0;
        for (; reg < 105; reg++) {
            if (reg == skip[skipIndex]) {
                skipIndex++;
                continue;
            }
            if ((u16)BBP_Read(reg) != 255) {
                if (errors++ > 32) {
                    goto done;
                }
            }
        }
    }
    {
        reg = 0;
        for (; reg < 105; reg++) {
            BBP_Write(reg, 0);
        }
    }
    {
        reg = 0;
        skipIndex = 0;
        for (; reg < 105; reg++) {
            if (reg == skip[skipIndex]) {
                skipIndex++;
                continue;
            }
            if ((u16)BBP_Read(reg) != 0) {
                if (errors++ > 32) {
                    goto done;
                }
            }
        }
    }
    {
        pattern = 0x55;
        reg = 0;
        for (; reg < 105; reg++, pattern = (u16)~pattern) {
            BBP_Write(reg, pattern);
        }
    }
    {
        u32 i;
        u16 expected;
        u32 k;
        i = 0;
        k = 0;
        expected = 0x55;
        for (; i < 105; expected = (u8)~expected, i++) {
            if (i == skip[k]) {
                k++;
                continue;
            }
            if (BbpValueDiffers1(BBP_Read(i), expected)) {
                if (errors++ > 32) {
                    goto done;
                }
            }
        }
    }
    {
        pattern = 255;
        reg = 0;
        for (; reg < 105; reg++, pattern--) {
            BBP_Write(reg, pattern);
        }
    }
    {
        u32 i;
        u16 expected;
        u32 k;
        i = 0;
        k = 0;
        expected = 255;
        for (; i < 105; i++, expected--) {
            if (i == skip[k]) {
                k++;
                continue;
            }
            if (BbpValueDiffers2(BBP_Read(i), expected)) {
                if (errors++ > 32) {
                    goto done;
                }
            }
        }
    }
    {
        for (reg = 0, skipIndex = 0; reg < 105; reg++) {
            if (reg == skip[skipIndex]) {
                skipIndex++;
                continue;
            }
            {
                u32 j, bit;
                for (bit = 1, j = 0; j < 9; bit = (bit << 1) & 255, j++) {
                    BBP_Write(reg, bit);
                    if (BbpValueDiffers3(BBP_Read(reg), bit)) {
                        if (errors++ > 32) {
                            goto done;
                        }
                    }
                }
            }
        }
    }
done:
    if (errors) {
        WlWork *w = wlWorkPlain;
        w->param.errors |= 8;
    }
}
