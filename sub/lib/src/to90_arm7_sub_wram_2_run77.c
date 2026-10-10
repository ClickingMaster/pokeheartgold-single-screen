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

void TP_AnalyzeCommand(u32 word) {
    u16 command;
    int i;
    if (word & 0x02000000) {
        for (i = 0; i < 16; i++) {
            ((u16 *)tpw)[i] = 0;
        }
    }
    ((u16 *)tpw)[(word & 0xf0000) >> 16] = word;
    if (!(word & 0x01000000)) {
        return;
    }
    command = (((u16 *)tpw)[0] & 0xff00) >> 8;
    switch (command) {
    case 3: {
        u16 low = (u16)(((u16 *)tpw)[0] & 255);
        if (!low) {
            SPIi_ReturnResult(3, 2);
        } else {
            W(tpw, 36) = low;
            W(tpw, 40) = low;
            SPIi_ReturnResult(3, 0);
        }
        break;
    }
    case 0:
        if (!SPIi_SetEntry(0, command, 0)) {
            SPIi_ReturnResult(command, 4);
        }
        break;
    case 1: {
        u16 low;
        if (W(tpw, 32)) {
            SPIi_ReturnResult(command, 3);
            break;
        }
        low = (u16)(((u16 *)tpw)[0] & 255);
        if (!low || low > 4) {
            SPIi_ReturnResult(command, 2);
            break;
        }
        if (((u16 *)tpw)[1] >= 263) {
            SPIi_ReturnResult(command, 2);
            break;
        }
        if (SPIi_SetEntry(0, command, 2, (u32)low, (u32)((u16 *)tpw)[1])) {
            W(tpw, 32) = 1;
        } else {
            SPIi_ReturnResult(command, 4);
        }
        break;
    }
    case 2:
        if (W(tpw, 32) != 2) {
            SPIi_ReturnResult(command, 3);
            break;
        }
        if (SPIi_SetEntry(0, command, 0)) {
            W(tpw, 32) = 3;
        } else {
            SPIi_ReturnResult(command, 4);
        }
        break;
    default:
        SPIi_ReturnResult(command, 1);
        break;
    }
}
