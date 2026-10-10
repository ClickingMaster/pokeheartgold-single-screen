#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))

struct W35041Entry {
    u32 header, command, args[2];
};

typedef struct {
    u16 key, trigger;
    u8 gap[0x24];
    u32 process, value, reg;
} W35041PM;

extern W35041PM PMi_KeyPattern;
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32);
extern void SPIi_GetException(int);
extern void SPIi_ReleaseException(int);
extern void SPIi_ReturnResult(u16, u32);
extern void PMi_DoSleep(void);
extern void PMi_SetRegister(u16, u8);
extern void PMi_SwitchUtilityProc(u32);
extern void PMi_SetLED(int);
extern int SPIi_CheckException(u32);
extern u8 PMi_GetRegister(u16);
void PM_ExecuteProcess(struct W35041Entry *entry);

void PM_ExecuteProcess(struct W35041Entry *entry) {
    u32 irq = OS_DisableInterrupts();
    if (!SPIi_CheckException(3)) {
        OS_RestoreInterrupts(irq);
        SPIi_ReturnResult((u16)entry->command, 4);
        return;
    }
    SPIi_GetException(3);
    OS_RestoreInterrupts(irq);
    switch (entry->command) {
    case 0x61:
        PMi_KeyPattern.process = 1;
        PMi_KeyPattern.trigger = (u16)entry->args[0];
        PMi_KeyPattern.key = (u16)entry->args[1];
        PMi_DoSleep();
        break;
    case 0x64: {
        u32 reg, value;
        PMi_KeyPattern.process = 4;
        reg = entry->args[0];
        PMi_KeyPattern.reg = reg;
        value = entry->args[1];
        PMi_KeyPattern.value = value;
        PMi_SetRegister((u16)reg, (u8)value);
        SPIi_ReturnResult(0x64, 0);
        break;
    }
    case 0x65: {
        u16 reg;
        u8 value;
        PMi_KeyPattern.process = 3;
        {
            u32 raw = entry->args[0];
            PMi_KeyPattern.reg = raw;
            reg = (u16)raw;
        }
        value = PMi_GetRegister(reg);
        SPIi_ReturnResult((u16)(reg + 0x70), value);
        break;
    }
    case 0x63: {
        u32 value;
        PMi_KeyPattern.process = 2;
        value = entry->args[0];
        PMi_KeyPattern.value = value;
        PMi_SwitchUtilityProc(value);
        SPIi_ReturnResult(0x63, 0);
        break;
    }
    case 0x66:
        PMi_SetLED(entry->args[0]);
        break;
    default:
        SPIi_ReturnResult((u16)entry->command, 1);
        break;
    }
    SPIi_ReleaseException(3);
}
