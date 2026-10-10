#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 sCommandMesgQueue[];
extern u8 sCommandMesgBuffer[];
extern void *SNDi_SharedWork;
extern void OS_InitMessageQueue(void *, void *, int), PxiFifoCallback(int, u32, int), PXI_SetFifoRecvCallback(int, void (*)(int, u32, int));
void SND_CommandInit(void);

void SND_CommandInit(void) {
    OS_InitMessageQueue(sCommandMesgQueue, sCommandMesgBuffer, 8);
    PXI_SetFifoRecvCallback(7, PxiFifoCallback);
    SNDi_SharedWork = 0;
}
