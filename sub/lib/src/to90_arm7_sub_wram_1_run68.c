#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 sCommandMesgQueue[];
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32);
extern int OS_SendMessage(void *, void *, int);
extern void SND_SendWakeupMessage(void);
void PxiFifoCallback(int tag, u32 data, int err);

void PxiFifoCallback(int tag, u32 data, int err) {
    u32 irq = OS_DisableInterrupts();
    if (data >= 0x02000000) {
        OS_SendMessage(sCommandMesgQueue, (void *)data, 0);
    } else if (!data) {
        SND_SendWakeupMessage();
    }
    OS_RestoreInterrupts(irq);
}
