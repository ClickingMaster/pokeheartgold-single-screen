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
void WUpdateCounter(void);
// @fn WUpdateCounter
void WUpdateCounter(void){u8*p=ROOT;volatile u16*r=(volatile u16*)0x048081b0;int v;v=r[0];W(p,1420)+=(v&255);v=r[1];W(p,1416)+=(v>>8);W(p,1432)+=(v&255);v=r[2];W(p,1428)+=(v>>8);W(p,1424)+=(v&255);v=r[3];W(p,1436)+=(v>>8);W(p,1396)+=(v&255);v=r[4];W(p,1412)+=(v&255);v=r[5];W(p,1372)+=(v&255);v=r[6];W(p,1388)+=(v>>8);W(p,1408)+=(v&255);v=r[7];W(p,1400)+=(v&255);W(p,1404)+=(v>>8);v=r[8];W(p,1352)+=(v&255);v=r[16];W(p,1460)+=(v>>8);v=r[17];W(p,1464)+=(v&255);W(p,1468)+=(v>>8);v=r[18];W(p,1472)+=(v&255);W(p,1476)+=(v>>8);v=r[19];W(p,1480)+=(v&255);W(p,1484)+=(v>>8);v=r[20];W(p,1488)+=(v&255);W(p,1492)+=(v>>8);v=r[21];W(p,1496)+=(v&255);W(p,1500)+=(v>>8);v=r[22];W(p,1504)+=(v&255);W(p,1508)+=(v>>8);v=r[23];W(p,1512)+=(v&255);W(p,1516)+=(v>>8);}
