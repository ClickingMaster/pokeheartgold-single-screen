#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int __VENEER_OS_IsThreadTerminated(void *);
extern void AddTask(u32, u32);
extern void __VENEER_OS_JoinThread(void *);
void WL_Terminate(void);

void WL_Terminate(void) {
    u8 *p = *(u8 *volatile *)0x380fff4;
    if (__VENEER_OS_IsThreadTerminated(p + 24) != 1) {
        AddTask(2, 22);
        __VENEER_OS_JoinThread(p + 24);
        while (!__VENEER_OS_IsThreadTerminated(p + 24)) {}
    }
}
