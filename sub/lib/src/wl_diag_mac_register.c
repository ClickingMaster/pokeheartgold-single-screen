#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Self test of the MAC registers in test_reg: the test patterns, an incrementing pattern, then zero.
// More than 32 mismatches end the test; any mismatch sets bit 0 of WlParam.errors.
void DiagMacRegister(void) {
    u32 errors;
    u32 j, i;
    errors = 0;
    i = 0;
    for (; i < 3; i++) {
        for (j = 0; j < 27; j++) {
            vu16 *reg = &WL_REG(test_reg[j].offset);
            u32 value = test_pattern[i] & test_reg[j].mask;
            *reg = value;
            if (*reg != value) {
                if (errors++ > 32) {
                    goto end;
                }
            }
        }
    }
    {
        u16 pattern = 0x1234;
        u32 k;
        for (k = 0; k < 27; k++) {
            WL_REG(test_reg[k].offset) = pattern & test_reg[k].mask;
            pattern += 0x1234;
        }
    }
    {
        u32 k;
        u16 pattern = 0x1234;
        for (k = 0; k < 27; k++) {
            if (WL_REG(test_reg[k].offset) != (pattern & test_reg[k].mask)) {
                if (errors++ > 32) {
                    goto end;
                }
            }
            pattern += 0x1234;
        }
    }
    {
        u32 k;
        for (k = 0; k < 27; k++) {
            WL_REG(test_reg[k].offset) = 0;
            if (WL_REG(test_reg[k].offset) != 0) {
                if (errors++ > 32) {
                    goto end;
                }
            }
        }
    }
end:
    if (errors) {
        wlWorkPlain->param.errors |= 1;
    }
}
