#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct T07 {u8 pad[0x48];u32 state;struct T07*next;} T07;
typedef struct S07 {u8 state,pad[3];void*copy;u32 begin,size;} S07;
extern S07 wvrStatus;extern T07 wvrThread;extern u8 wvrWlStack[];
extern int PXI_SendWordByFifo(int,u32,int);extern u32 OS_DisableInterrupts(void);extern void OS_RestoreInterrupts(u32),OS_ExitThread(void),OS_WakeupThreadDirect(T07*),OS_CreateThread(T07*,void(*)(void*),void*,void*,u32,int);extern int WvrCheckThreadRunning(T07*);extern void WvrStartUpThread(void*),WvrTerminateThread(void*),WvrPxiWmSubstituteCallback(int,void*,int);extern void MIi_CpuClear32(u32,void*,u32),PM_SetLEDPattern(int),PMi_SetLED(int);

void WvrPxiWmSubstituteCallback(int tag,void*buf,int err);
// @fn WvrPxiWmSubstituteCallback
void WvrPxiWmSubstituteCallback(int tag,void*buf,int err){u16*p=buf;u16 code=*p;if(p){MIi_CpuClear32(0,p,0x100);p[0]=code;p[1]=4;while(PXI_SendWordByFifo(10,(u32)p,0)<0){}}}
