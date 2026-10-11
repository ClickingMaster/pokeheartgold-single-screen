#include <nitro/types.h>

// The code below is compiled as Thumb (16-bit) code.
#include <nitro/code16.h>

// BIOS calls SVC_WaitVBlankIntr .. SVC_DivRem. Their entry points sit at 2-byte aligned addresses,
// which MWCC cannot emit as separate functions (each function gets a 4-aligned section), so the run is
// one asm function; tools/object_aliases.py adds the inner entry points as aliases
// (svc_wait_vblank_div.alias-map.json).

void SVC_WaitVBlankIntr(void);

// clang-format off
asm void SVC_WaitVBlankIntr(void)
{
    // SVC_WaitVBlankIntr
    mov r2, #0
    swi 5
    bx lr
    // SVC_Halt (+0x06)
    swi 6
    bx lr
    // SVC_Sleep (+0x0A)
    swi 7
    bx lr
    // SVC_ChangeSoundBias (+0x0E)
    swi 8
    bx lr
    // SVC_SetSoundBias (+0x12)
    add r1, r0, #0
    mov r0, #1
    swi 8
    bx lr
    // SVC_ResetSoundBias (+0x1A)
    add r1, r0, #0
    mov r0, #0
    swi 8
    bx lr
    // SVC_Div (+0x22)
    swi 9
    bx lr
    // SVC_DivRem (+0x26)
    swi 9
    add r0, r1, #0
    bx lr
}
// clang-format on
