#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct T07 {u8 pad[0x48];u32 state;struct T07*next;} T07;
typedef struct S07 {u8 state,pad[3];void*copy;u32 begin,size;} S07;
extern S07 wvrStatus;extern T07 wvrThread;extern u8 wvrWlStack[];
extern int PXI_SendWordByFifo(int,u32,int);extern u32 OS_DisableInterrupts(void);extern void OS_RestoreInterrupts(u32),OS_ExitThread(void),OS_WakeupThreadDirect(T07*),OS_CreateThread(T07*,void(*)(void*),void*,void*,u32,int);extern int WvrCheckThreadRunning(T07*);extern void WvrStartUpThread(void*),WvrTerminateThread(void*),WvrPxiWmSubstituteCallback(int,void*,int);extern void MIi_CpuClear32(u32,void*,u32),PM_SetLEDPattern(int),PMi_SetLED(int);
extern void MIi_CpuCopyFast(const void*,void*,u32),WvrBegin(u32);
void WvrStartUpThread(void*arg);
// @fn WvrStartUpThread
void WvrStartUpThread(void*arg){if(wvrStatus.copy)MIi_CpuCopyFast(wvrStatus.copy,(void*)0x6000000,wvrStatus.size);WvrBegin(wvrStatus.begin);while(PXI_SendWordByFifo(15,0x10000,0)<0){}OS_DisableInterrupts();wvrStatus.state=3;OS_ExitThread();}
