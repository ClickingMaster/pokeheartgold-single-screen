#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void __VENEER_MIi_CpuClearFast(u32, void *, u32), InitializeHeapBuf(void *), FLASH_MakeImage(void), FLASH_Read(u32, u32, void *), InitializeParam(u32, u32), InitializeTask(void), InitializeCmdIf(void), InitializeMLME(void), InitializeCAM(void), InitializeAlarm(void), WConfigDevice(void), DiagMacRegister(void), WWakeUp(void), InitMac(void), InitRF(void), DiagMacMemory(void), DiagBaseBand(void), InitBaseBand(void), WSetDefaultParameters(void), WShutdown(void), InitializeIntr(void), MainTaskRoutine(void *), __VENEER_OS_CreateThread(void *, void (*)(void *), void *, void *, u32, u32), __VENEER_OS_WakeupThreadDirect(void *);
extern u32 __VENEER_OS_GetLockID(void);
extern u32 __VENEER__u32_div_f(u32, u32);
extern u32 FLASH_VerifyCheckSum(u32 *);
#define P      (*(u8 *volatile *)0x380fff4)
#define U32(o) (*(u32 *)(P + (o)))
#define U16(o) (*(u16 *)(P + (o)))
u16 WL_InitDriver(u32 *r);

u16 WL_InitDriver(u32 *r) {
    u32 check;
    u8 *initial = (u8 *)r[0];
    *(u8 *volatile *)0x380fff4 = initial;
    __VENEER_MIi_CpuClearFast(0, initial, 0x694);
    U32(0x314) = __VENEER_OS_GetLockID();
    U32(0x30c) = r[6];
    U32(0x310) = r[7] >> 1;
    {
        u8 *work = P;
        if (*(u32 *)(work + 0x310) == 0) {
            *(u32 *)(work + 0x310) = 0xffffffff;
        }
    }
    InitializeHeapBuf(r + 8);
    FLASH_MakeImage();
    *(volatile u16 *)0x4000304 |= 2;
    *(volatile u16 *)0x4000206 = 48;
    U32(0x68c) = 3;
    FLASH_Read(60, 2, P + 0x692);
    FLASH_Read(62, 2, P + 0x690);
    U32(0x308) = r[4];
    U32(0x304) = r[5];
    {
        u32 x = __VENEER__u32_div_f(r[12], 28);
        InitializeParam(r[11], x);
    }
    InitializeTask();
    InitializeCmdIf();
    InitializeMLME();
    InitializeCAM();
    InitializeAlarm();
    if (FLASH_VerifyCheckSum(&check)) {
        U16(0x33e) |= 128;
    } else {
        WConfigDevice();
        DiagMacRegister();
        WWakeUp();
        InitMac();
        InitRF();
        DiagMacMemory();
        DiagBaseBand();
        InitBaseBand();
        WSetDefaultParameters();
        WShutdown();
    }
    __VENEER_OS_CreateThread(P + 24, MainTaskRoutine, 0, (void *)r[1], r[2], r[3]);
    __VENEER_OS_WakeupThreadDirect(P + 24);
    InitializeIntr();
    return U16(0x33e);
}
