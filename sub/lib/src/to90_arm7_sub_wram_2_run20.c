#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 owned, irq;
} Lock;

extern void OS_UnLockCartridge(u32);
extern void OS_RestoreInterrupts(u32);
void CTRDGi_UnlockByProcessor(u32 id, Lock *l);

void CTRDGi_UnlockByProcessor(u32 id, Lock *l) {
    if (!l->owned) {
        OS_UnLockCartridge(id);
    }
    OS_RestoreInterrupts(l->irq);
}
