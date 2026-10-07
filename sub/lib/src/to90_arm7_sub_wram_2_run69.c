#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define V(o) (*(volatile u16*)(o))
extern u8 spiWork[],tpw[],PMi_KeyPattern[];
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
extern void PM_SetLEDPattern(u32);extern u16 PM_GetLEDPattern(void);
void SPI_Init(u32 priority);
// @fn SPI_Init
void SPI_Init(u32 priority){int i;if(spiInitialized)return;W(spiWork,0)=0;spiInitialized=1;W(spiWork,4)=5;TP_Init();NVRAM_Init();MIC_Init();PM_Init();PXI_Init();PXI_SetFifoRecvCallback(6,SpiPxiCallback);PXI_SetFifoRecvCallback(9,SpiPxiCallback);PXI_SetFifoRecvCallback(8,SpiPxiCallback);PXI_SetFifoRecvCallback(4,SpiPxiCallback);OS_InitMessageQueue(spiWork+0x2ac,spiWork+0x2cc,16);for(i=0;i<16;i++)MI_CpuFill8(spiWork+0x30c+i*24,0,24);W(spiWork,0x48c)=0;W(spiWork,0x494)=0;W(spiWork,0x490)=0;OS_CreateThread(spiWork+8,SpiCommonThread,0,spiWork+0x2ac,512,priority);OS_WakeupThreadDirect(spiWork+8);}
