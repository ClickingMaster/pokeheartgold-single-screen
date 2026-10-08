#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
typedef struct {u16 samples[16];u32 state;u16 command,mode;u8 *buffer;u32 offset,size;u16 timer,clock,partial,last;} W35056Mic;extern W35056Mic micw;extern u32 OS_DisableInterrupts(void);extern void OS_RestoreInterrupts(u32),OS_EnableIrqMask(u32),MIC_SetIrqFunction(u32,void(*)(void)),MIC_TimerHandler(void),MIC_EnableMultipleInterrupt(void),MIC_DisableMultipleInterrupt(void),SPIi_GetException(int),SPIi_ReleaseException(int),SPIi_ReturnResult(u16,u32);extern int SPIi_CheckException(int),SPIi_CheckEntry(void),SPIi_SetEntry(int,int,int);extern u16 MIC_ExecSampling8(void),MIC_ExecSampling12(void);
typedef struct{u32 reserved,command,flags;} W40756Entry;
void MIC_ExecuteProcess(W40756Entry *entry);
static inline void W40756Publish(u16 value){*(u16*)0x027fff94=value;*(u32*)0x027fff90=0x027fff94;}
// @fn MIC_ExecuteProcess
void MIC_ExecuteProcess(W40756Entry *entry){switch(entry->command){case 0x40:{u32 irq=OS_DisableInterrupts();u16 value;if(!SPIi_CheckException(2)){OS_RestoreInterrupts(irq);SPIi_ReturnResult((u16)entry->command,4);break;}SPIi_GetException(2);OS_RestoreInterrupts(irq);if((entry->flags&1)==1){value=MIC_ExecSampling12();if(entry->flags&2)value^=0x8000;W40756Publish(value);}else{value=MIC_ExecSampling8();if(entry->flags&2)value^=0x80;W40756Publish(value);}SPIi_ReturnResult((u16)entry->command,0);SPIi_ReleaseException(2);break;}case 0x41:if(micw.state==1){u32 irq;micw.last=0;micw.partial=0;irq=OS_DisableInterrupts();OS_EnableIrqMask(64);MIC_SetIrqFunction(64,MIC_TimerHandler);MIC_EnableMultipleInterrupt();H(0x0400010c)=micw.timer;H(0x0400010e)=micw.clock|0xc0;OS_RestoreInterrupts(irq);SPIi_ReturnResult((u16)entry->command,0);micw.state=2;}else SPIi_ReturnResult((u16)entry->command,3);break;case 0x42:if(micw.state-3u<=1){u32 irq;H(0x0400010e)&=~128u;irq=OS_DisableInterrupts();MIC_SetIrqFunction(64,0);MIC_DisableMultipleInterrupt();OS_RestoreInterrupts(irq);if(micw.state==3)SPIi_ReturnResult(0x42,0);else SPIi_ReturnResult(0x51,0);micw.state=0;}else{if(micw.state==3)SPIi_ReturnResult(0x42,3);else SPIi_ReturnResult(0x51,3);}break;}}
