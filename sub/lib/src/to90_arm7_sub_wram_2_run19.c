#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 owned, irq;
} Lock;

extern u32 OS_DisableInterrupts(void);
extern u32 OS_ReadOwnerOfLockWord(void *);
extern int OS_TryLockCartridge(u32);
int CTRDGi_LockByProcessor(u32 id, Lock *l);

int CTRDGi_LockByProcessor(u32 id, Lock *l) {
    l->irq = OS_DisableInterrupts();
    l->owned = OS_ReadOwnerOfLockWord((void *)0x27fffe8) & 128;
    if (l->owned || !OS_TryLockCartridge(id)) {
        return 1;
    }
    return 0;
}
