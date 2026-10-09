#pragma force_active on
#include <nitro/types.h>

u32 OSi_CpuClear32(register u32 value, register void *ptr, register u32 size);

#include <nitro/code32.h>
#include <nitro/itcm_begin.h>

// @fn OSi_CpuClear32
asm u32 OSi_CpuClear32(register u32 value, register void *ptr, register u32 size) {
    add r12, r1, r2
@loop:
    cmp r1, r12
    stmltia r1!, {r0}
    blt @loop
    bx lr
}

#include <nitro/itcm_end.h>
