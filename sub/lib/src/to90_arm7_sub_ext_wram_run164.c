#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define ROOT    (*(u8 **)(0x0380fff4))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define IO(a)   (*(volatile u16 *)(a))
extern u32 WL_ReadByte(const void *);
extern void FLASH_Read(u32, u32, void *);
extern int WaitLoop_BbpAccess(void);
extern int WaitLoop_RfAccess(void);
extern void __VENEER_OS_CancelAlarm(void *);
extern void OS_CancelAlarm(void *);
extern int __VENEER_OS_IsAlarmAvailable(void);
extern void __VENEER_OS_CreateAlarm(void *);
extern u64 __VENEER_OS_GetTick(void);
typedef void (*Callback)(void *);
extern void __VENEER_OS_SetAlarm(void *, u64, Callback, void *);
extern void __VENEER_OS_SetPeriodicAlarm(void *, u64, u64, Callback, void *);
extern u64 __VENEER__ll_udiv(u64, u64);
extern void AddTask(u32, u32);
extern void __VENEER_MIi_CpuClear16(u32, void *, u32);
void WConfigDevice(void);

void WConfigDevice(void) {
    u8 *p = ROOT + 0x5f8;
    __VENEER_MIi_CpuClear16(0, p, 16);
    FLASH_Read(0x40, 1, p);
    FLASH_Read(0x41, 1, p + 2);
    FLASH_Read(0x42, 1, p + 4);
    FLASH_Read(0x43, 1, p + 6);
}
