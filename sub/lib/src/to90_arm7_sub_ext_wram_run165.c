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
extern int BBP_Write(u32, u32);
void InitBaseBand(void);

void InitBaseBand(void) {
    u32 value;
    u32 i;
    u32 address;
    IO(0x04808160) = 0x100;
    value = 0;
    i = 0;
    address = 100;
    for (; i < 105; i++, address++) {
        FLASH_Read(address, 1, &value);
        BBP_Write(i, value);
    }
    BBP_Write(0x5a, 2);
}
