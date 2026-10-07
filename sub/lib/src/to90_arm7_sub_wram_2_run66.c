#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct T07 {u8 pad[0x48];u32 state;struct T07*next;} T07;
typedef struct S07 {u8 state,pad[3];void*copy;u32 begin,size;} S07;
extern S07 wvrStatus;extern T07 wvrThread;extern u8 wvrWlStack[];
extern int PXI_SendWordByFifo(int,u32,int);extern u32 OS_DisableInterrupts(void);extern void OS_RestoreInterrupts(u32),OS_ExitThread(void),OS_WakeupThreadDirect(T07*),OS_CreateThread(T07*,void(*)(void*),void*,void*,u32,int);extern int WvrCheckThreadRunning(T07*);extern void WvrStartUpThread(void*),WvrTerminateThread(void*),WvrPxiWmSubstituteCallback(int,void*,int);extern void MIi_CpuClear32(u32,void*,u32),PM_SetLEDPattern(int),PMi_SetLED(int);
extern struct {u32 pad[2];T07*head;} OSi_ThreadInfo;
int WvrCheckThreadRunning(T07*t);
// @fn WvrCheckThreadRunning
int WvrCheckThreadRunning(T07*t){T07*p;if(!t)return 0;for(p=OSi_ThreadInfo.head;p;p=p->next){if(p==t){switch(t->state){case 0:case 1:return 1;case 2:default:return 0;}}}return 0;}
