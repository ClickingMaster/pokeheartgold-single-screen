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
extern void WSetStaState(u16);
extern void StopBeaconFrame(void);
extern void DeleteAllTxFrames(void);
extern void ReleaseAllWlHeapBuf(void);
void WStop(void);
// @fn WStop
void WStop(void){u8*q=ROOT+0x344;__VENEER_OS_CancelAlarm(ROOT+0x608);__VENEER_OS_CancelAlarm(ROOT+0x634);WSetStaState(32);H(q,0xa4)=0;H(q,0x12)=0;IO(0x04808012)=0;IO(0x04808004)=0;IO(0x048080ea)=0;IO(0x048080e8)=0;IO(0x04808008)=0;IO(0x0480800a)=0;if(H(q,0xc)==1)StopBeaconFrame();IO(0x048080ac)=0xffff;IO(0x048080b4)=0xffff;DeleteAllTxFrames();ReleaseAllWlHeapBuf();}
