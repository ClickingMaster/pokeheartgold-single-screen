#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    s8 mode;
    u8 p[3];
    void *arg;
    volatile u32 flags;
} Control;

extern Control ov13_0224F4CC;
extern void WifiSetup_SaveVramBanks(void);
extern void WifiSetupHeap_Init(void *);
extern void WifiSetup_InstallVBlankIntr(void);
extern void WifiSetupTask_InitScheduler(void);
extern void ov13_0223F664(void);
extern void WifiSetupInput_Init(void);
extern void WifiSetupFade_Init(void);
extern void DWC_BM_Init(void *);
extern void *WifiSetupHeap_AllocZeroed(u32, u32);
extern void WifiSetupHeap_FreeAndClear(void *);
extern BOOL OS_IsTickAvailable(void);
extern BOOL OS_IsAlarmAvailable(void);
extern void OS_Terminate(void);
extern void FX_Init(void);
extern void TP_Init(void);
extern void RTC_Init(void);
int ov13_0222B4BC(int, u32);
void ov13_0222B558(void);

void ov13_0222B558(void) {
    void *temp;
    (void)*(volatile u16 *)0x4000208;
    *(volatile u16 *)0x4000208 = 0;
    GX_DispOff();
    *(volatile u32 *)0x4001000 &= ~0x10000;
    if (!OS_IsTickAvailable()) {
        OS_Terminate();
    }
    if (!OS_IsAlarmAvailable()) {
        OS_Terminate();
    }
    GX_VBlankIntr(0);
    FX_Init();
    FS_Init((u32)-1);
    TP_Init();
    RTC_Init();
    GX_DispOff();
    *(volatile u32 *)0x4001000 &= ~0x10000;
    WifiSetup_SaveVramBanks();
    WifiSetupHeap_Init(ov13_0224F4CC.arg);
    WifiSetup_InstallVBlankIntr();
    WifiSetupTask_InitScheduler();
    ov13_0223F664();
    WifiSetupInput_Init();
    WifiSetupFade_Init();
    temp = WifiSetupHeap_AllocZeroed(0x700, 32);
    DWC_BM_Init(temp);
    WifiSetupHeap_FreeAndClear(&temp);
}
