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

// @fn TPi_DetectPos
int TPi_DetectPos(u16*out,int range,int axis,u16*spread)
{
    int command;
    u16 *spreadOut = spread;
    int error;
    s32 i;
    s32 j;
    s32 k;
    int max;
    int values [ 5 ];
    int vj;
    int vi;
    if (axis == 0)
    {
        command = 0xd1;
        error = 1;
    }
    else
    {
        command = 0x91;
        error = 2;
    }
    while (V(0x040001c0) & 128)
    {
    }
    V(0x040001c0) = 0x8a01;
    V(0x040001c2) = command & 0xff;
    while (V(0x040001c0) & 128)
    {
    }
    for (i = 0; i < 5; i++)
    {
        V(0x040001c2) = 0;
        while (V(0x040001c0) & 128)
        {
        }
        values[i] = (u16)(V(0x040001c2) & 255) << 8;
        V(0x040001c2) = ((u16)(command & 0xff));
        while (V(0x040001c0) & 128)
        {
        }
        {
            int t = (u16)(V(0x040001c2) & 255);
            t |= values[i];
            values[i] = t;
        }
        values[i] = (values[i] & 0x7ff8) >> 3;
    }
    V(0x040001c0) = 0x8201;
    SPI_DummyWait_03801538();
    max = 0;
    for (i = 0; i < 4; i++)
    {
        for (j = i + 1; j < 5; j++)
        {
            int d;
            d = values[i] - values[j];
            if (d < 0)
            {
                d = -d;
            }
            if (d > max)
            {
                max = d;
            }
        }
    }
    *spreadOut = max;
    for (i = 0; i < 3; i++)
    {
        vi = values[i];
        for (j = i + 1; j < 4; j++)
        {
            vj = values[j];
            int d = vi - vj;
            if (d < 0)
                d = -d;
            if (d > range)
                continue;
            for (k = j + 1; k < 5; k++)
            {
                int vk = values[k];
                d = vi - vk;
                if (d < 0)
                    d = -d;
                if (d > range)
                    continue;
                *out = ((vj + vi * 2 + vk) >> 2) & ~7;
                return 0;
            }
        }
    }
    *out = ((values[0] + values[4]) >> 1) & ~7;
    return error;
}
