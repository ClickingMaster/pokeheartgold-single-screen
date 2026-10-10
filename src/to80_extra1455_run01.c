#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#pragma opt_strength_reduction off
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))
extern volatile u32 ov13_0224CF40[];
extern int MBi_IsCommSizeValid(int, int, int);
BOOL MB_SetParentCommParam(u16, u16);
extern void MBi_SetMaxScanTime(int);
extern int WM_Initialize(void *, u32, u16);
extern void WM_SetIndCallback(u32);
extern void MBi_OnInitializeDone(void);
int MBi_StartCommon(void);
extern void MB_CommSetParentStateCallback(u32);
extern void MBi_SetChildMPMaxSize(u16);
extern void MBi_SetParentPieceBuffer(void *);
extern void MBi_CommParentCallback(void);
extern void MBi_ParentCallback(void);
extern void MB_InitSendGameInfoStatus(void);
int MBi_StartParentCore(u16);

BOOL MB_SetParentCommParam(u16 parent, u16 child) {
    OSIntrMode mode = OS_DisableInterrupts();
    if (B((void *)ov13_0224CF40[2], 0x50d)) {
        OS_RestoreInterrupts(mode);
        return 0;
    }
    if (!MBi_IsCommSizeValid(parent, 8, child)) {
        OS_RestoreInterrupts(mode);
        return 0;
    }
    H((void *)ov13_0224CF40[2], 16) = child;
    H((void *)ov13_0224CF40[2], 0x500) = parent;
    H((void *)ov13_0224CF40[2], 0x502) = 8;
    OS_RestoreInterrupts(mode);
    return 1;
}

int MBi_StartCommon(void) {
    int status;
    H((void *)ov13_0224CF40[2], 0x528) = 0;
    H((void *)ov13_0224CF40[2], 0x52a) = 0;
    H((void *)ov13_0224CF40[2], 0x526) = 0;
    H((void *)ov13_0224CF40[2], 0x548) = 0;
    MBi_SetMaxScanTime(10);
    if (!W((void *)ov13_0224CF40[3], 0x1320)) {
        do {
            {
                u16 dma = H(ov13_0224CF40, 0);
                u32 *args = (u32 *)ov13_0224CF40;
                u32 buffer = args[1];
                u32 context = args[2];
                status = WM_Initialize((void *)buffer, W((void *)context, 0x508), dma);
            }
        } while (status == 4);
        if (status != 2) {
            return 8;
        }
        WM_SetIndCallback(W((void *)ov13_0224CF40[2], 0x508));
        B((void *)ov13_0224CF40[2], 0x50d) = 1;
        return 0;
    }
    WM_SetIndCallback(W((void *)ov13_0224CF40[2], 0x508));
    B((void *)ov13_0224CF40[2], 0x50d) = 1;
    MBi_OnInitializeDone();
    return 0;
}

int MBi_StartParentCore(u16 channel) {
    OSIntrMode mode = OS_DisableInterrupts();
    u8 *parent;
    u32 callback;
    int i, result;
    H((void *)ov13_0224CF40[2], 50) = channel;
    parent = (u8 *)ov13_0224CF40[3];
    ov13_0224CF40[1] = ((u32)parent + 0x7d1f) & ~31;
    callback = W(parent, 0x14e4);
    MIi_CpuClear16(0, (u16 *)(parent + 0x1340), 0x69c0);
    MB_CommSetParentStateCallback(callback);
    W((void *)ov13_0224CF40[3], 0x1318) = H((void *)ov13_0224CF40[2], 0x500) - 6;
    MBi_SetChildMPMaxSize(H((void *)ov13_0224CF40[2], 0x502));
    MBi_SetParentPieceBuffer((u8 *)ov13_0224CF40[3] + 0x1538);
    for (i = 0; i < 15; i++) {
        W((u8 *)ov13_0224CF40[3] + i * 4, 0x14e8) = 0;
        *(s8 *)((u8 *)ov13_0224CF40[3] + i + 0x1526) = -1;
    }
    B((void *)ov13_0224CF40[3], 0x1524) = 0;
    MIi_CpuClear16(0, (u16 *)((u8 *)ov13_0224CF40[3] + 0x1788), 0x5d40);
    MI_CpuFill8((u8 *)ov13_0224CF40[3] + 0x1754, 0, 30);
    H((void *)ov13_0224CF40[2], 0x524) = 1;
    W((void *)ov13_0224CF40[2], 0x51c) = (u32)MBi_CommParentCallback;
    W((void *)ov13_0224CF40[2], 0x508) = (u32)MBi_ParentCallback;
    {
        u8 *c = (u8 *)ov13_0224CF40[2];
        H(c, 0x34) = H(c, 0x500);
    }
    {
        u8 *c = (u8 *)ov13_0224CF40[2];
        H(c, 0x518) = (H(c, 0x34) + 35) & ~31;
    }
    {
        u8 *c = (u8 *)ov13_0224CF40[2];
        H(c, 0x36) = H(c, 0x502);
    }
    {
        u8 *c = (u8 *)ov13_0224CF40[2];
        H(c, 0x51a) = ((15 * (H(c, 0x36) + 14) + 41) & ~31) * 2;
    }
    MB_InitSendGameInfoStatus();
    result = MBi_StartCommon();
    OS_RestoreInterrupts(mode);
    W((void *)ov13_0224CF40[3], 0x74c8) = PXI_IsCallbackReady(15, PXI_PROC_ARM7);
    return result;
}
