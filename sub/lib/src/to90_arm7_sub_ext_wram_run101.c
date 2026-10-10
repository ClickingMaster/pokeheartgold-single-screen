#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int __VENEER_OS_ReceiveMessage(void *, void **, int);
extern void ExecuteMessage(void **);
extern void AddTask(u32, u32);
void LowestIdleTask(void);

void LowestIdleTask(void) {
    void *message;
    __VENEER_OS_ReceiveMessage(*(void **)(*(u8 *volatile *)0x380fff4 + 0x308), &message, 1);
    ExecuteMessage(&message);
    AddTask(3, 12);
}
