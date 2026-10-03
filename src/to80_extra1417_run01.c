#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {s8 mode;u8 p[3];void*arg;volatile u32 flags;}Control;
extern Control ov13_0224F4CC;
extern void ov13_022420A0(void),ov13_022411DC(void*),ov13_02241AC0(void),ov13_0224231C(void),ov13_0223F664(void),ov13_02241330(void),ov13_02240338(void),DWC_BM_Init(void*);
extern void*ov13_0224128C(u32,u32);extern void ov13_022412B0(void*);
extern BOOL OS_IsTickAvailable(void),OS_IsAlarmAvailable(void);extern void OS_Terminate(void),FX_Init(void),TP_Init(void),RTC_Init(void);
int ov13_0222B4BC(int,u32);void ov13_0222B558(void);
void ov13_0222B558(void){void*temp;(void)*(volatile u16*)0x4000208;*(volatile u16*)0x4000208=0;GX_DispOff();*(volatile u32*)0x4001000&=~0x10000;if(!OS_IsTickAvailable())OS_Terminate();if(!OS_IsAlarmAvailable())OS_Terminate();GX_VBlankIntr(0);FX_Init();FS_Init((u32)-1);TP_Init();RTC_Init();GX_DispOff();*(volatile u32*)0x4001000&=~0x10000;ov13_022420A0();ov13_022411DC(ov13_0224F4CC.arg);ov13_02241AC0();ov13_0224231C();ov13_0223F664();ov13_02241330();ov13_02240338();temp=ov13_0224128C(0x700,32);DWC_BM_Init(temp);ov13_022412B0(&temp);}
