#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define V(o) (*(volatile u16*)(o))
extern u8 spiWork[],tpw[],PMi_KeyPattern[];typedef struct{u16 low:16,high:16;}W35040Half;
extern u16 spiInitialized,last_touch_flg,PMi_Work[];
extern u32 OS_DisableInterrupts(void);extern void OS_RestoreInterrupts(u32),OS_SleepThread(void*),OS_WakeupThread(void*),OS_WakeupThreadDirect(void*);
extern int OS_SendMessage(void*,void*,int),OS_ReadMessage(void*,void**,int),OS_ReceiveMessage(void*,void**,int);
extern void OS_InitMessageQueue(void*,void*,int),OS_CreateThread(void*,void(*)(void*),void*,void*,u32,u32),MI_CpuFill8(void*,u8,u32);
extern void TP_Init(void),NVRAM_Init(void),MIC_Init(void),PM_Init(void),PXI_Init(void),SpiCommonThread(void*),SpiPxiCallback(u32,u32,int);
extern void PXI_SetFifoRecvCallback(int,void(*)(u32,u32,int));extern int PXI_SendWordByFifo(int,u32,int);
extern void TP_ExecuteProcess(void*),MIC_ExecuteProcess(void*),PM_ExecuteProcess(void*),NVRAM_ExecuteProcess(void*),TP_AnalyzeCommand(u32),MIC_AnalyzeCommand(u32),PM_AnalyzeCommand(u32),NVRAM_AnalyzeCommand(u32);
extern int OS_IsVAlarmAvailable(void);extern void OS_InitVAlarm(void),OS_CreateVAlarm(void*),OS_SetVAlarmTag(void*,u32);
extern void SPI_DummyWait_03800EE0(void),SPI_DummyWait_03801538(void),EXIi_SelectRcnt(u32),OS_Terminate(void);
extern int SPIi_SetEntry(u32,u32,u16,...);extern void SPIi_ReturnResult(u32,u32);
extern int TPi_DetectTouch(void),TPi_DetectPos(u16*,int,int,u16*);
extern void PM_SetLEDPattern(u32);extern u32 PM_GetLEDPattern(void);

// @fn PM_AnalyzeCommand
void PM_AnalyzeCommand(u32 word){u32 header;u16 command;int i;if(word&0x02000000)for(i=0;i<16;i++)PMi_Work[i]=0;PMi_Work[(word&0xf0000)>>16]=word;if(!(word&0x01000000))return;header=((W35040Half*)(PMi_KeyPattern+8))->low;command=(header&0xff00)>>8;switch(command){case 0x60:SPIi_ReturnResult(0x60,0);break;case 0x61:if(!SPIi_SetEntry(3,command,2,(u32)(header&255),(u32)*(u16*)(PMi_KeyPattern+10)))SPIi_ReturnResult(command,4);break;case 0x64:if(!SPIi_SetEntry(3,command,2,(u32)(header&255),(u32)*(u16*)(PMi_KeyPattern+10)))SPIi_ReturnResult(command,4);break;case 0x65:if(!SPIi_SetEntry(3,command,1,(u32)(u16)header))SPIi_ReturnResult(command,4);break;case 0x63:if(!SPIi_SetEntry(3,command,1,((header&255)<<16)|*(u16*)(PMi_KeyPattern+10)))SPIi_ReturnResult(command,4);break;case 0x66:PM_SetLEDPattern(header&255);SPIi_ReturnResult(0x66,0);break;case 0x67:SPIi_ReturnResult(0x67,(u16)PM_GetLEDPattern());break;default:SPIi_ReturnResult(command,1);break;}}
