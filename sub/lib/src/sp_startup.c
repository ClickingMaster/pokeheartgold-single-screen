#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Start-up helpers of the ARM7 crt0 (_start), linked right after it.

extern u32 SDK_AUTOLOAD_LIST[];
extern u32 SDK_AUTOLOAD_LIST_END[];
extern u32 SDK_AUTOLOAD_START[];
extern u32 SDK_STATIC_BSS_START[];
extern u32 SDK_STATIC_BSS_END[];

void _start_AutoloadDoneCallback(void);
void detect_main_memory_size(void);
void _start_ModuleParams(void);

#pragma force_active on

void _start_AutoloadDoneCallback(void) {
}

// Writes 0/1 to the last halfword of main memory and reads it back 4 MB lower: 1 when that address
// mirrors it (4 MB), 2 otherwise (8 MB). The result is stored in that halfword.
// clang-format off
asm void detect_main_memory_size(void)
{
    mov r0, #1
    mov r1, #0
    ldr r2, =0x027FFFFA
    sub r3, r2, #0x400000
@loop:
    strh r1, [r2]
    ldrh r12, [r3]
    cmp r1, r12
    movne r0, #2
    bne @done
    add r1, r1, #1
    cmp r1, #2
    bne @loop
@done:
    strh r0, [r2]
    bx lr
}
// clang-format on

#pragma force_active reset

// The autoload / static .bss parameter block that _start reads (5 words, emitted as an asm body so it
// stays in .text right after the start-up code).
// clang-format off
asm void _start_ModuleParams(void)
{
    dcd SDK_AUTOLOAD_LIST
    dcd SDK_AUTOLOAD_LIST_END
    dcd SDK_AUTOLOAD_START
    dcd SDK_STATIC_BSS_START
    dcd SDK_STATIC_BSS_END
}
// clang-format on
