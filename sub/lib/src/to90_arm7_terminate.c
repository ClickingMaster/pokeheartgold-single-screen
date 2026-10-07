#include <nitro.h>

void OS_Terminate(void) {
#ifdef SDK_ARM7
    CTRDG_VibPulseEdgeUpdate(0);
#endif
    while (1) {
        OS_DisableInterrupts();
        OS_Halt();
    }
}
