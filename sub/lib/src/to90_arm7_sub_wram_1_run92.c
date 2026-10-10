#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma dollar_identifiers on
extern volatile u32 skipCheck$3668[];
extern volatile u32 nextCount$3668[];
extern void PXI_Init(void), PXI_SetFifoRecvCallback(u32, void (*)(u32, u32, int));
extern int PXI_IsCallbackReady(u32, u32);
extern void CARDi_CallbackForPulledOut(u32, u32, int);
void CARD_InitPulledOutCallback(void);

void CARD_InitPulledOutCallback(void) {
    if (!skipCheck$3668[2]) {
        skipCheck$3668[2] = 1;
        PXI_Init();
        while (!PXI_IsCallbackReady(14, 0)) {}
        PXI_SetFifoRecvCallback(14, CARDi_CallbackForPulledOut);
    }
}
