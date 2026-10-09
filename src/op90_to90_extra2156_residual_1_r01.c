#pragma force_active on
#include <nitro/types.h>
u64 get48bits_2(u16 *a);
u64 get48bits_3(u16 *a);
u64 get64bits(u16 *a);

#include <nitro/code32.h>

// @fn get48bits_2
asm u64 get48bits_2(u16 *a) {
    ldrh r1, [r0]
    ldrh r0, [r0, #-2]
    mov r0, r0, lsl #0x10
    bx lr
}

// @fn get48bits_3
asm u64 get48bits_3(u16 *a) {
    ldrh r1, [r0]
    ldrh r2, [r0, #-2]
    ldrh r3, [r0, #-4]
    orr r0, r3, r2, lsl #16
    bx lr
}

// @fn get64bits
asm u64 get64bits(u16 *a) {
    ldrh r2, [r0]
    ldrh r3, [r0, #-2]
    orr r1, r3, r2, lsl #16
    ldrh r2, [r0, #-4]
    ldrh r3, [r0, #-6]
    orr r0, r3, r2, lsl #16
    bx lr
}
