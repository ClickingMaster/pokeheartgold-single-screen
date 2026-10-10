#ifndef POKEHEARTGOLD_SUB_SPI_INTERNAL_H
#define POKEHEARTGOLD_SUB_SPI_INTERNAL_H

#include <nitro/os/valarm.h>
#include <nitro/types.h>

// ARM7 SPI bus: touch panel, NVRAM, microphone and power-management drivers.

#define REG_SPICNT  (*(volatile u16 *)0x040001C0)
#define REG_SPIDATA (*(volatile u16 *)0x040001C2)

#define SPICNT_BUSY 0x0080

// SPI devices served by the common SPI thread (SpiCommand.module / SPIi_*Exception owner).
#define SPI_MODULE_TP    0
#define SPI_MODULE_NVRAM 1
#define SPI_MODULE_MIC   2
#define SPI_MODULE_PM    3
#define SPI_OWNER_LOCK   4 // SPI_Lock
#define SPI_OWNER_NONE   5

#define TP_VALARM_TAG 0x54505641 // 'TPVA'

typedef struct SpiCommand {
    u32 module;
    u32 command;
    u32 args[4];
} SpiCommand;

// Touch-panel sample word shared with the ARM9.
typedef union TpSample {
    struct {
        u32 x : 12;
        u32 y : 12;
        u32 touch : 1;
        u32 validity : 2;
        u32 unk27 : 5;
    } bits;

    u16 halves[2];
} TpSample;

typedef struct TpWork {
    u16 command[16]; // PXI command words collected by TP_AnalyzeCommand
    u32 state;
    s32 range;
    s32 minRange;
    OSVAlarm alarms[4];
    s16 lines[4];
} TpWork;

typedef struct TpRequest {
    u32 module;
    u32 command;
    u32 count;
    u32 start;
} TpRequest;

extern TpWork tpw;
extern u16 last_touch_flg;

BOOL SPIi_CheckException(u32 owner);
void SPIi_GetException(u32 owner);
void SPIi_ReleaseException(u32 owner);
BOOL SPIi_SetEntry(u32 module, u32 command, u16 count, ...);
// SPIi_ReturnResult is declared by each file: the drivers' callers were compiled against different
// parameter widths (u16 / u32), which changes their code.

void TP_ExecSampling(TpSample *sample, int range, u16 *spread);
void TpVAlarmHandler(void *arg);

#endif // POKEHEARTGOLD_SUB_SPI_INTERNAL_H
