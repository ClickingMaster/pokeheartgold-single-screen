#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma dollar_identifiers on

typedef struct {
    u32 status, pad, id, source, destination, length;
    u8 p[0x58 - 0x18];
    u32 supported;
} Request;

typedef struct Common {
    Request *request;
    u32 command;
    volatile u32 received;
    u8 p[0x3c - 12];
    void (*callback)(void *);
    void *argument;
    void (*task)(struct Common *);
    u8 thread[0xec - 0x48];
    void *wake;
    u32 pad;
    u32 queue[2];
    volatile u32 flags;
} Common;

extern Common cardi_common;
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32);
extern void OS_SleepThread(void *);
extern void OS_WakeupThread(void *);
extern void OS_WakeupThreadDirect(void *);
extern void CARDi_WaitTask(Common *, void (*)(void *), void *), CARDi_ReadEnd(void);
extern u32 CARDi_ReadRomIDCore(void);
u32 CARDi_ReadRomID(void);

u32 CARDi_ReadRomID(void) {
    u32 id;
    CARDi_WaitTask(&cardi_common, 0, 0);
    id = CARDi_ReadRomIDCore();
    CARDi_ReadEnd();
    return id;
}
