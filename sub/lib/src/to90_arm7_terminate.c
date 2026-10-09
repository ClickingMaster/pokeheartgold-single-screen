#include <nitro.h>

// SVC_Halt is Thumb; route this ARM call through the explicit veneer defined in
// sp_main.s so the linker does not emit a second interworking veneer.
void __Veneer_SVC_Halt(void);

void OS_Terminate(void) {
#ifdef SDK_ARM7
    CTRDG_VibPulseEdgeUpdate(0);
#endif
    while (1) {
        OS_DisableInterrupts();
        __Veneer_SVC_Halt();
    }
}
