#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/exi/ARM7/genPort.h>
#include <nitro/os/interrupt.h>
#include <nitro/os/system.h>
#include <nitro/os/terminate_proc.h>
#include <nitro/pxi/fifo.h>

#include "spi_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#define REG_IME    (*(volatile u16 *)0x04000208)
#define REG_KEYCNT (*(volatile u16 *)0x04000132)
#define REG_RCNT   (*(volatile u16 *)0x04000134)

// Power-management chip registers are written over SPI (device select 0x02).
#define PM_REG_CONTROL 0

// Wake-up triggers selected by the ARM9 (PMi_TriggerBL bits).
#include <nitro/spi/common/pm_common.h>

typedef struct PmBlinkPattern {
    u64 pattern;
    u16 length;
    u16 speed;
} PmBlinkPattern;

typedef struct PmBlinkCounter {
    u32 count;
    int pattern;
} PmBlinkCounter;

typedef struct PmWork {
    u16 command[16]; // PXI command words collected by PM_AnalyzeCommand
    u32 busy;
    u32 value;
    u32 reg;
} PmWork;

// The original code reaches PMi_Work through PMi_KeyPattern, the first of the four consecutive PM
// variables (PMi_KeyPattern, PMi_TriggerBL, PMi_Initialized, PMi_Work); those accesses keep that
// base symbol, so this view of PMi_KeyPattern spans all four.
typedef struct PmVariables {
    u16 keyPattern;
    u16 trigger;
    u32 initialized;
    PmWork work;
} PmVariables;

typedef struct PmRequest {
    u32 module;
    u32 command;
    u32 args[2];
} PmRequest;

extern PmVariables PMi_KeyPattern;
extern u16 PMi_TriggerBL;
extern PmWork PMi_Work;
extern PmBlinkCounter PMi_BlinkCounter;
extern int PMi_LEDStatus;
extern PmBlinkPattern PMi_BlinkPatternData[];

void SPIi_ReturnResult(u16 command, u32 result);
void SND_BeginSleep(void);
void SND_EndSleep(void);
void SVC_Sleep(void);

void PM_Init(void);
void PM_AnalyzeCommand(u32 word);
void PM_ExecuteProcess(PmRequest *request);
void PMi_SendPxiCommand(u32 command, u32 param, u32 data);
void PMi_SetRegister(u16 reg, u32 value);
void SPI_SendWait_03801D48(u16 value);
u8 PMi_GetRegister(u16 reg);
void PMi_SetControl(u32 mask);
void PMi_ResetControl(u32 mask);
void PMi_SwitchUtilityProc(u32 command);
void PMi_SetLED(int state);
void PMi_DoSleep(void);
void __VENEER_SVC_Sleep(void);
void PM_SelfBlinkProc(void);
void PM_SetLEDPattern(int pattern);
int PM_GetLEDPattern(void);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void PM_Init(void) {
    int i;
    PMi_KeyPattern.initialized = 1;
    PMi_KeyPattern.work.busy = 0;
    for (i = 0; i < 16; i++) {
        PMi_Work.command[i] = 0;
    }
}

void PM_AnalyzeCommand(u32 word) {
    u32 header;
    u16 command;
    int i;
    if (word & 0x02000000) {
        for (i = 0; i < 16; i++) {
            PMi_Work.command[i] = 0;
        }
    }
    PMi_Work.command[(word & 0xf0000) >> 16] = word;
    if (!(word & 0x01000000)) {
        return;
    }
    header = PMi_KeyPattern.work.command[0];
    command = (header & 0xff00) >> 8;
    switch (command) {
    case 0x60:
        SPIi_ReturnResult(0x60, 0);
        break;
    case 0x61:
        if (!SPIi_SetEntry(SPI_MODULE_PM, command, 2, (u32)(header & 0xff), (u32)PMi_KeyPattern.work.command[1])) {
            SPIi_ReturnResult(command, 4);
        }
        break;
    case 0x64:
        if (!SPIi_SetEntry(SPI_MODULE_PM, command, 2, (u32)(header & 0xff), (u32)PMi_KeyPattern.work.command[1])) {
            SPIi_ReturnResult(command, 4);
        }
        break;
    case 0x65:
        if (!SPIi_SetEntry(SPI_MODULE_PM, command, 1, (u32)(u16)header)) {
            SPIi_ReturnResult(command, 4);
        }
        break;
    case 0x63:
        if (!SPIi_SetEntry(SPI_MODULE_PM, command, 1, ((header & 0xff) << 16) | PMi_KeyPattern.work.command[1])) {
            SPIi_ReturnResult(command, 4);
        }
        break;
    case 0x66:
        PM_SetLEDPattern(header & 0xff);
        SPIi_ReturnResult(0x66, 0);
        break;
    case 0x67:
        SPIi_ReturnResult(0x67, (u16)PM_GetLEDPattern());
        break;
    default:
        SPIi_ReturnResult(command, 1);
        break;
    }
}

void PM_ExecuteProcess(PmRequest *request) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (!SPIi_CheckException(SPI_MODULE_PM)) {
        OS_RestoreInterrupts(irq);
        SPIi_ReturnResult((u16)request->command, 4);
        return;
    }
    SPIi_GetException(SPI_MODULE_PM);
    OS_RestoreInterrupts(irq);
    switch (request->command) {
    case 0x61:
        PMi_KeyPattern.work.busy = 1;
        PMi_KeyPattern.trigger = (u16)request->args[0];
        PMi_KeyPattern.keyPattern = (u16)request->args[1];
        PMi_DoSleep();
        break;
    case 0x64: {
        u32 reg, value;
        PMi_KeyPattern.work.busy = 4;
        reg = request->args[0];
        PMi_KeyPattern.work.reg = reg;
        value = request->args[1];
        PMi_KeyPattern.work.value = value;
        PMi_SetRegister((u16)reg, (u8)value);
        SPIi_ReturnResult(0x64, 0);
        break;
    }
    case 0x65: {
        u16 reg;
        u8 value;
        PMi_KeyPattern.work.busy = 3;
        {
            u32 raw = request->args[0];
            PMi_KeyPattern.work.reg = raw;
            reg = (u16)raw;
        }
        value = PMi_GetRegister(reg);
        SPIi_ReturnResult((u16)(reg + 0x70), value);
        break;
    }
    case 0x63: {
        u32 value;
        PMi_KeyPattern.work.busy = 2;
        value = request->args[0];
        PMi_KeyPattern.work.value = value;
        PMi_SwitchUtilityProc(value);
        SPIi_ReturnResult(0x63, 0);
        break;
    }
    case 0x66:
        PMi_SetLED(request->args[0]);
        break;
    default:
        SPIi_ReturnResult((u16)request->command, 1);
        break;
    }
    SPIi_ReleaseException(SPI_MODULE_PM);
}

void PMi_SendPxiCommand(u32 command, u32 param, u32 data) {
    u32 word = ((command & 0x3c00000u) << 22) | ((param & 0x3f0000u) << 16) | (u16)data;
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_PM, word, 0) != 0) {}
}

void PMi_SetRegister(u16 reg, u32 value) {
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = 0x8202;
    REG_SPICNT = 0x8802;
    SPI_SendWait_03801D48((u8)reg);
    REG_SPICNT = 0x8002;
    REG_SPIDATA = (u8)value;
}

void SPI_SendWait_03801D48(u16 value) {
    REG_SPIDATA = (u8)value;
    while (REG_SPICNT & SPICNT_BUSY) {}
}

u8 PMi_GetRegister(u16 reg) {
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = 0x8202;
    REG_SPICNT = 0x8802;
    SPI_SendWait_03801D48((u8)(reg | 0x80));
    REG_SPICNT = 0x8002;
    REG_SPIDATA = 0;
    while (REG_SPICNT & SPICNT_BUSY) {}
    return (u8)REG_SPIDATA;
}

void PMi_SetControl(u32 mask) {
    u32 value = PMi_GetRegister(PM_REG_CONTROL);
    value |= mask;
    PMi_SetRegister(PM_REG_CONTROL, value);
}

void PMi_ResetControl(u32 mask) {
    u32 value = PMi_GetRegister(PM_REG_CONTROL);
    value &= ~mask;
    PMi_SetRegister(PM_REG_CONTROL, (u8)value);
}

void PMi_SwitchUtilityProc(u32 command) {
    switch (command) {
    case 1:
        PM_SetLEDPattern(1);
        PMi_SetLED(1);
        break;
    case 2:
        PM_SetLEDPattern(3);
        PMi_SetLED(3);
        break;
    case 3:
        PM_SetLEDPattern(2);
        PMi_SetLED(2);
        break;
    case 4:
        PMi_SetControl(4);
        break;
    case 5:
        PMi_ResetControl(4);
        break;
    case 6:
        PMi_SetControl(8);
        break;
    case 7:
        PMi_ResetControl(8);
        break;
    case 8:
        PMi_SetControl(12);
        break;
    case 9:
        PMi_ResetControl(12);
        break;
    case 10:
        PMi_SetControl(1);
        break;
    case 11:
        PMi_ResetControl(1);
        break;
    case 12:
        PMi_ResetControl(2);
        break;
    case 13:
        PMi_SetControl(2);
        break;
    case 15:
        PMi_ResetControl(64);
        break;
    case 14:
        SND_BeginSleep();
        PMi_SetControl(64);
        break;
    }
}

void PMi_SetLED(int state) {
    switch (state) {
    case 1:
        PMi_ResetControl(16);
        break;
    case 3:
        PMi_SetControl(48);
        break;
    case 2:
        PMi_ResetControl(32);
        PMi_SetControl(16);
        break;
    default:
        OS_Terminate();
        break;
    }
    PMi_LEDStatus = state;
}

void PMi_DoSleep(void) {
    OSIntrMode irq;
    OSIrqMask mask;
    u8 control;
    u16 rcnt;
    BOOL restore;
    u16 ime;
    ime = REG_IME;
    restore = 0;
    REG_IME = OS_IME_DISABLE;
    irq = OS_DisableInterrupts();
    mask = OS_DisableIrqMask(~0xfe000000u);
    control = PMi_GetRegister(PM_REG_CONTROL);
    PM_SetLEDPattern(2);
    PMi_SetLED(2);
    PMi_SetLED(2);
    SND_BeginSleep();
    PMi_ResetControl(1);
    if (PMi_TriggerBL & PM_TRIGGER_KEY) {
        REG_KEYCNT = PMi_KeyPattern.keyPattern | REG_PAD_KEYCNT_INTR_MASK;
        OS_EnableIrqMask(OS_IE_KEY);
    }
    if (PMi_TriggerBL & PM_TRIGGER_COVER_OPEN) {
        OS_EnableIrqMask(0x400000);
    }
    if (PMi_TriggerBL & PM_TRIGGER_RTC_ALARM) {
        rcnt = REG_RCNT;
        restore = 1;
        EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
        EXIi_SetBitRcnt0L(0x40, 0);
        EXIi_SetBitRcnt0L(0x100, 0x100);
        OS_EnableIrqMask(OS_IE_SIO);
    }
    if (PMi_TriggerBL & PM_TRIGGER_CARD) {
        OS_EnableIrqMask(OS_IE_CARD_IREQ);
    }
    if (PMi_TriggerBL & PM_TRIGGER_CARTRIDGE) {
        OS_EnableIrqMask(OS_IE_CARTRIDGE);
    }
    OS_RestoreInterrupts(irq);
    {
        u16 previous = REG_IME;
        (void)previous;
        REG_IME = OS_IME_ENABLE;
    }
    __VENEER_SVC_Sleep();
    PMi_SetRegister(PM_REG_CONTROL, control);
    {
        int a = (PMi_TriggerBL & 0x20) ? 6 : 7;
        int b = (PMi_TriggerBL & 0x40) ? 4 : 5;
        PMi_SwitchUtilityProc(a);
        PMi_SwitchUtilityProc(b);
    }
    if (restore) {
        REG_RCNT = rcnt;
    }
    PMi_SetControl(1);
    SND_EndSleep();
    PMi_Work.busy = 0;
    PMi_SendPxiCommand(0x62, 0, 0);
    OS_DisableInterrupts();
    OS_SetIrqMask(mask);
    OS_RestoreInterrupts(irq);
    {
        u16 previous = REG_IME;
        (void)previous;
        REG_IME = ime;
    }
}

void __VENEER_SVC_Sleep(void) {
    SVC_Sleep();
}

void PM_SelfBlinkProc(void) {
    int pattern = PMi_BlinkCounter.pattern;
    PmBlinkPattern *data;
    u32 status;
    if (pattern == 0) {
        if (SPIi_SetEntry(SPI_MODULE_PM, 0x66, 1, 1)) {
            PM_SetLEDPattern(1);
        }
        return;
    }
    if (pattern < 4) {
        if (pattern != PMi_LEDStatus) {
            SPIi_SetEntry(SPI_MODULE_PM, 0x66, 1, pattern);
        }
        return;
    }
    data = &PMi_BlinkPatternData[pattern - 4];
    if (data->pattern & (0x8000000000000000ULL >> (PMi_BlinkCounter.count / data->speed))) {
        status = 1;
    } else {
        status = 2;
    }
    if (++PMi_BlinkCounter.count >= data->length * data->speed) {
        PMi_BlinkCounter.count = 0;
    }
    if (status != PMi_LEDStatus) {
        SPIi_SetEntry(SPI_MODULE_PM, 0x66, 1, status);
    }
}

void PM_SetLEDPattern(int pattern) {
    if (pattern <= 15) {
        PMi_BlinkCounter.pattern = pattern;
        PMi_BlinkCounter.count = 0;
    }
}

int PM_GetLEDPattern(void) {
    return PMi_BlinkCounter.pattern;
}

#pragma dont_inline reset
