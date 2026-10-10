#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void ClearTimeOut(void);
extern void WShutdown(void);
extern void InitMac(void);
extern void ReleaseAllWlHeapBuf(void);
extern void InitializeTask(void);
extern void InitializeParam(u32, u16);
extern void InitializeCmdIf(void);
extern void InitializeMLME(void);
extern void InitializeCAM(void);
extern void WSetDefaultParameters(void);
void WlessLibReboot(void);

void WlessLibReboot(void) {
    u8 *p;
    ClearTimeOut();
    WShutdown();
    InitMac();
    ReleaseAllWlHeapBuf();
    InitializeTask();
    p = *(u8 *volatile *)0x380fff4;
    InitializeParam(*(u32 *)(p + 0x31c), *(u16 *)(p + 0x320));
    InitializeCmdIf();
    InitializeMLME();
    InitializeCAM();
    WSetDefaultParameters();
}
