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

typedef union {
    u32 word;

    struct {
        u32 x : 12, y : 12, touch : 1, invalid : 2, rest : 5;
    } b;
} W35038Sample;

void TP_ExecSampling(W35038Sample *, int, u16 *);

void TP_ExecSampling(W35038Sample *sample, int range, u16 *spread) {
    int touch, i;
    u16 value, dx, dy;
    u32 magnitude = (u32)range;
    *spread = 0;
    if (range < 0) {
        magnitude = 0 - magnitude;
    }
    touch = TPi_DetectTouch();
    if (!touch) {
        sample->b.x = 0;
        sample->b.y = 0;
        sample->b.touch = 0;
        sample->b.invalid = 3;
        last_touch_flg = 0;
        return;
    }
    sample->b.invalid = TPi_DetectPos(&value, (int)magnitude, 0, &dx);
    sample->b.x = value;
    if (TPi_DetectPos(&value, (int)magnitude, 1, &dy) == 2) {
        sample->b.invalid |= 2;
    }
    sample->b.y = value;
    V(0x040001c0) = 0x8a01;
    for (i = 0; i < 12; i++) {
        SPI_DummyWait_03801538();
    }
    V(0x040001c0) = 0x8201;
    SPI_DummyWait_03801538();
    if (touch == 2) {
        sample->b.invalid = 3;
    }
    switch (TPi_DetectTouch()) {
    case 2:
        sample->b.touch = 1;
        sample->b.invalid = 3;
        last_touch_flg = 0;
        break;
    case 1:
        sample->b.touch = 1;
        last_touch_flg = 1;
        *spread = dx >= dy ? dx : dy;
        break;
    case 0:
        sample->b.touch = 0;
        last_touch_flg = 0;
        break;
    default:
        OS_Terminate();
        break;
    }
}
