#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define V(o)    (*(volatile u16 *)(o))
extern u8 spiWork[];
extern u8 tpw[];
extern u8 PMi_KeyPattern[];
extern u16 spiInitialized;
extern u16 last_touch_flg;
extern u16 PMi_Work[];
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32);
extern void OS_SleepThread(void *);
extern void OS_WakeupThread(void *);
extern void OS_WakeupThreadDirect(void *);
extern int OS_SendMessage(void *, void *, int);
extern int OS_ReadMessage(void *, void **, int);
extern int OS_ReceiveMessage(void *, void **, int);
extern void OS_InitMessageQueue(void *, void *, int), OS_CreateThread(void *, void (*)(void *), void *, void *, u32, u32), MI_CpuFill8(void *, u8, u32);
extern void TP_Init(void);
extern void NVRAM_Init(void);
extern void MIC_Init(void);
extern void PM_Init(void);
extern void PXI_Init(void);
extern void SpiCommonThread(void *);
extern void SpiPxiCallback(u32, u32, int);
extern void PXI_SetFifoRecvCallback(int, void (*)(u32, u32, int));
extern int PXI_SendWordByFifo(int, u32, int);
extern void TP_ExecuteProcess(void *);
extern void MIC_ExecuteProcess(void *);
extern void PM_ExecuteProcess(void *);
extern void NVRAM_ExecuteProcess(void *);
extern void TP_AnalyzeCommand(u32);
extern void MIC_AnalyzeCommand(u32);
extern void PM_AnalyzeCommand(u32);
extern void NVRAM_AnalyzeCommand(u32);
extern int OS_IsVAlarmAvailable(void);
extern void OS_InitVAlarm(void);
extern void OS_CreateVAlarm(void *);
extern void OS_SetVAlarmTag(void *, u32);
extern void SPI_DummyWait_03800EE0(void);
extern void SPI_DummyWait_03801538(void);
extern void EXIi_SelectRcnt(u32);
extern void OS_Terminate(void);
extern int SPIi_SetEntry(u32, u32, u16, ...);
extern void SPIi_ReturnResult(u32, u32);
extern int TPi_DetectTouch(void);
extern int TPi_DetectPos(u16 *, int, int, u16 *);
extern void PM_SetLEDPattern(u32);
extern u16 PM_GetLEDPattern(void);

void PM_Init(void) {
    int i;
    W(PMi_KeyPattern, 4) = 1;
    W(PMi_KeyPattern, 40) = 0;
    for (i = 0; i < 16; i++) {
        PMi_Work[i] = 0;
    }
}
