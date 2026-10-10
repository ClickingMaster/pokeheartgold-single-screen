#pragma force_active on
#pragma opt_propagation off
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35240Card {
    u8 pad[72];
    u8 thread[168];
    int priority;
} W35240Card;

extern W35240Card cardi_common;
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32);
extern int OS_SetThreadPriority(void *, int);
int CARD_SetThreadPriority(int p);

int CARD_SetThreadPriority(int p) {
    W35240Card *c = &cardi_common;
    u32 irq = OS_DisableInterrupts();
    int old = c->priority;
    c->priority = p;
    OS_SetThreadPriority(c->thread, p);
    OS_RestoreInterrupts(irq);
    return old;
}
