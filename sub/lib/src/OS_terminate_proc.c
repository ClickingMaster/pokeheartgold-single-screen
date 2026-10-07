#include <nitro.h>


#ifdef SDK_ARM9
asm void OS_Halt(void) {
    mov r0, #0
    mcr p15, 0, r0, c7, c0, 4
    bx lr
}
#endif
