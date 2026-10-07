#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct T07 {u8 pad[0x48];u32 state;struct T07*next;} T07;
typedef struct S07 {u8 state,pad[3];void*copy;u32 begin,size;} S07;
extern S07 wvrStatus;extern T07 wvrThread;extern u8 wvrWlStack[];
extern int PXI_SendWordByFifo(int,u32,int);extern u32 OS_DisableInterrupts(void);extern void OS_RestoreInterrupts(u32),OS_ExitThread(void),OS_WakeupThreadDirect(T07*),OS_CreateThread(T07*,void(*)(void*),void*,void*,u32,int);extern int WvrCheckThreadRunning(T07*);extern void WvrStartUpThread(void*),WvrTerminateThread(void*),WvrPxiWmSubstituteCallback(int,void*,int);extern void MIi_CpuClear32(u32,void*,u32),PM_SetLEDPattern(int),PMi_SetLED(int);
extern void PXI_SetFifoRecvCallback(int,void(*)(int,void*,int));
void WvrPxiReceiveCallback(int tag,u32 word,int err);
// @fn WvrPxiReceiveCallback
void WvrPxiReceiveCallback(int tag,u32 word,int err){u32 result;int ok;u32 irq;switch(word){case 0x10000:irq=OS_DisableInterrupts();if(WvrCheckThreadRunning(&wvrThread)){OS_RestoreInterrupts(irq);ok=0;}else if(wvrStatus.state!=1){OS_RestoreInterrupts(irq);ok=0;}else{wvrStatus.state=2;OS_RestoreInterrupts(irq);OS_CreateThread(&wvrThread,WvrStartUpThread,0,wvrWlStack,0x400,10);OS_WakeupThreadDirect(&wvrThread);ok=1;}if(ok)return;result=word|5;break;case 0x20000:irq=OS_DisableInterrupts();if(WvrCheckThreadRunning(&wvrThread)){OS_RestoreInterrupts(irq);ok=0;}else if(wvrStatus.state!=3){OS_RestoreInterrupts(irq);ok=0;}else{wvrStatus.state=4;OS_RestoreInterrupts(irq);PXI_SetFifoRecvCallback(10,WvrPxiWmSubstituteCallback);OS_CreateThread(&wvrThread,WvrTerminateThread,0,wvrWlStack,0x400,2);OS_WakeupThreadDirect(&wvrThread);ok=1;}if(ok)return;result=word|5;break;default:result=word|7;break;}while(PXI_SendWordByFifo(15,result,0)<0){}}
