#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern void ClearTimeOut(void),WShutdown(void),InitMac(void),ReleaseAllWlHeapBuf(void),InitializeTask(void),InitializeParam(u32,u16),InitializeCmdIf(void),InitializeMLME(void),InitializeCAM(void),WSetDefaultParameters(void);
void WlessLibReboot(void);
// @fn WlessLibReboot
void WlessLibReboot(void){u8*p;ClearTimeOut();WShutdown();InitMac();ReleaseAllWlHeapBuf();InitializeTask();p=*(u8*volatile*)0x380fff4;InitializeParam(*(u32*)(p+0x31c),*(u16*)(p+0x320));InitializeCmdIf();InitializeMLME();InitializeCAM();WSetDefaultParameters();}
