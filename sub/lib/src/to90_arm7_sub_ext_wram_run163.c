#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define ROOT (*(u8 **)(0x0380fff4))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define IO(a) (*(volatile u16*)(a))
extern u32 WL_ReadByte(const void*);
extern void FLASH_Read(u32,u32,void*);
extern int WaitLoop_BbpAccess(void);
extern int WaitLoop_RfAccess(void);
extern void __VENEER_OS_CancelAlarm(void*);
extern void OS_CancelAlarm(void*);
extern int __VENEER_OS_IsAlarmAvailable(void);
extern void __VENEER_OS_CreateAlarm(void*);
extern u64 __VENEER_OS_GetTick(void);
typedef void (*Callback)(void*);
extern void __VENEER_OS_SetAlarm(void*,u64,Callback,void*);
extern void __VENEER_OS_SetPeriodicAlarm(void*,u64,u64,Callback,void*);
extern u64 __VENEER__ll_udiv(u64,u64);
extern void AddTask(u32,u32);
int RF_Write(u32 value);
// @fn RF_Write
int RF_Write(u32 value){IO(0x0480817e)=value;IO(0x0480817c)=value>>16;return WaitLoop_RfAccess();}
