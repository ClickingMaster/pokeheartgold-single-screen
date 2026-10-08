#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#pragma dollar_identifiers on
typedef struct {
    u32 x0;
    u32 x4;
    u32 x8;
    volatile int result;
    u32 x10;
    u32 x14;
    u32 x18;
    u32 x1c;
    u8 pad20[0x48 - 0x20];
    u8 thread[0xf0 - 0x48];
    volatile u32 priority;
    u32 xf4;
    u32 xf8;
} CardiCommon;
extern CardiCommon cardi_common;
extern u8 status_checked$3825[];
extern BOOL CARDi_EnableFlag;
extern void CARDi_TaskThread(void *);
extern void CARDi_OnFifoRecv(int, u32, BOOL);
extern void OS_CreateThread(void *, void (*)(void *), void *, void *, u32, u32);
extern void OS_WakeupThreadDirect(void *);
extern void PXI_SetFifoRecvCallback(int, void (*)(int, u32, BOOL));
void CARDi_InitCommon(void);
static inline BOOL CardIsChecked(void)
{
    return *(vu16 *)0x027ffc40 == 2;
}
#pragma opt_propagation off
// @fn CARDi_InitCommon
void CARDi_InitCommon(void)
{
    CardiCommon *p = &cardi_common;
    p->result = -3;
    p->priority = 4;
    p->x10 = 0;
    p->x1c = 0;
    p->x0 = 0;
    p->x8 = 0;
    p->x18 = 0;
    p->x14 = 0;
    p->xf8 = 0;
    p->xf4 = 0;
    OS_CreateThread(p->thread, CARDi_TaskThread, NULL, status_checked$3825, 0x400, p->priority);
    OS_WakeupThreadDirect(p->thread);
    PXI_SetFifoRecvCallback(11, CARDi_OnFifoRecv);
    if (!CardIsChecked())
        CARDi_EnableFlag = TRUE;
}
#pragma opt_propagation reset
