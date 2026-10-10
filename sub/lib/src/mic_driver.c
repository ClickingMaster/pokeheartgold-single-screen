#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/os/interrupt.h>
#include <nitro/os/irqHandler.h>
#include <nitro/os/system.h>

#include "spi_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Interrupt vector / check flags the ARM7 BIOS uses (end of the ARM7 WRAM).
#define OS_IRQ_HANDLER_VECTOR (*(volatile u32 *)0x0380FFFC)
#define OS_IRQ_CHECK_FLAGS    (*(volatile u32 *)0x0380FFF8)
#define REG_IF                (*(volatile u32 *)0x04000214)

// Running averages used to remove the DC bias of the 8- and 12-bit samples.
typedef struct MicCalibration {
    s8 bias8;
    u8 unk01;
    s16 bias12;
    u32 sum12;
    u32 count12;
    u32 count8;
    u32 sum8;
} MicCalibration;

typedef struct MicRequest {
    u32 module;
    u32 command;
    u32 flags;
} MicRequest;

extern MicCalibration micCalib;
extern u32 micIntrInfo[4];
extern u32 micIntrPrio[];

void SPIi_ReturnResult(u16 command, u32 result);

BOOL MicSetTimerValue(u32 value);
void MIC_ExecuteProcess(MicRequest *request);
void MIC_TimerHandler(void);
void MicTimerHandler(void);
u16 MIC_ExecSampling8(void);
u8 SPI_DummyWaitReceive(void);
u16 MIC_ExecSampling12(void);
void MIC_SetIrqFunction(u32 mask, OSIrqFunction handler);
void MIC_EnableMultipleInterrupt(void);
void MIC_DisableMultipleInterrupt(void);
void MIC_IrqHandler(void);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

BOOL MicSetTimerValue(u32 value) {
    if (value < 0x10000) {
        micw.clock = 0;
        micw.timer = (u16)(0x10000 - value);
        return TRUE;
    }
    if (value < 0x400000) {
        micw.clock = 1;
        micw.timer = (u16)(0x10000 - (value >> 6));
        return TRUE;
    }
    if (value < 0x1000000) {
        micw.clock = 2;
        micw.timer = (u16)(0x10000 - (value >> 8));
        return TRUE;
    }
    if (value < 0x4000000) {
        micw.clock = 3;
        micw.timer = (u16)(0x10000 - (value >> 10));
        return TRUE;
    }
    return FALSE;
}

#pragma dont_inline reset

static inline void PublishMicSample(u16 value) {
    *(u16 *)MIC_SAMPLE_ADDR = value;
    *(u32 *)MIC_SAMPLE_POINTER_ADDR = MIC_SAMPLE_ADDR;
}

void MIC_ExecuteProcess(MicRequest *request) {
    switch (request->command) {
    case 0x40: {
        OSIntrMode irq = OS_DisableInterrupts();
        u16 value;
        if (!SPIi_CheckException(SPI_MODULE_MIC)) {
            OS_RestoreInterrupts(irq);
            SPIi_ReturnResult((u16)request->command, 4);
            break;
        }
        SPIi_GetException(SPI_MODULE_MIC);
        OS_RestoreInterrupts(irq);
        if ((request->flags & 1) == 1) {
            value = MIC_ExecSampling12();
            if (request->flags & 2) {
                value ^= 0x8000;
            }
            PublishMicSample(value);
        } else {
            value = MIC_ExecSampling8();
            if (request->flags & 2) {
                value ^= 0x80;
            }
            PublishMicSample(value);
        }
        SPIi_ReturnResult((u16)request->command, 0);
        SPIi_ReleaseException(SPI_MODULE_MIC);
        break;
    }
    case 0x41:
        if (micw.state == 1) {
            OSIntrMode irq;
            micw.last = 0;
            micw.partial = 0;
            irq = OS_DisableInterrupts();
            OS_EnableIrqMask(OS_IE_TIMER3);
            MIC_SetIrqFunction(OS_IE_TIMER3, MIC_TimerHandler);
            MIC_EnableMultipleInterrupt();
            REG_TM3CNT_L = micw.timer;
            REG_TM3CNT_H = micw.clock | 0xc0;
            OS_RestoreInterrupts(irq);
            SPIi_ReturnResult((u16)request->command, 0);
            micw.state = 2;
        } else {
            SPIi_ReturnResult((u16)request->command, 3);
        }
        break;
    case 0x42:
        if (micw.state - 3u <= 1) {
            OSIntrMode irq;
            REG_TM3CNT_H &= ~0x80u;
            irq = OS_DisableInterrupts();
            MIC_SetIrqFunction(OS_IE_TIMER3, NULL);
            MIC_DisableMultipleInterrupt();
            OS_RestoreInterrupts(irq);
            if (micw.state == 3) {
                SPIi_ReturnResult(0x42, 0);
            } else {
                SPIi_ReturnResult(0x51, 0);
            }
            micw.state = 0;
        } else {
            if (micw.state == 3) {
                SPIi_ReturnResult(0x42, 3);
            } else {
                SPIi_ReturnResult(0x51, 3);
            }
        }
        break;
    }
}

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void MIC_TimerHandler(void) {
    MicTimerHandler();
    OS_IRQ_CHECK_FLAGS |= OS_IE_TIMER3;
    REG_IF = OS_IE_TIMER3;
}

// clang-format off
asm void MicTimerHandler(void)
{
    stmdb sp!, {r4, r5, r6, r7, lr}
    ldr r4, =micw
    ldrh r5, [r4, #0x26]
    and r0, r5, #4
    cmp r0, #4
    ldrh r6, [r4, #0x38]
    ldrneh r7, [r4, #0x3a]
    ldreq r7, =0x0000FFFF
    bl SPIi_CheckEntry
    cmp r0, #0
    bne @L03802964
    mov r0, #2
    bl SPIi_CheckException
    cmp r0, #0
    beq @L03802964
    and r0, r5, #1
    cmp r0, #1
    bne @L03802954
    bl MIC_ExecSampling12
    tst r5, #2
    moveq r7, r0
    eorne r7, r0, #0x8000
    b @L03802964
@L03802954:
    bl MIC_ExecSampling8
    tst r5, #2
    moveq r7, r0
    eorne r7, r0, #0x80
@L03802964:
    and r0, r5, #1
    ldr r3, =0x027FFC00
    ldr r1, [r4, #0x2c]
    cmp r0, #1
    bne @L03802994
    ldr r2, [r4, #0x28]
    strh r7, [r2, r1]!
    str r2, [r3, #0x390]
    add r3, r3, #0x394
    strh r7, [r3]
    add r1, r1, #2
    b @L038029CC
@L03802994:
    and r7, r7, #0xff
    tst r1, #1
    bne @L038029AC
    mov r6, r7
    add r1, r1, #1
    b @L038029CC
@L038029AC:
    orr r0, r6, r7, lsl #8
    ldr r2, [r4, #0x28]
    sub r1, r1, #1
    strh r0, [r2, r1]!
    str r2, [r3, #0x390]
    add r3, r3, #0x394
    strh r0, [r3]
    add r1, r1, #2
@L038029CC:
    strh r6, [r4, #0x38]
    strh r7, [r4, #0x3a]
    ldr r0, [r4, #0x30]
    cmp r1, r0
    movcs r1, #0
    str r1, [r4, #0x2c]
    bcc @L03802A48
    ldrh r0, [r4, #0x24]
    and r0, r0, #0x10
    cmp r0, #0x10
    bne @L03802A08
    mov r0, #0x51
    mov r1, #0
    bl SPIi_ReturnResult
    b @L03802A48
@L03802A08:
    mov r0, #2
    mov r1, #0x42
    mov r2, #0
    bl SPIi_SetEntry
    cmp r0, #0
    bne @L03802A30
    mov r0, #0x51
    mov r1, #4
    bl SPIi_ReturnResult
    b @L03802A48
@L03802A30:
    mov r0, #4
    str r0, [r4, #0x20]
    ldr r1, =0x0400010E
    ldrh r0, [r1]
    bic r0, r0, #0x80
    strh r0, [r1]
@L03802A48:
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
}
// clang-format on

// The two sampling functions are the only ones here that were not force_active.
#pragma force_active off

u16 MIC_ExecSampling8(void) {
    u32 first, sample;
    int value;
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = 0x8a01;
    REG_SPIDATA = 0xec;
    while (REG_SPICNT & SPICNT_BUSY) {}
    first = (SPI_DummyWaitReceive() & 0xff) << 8;
    REG_SPICNT = 0x8201;
    sample = SPI_DummyWaitReceive() | first;
    sample = (u16)((sample & 0x7f80) >> 7);
    micCalib.sum8 += sample;
    micCalib.count8++;
    if (micCalib.count8 >= 0x1000) {
        int target = (s8)((micCalib.sum8 >> 12) - 0x80);
        int current = micCalib.bias8;
        if (current < target && current < 12) {
            micCalib.bias8 = current + 1;
        } else if (target < current && current > -12) {
            micCalib.bias8--;
        }
        micCalib.count8 = 0;
        micCalib.sum8 = 0;
    }
    value = (int)sample - micCalib.bias8;
    if (value > 0xff) {
        value = 0xff;
    } else if (value < 0) {
        value = 0;
    }
    return (u16)value;
}

#pragma force_active on

u8 SPI_DummyWaitReceive(void) {
    REG_SPIDATA = 0;
    while (REG_SPICNT & SPICNT_BUSY) {}
    return (u8)REG_SPIDATA;
}

#pragma force_active off

u16 MIC_ExecSampling12(void) {
    u16 data, sample;
    int value;
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = 0x8a01;
    REG_SPIDATA = 0xe4;
    while (REG_SPICNT & SPICNT_BUSY) {}
    data = SPI_DummyWaitReceive() << 8;
    REG_SPICNT = 0x8201;
    data |= SPI_DummyWaitReceive();
    sample = (data & 0x7ff8) << 1;
    micCalib.sum12 += sample;
    micCalib.count12++;
    if (micCalib.count12 >= 0x100) {
        int target = (s16)(((micCalib.sum12 >> 8) & 0xfff0) - 0x8000);
        int current = micCalib.bias12;
        if (current < target && current < 0xc00) {
            micCalib.bias12 = current + 16;
        } else if (target < current && current > -0xc00) {
            micCalib.bias12 -= 16;
        }
        micCalib.count12 = 0;
        micCalib.sum12 = 0;
    }
    value = (int)sample - micCalib.bias12;
    if (value > 0xfff0) {
        value = 0xfff0;
    } else if (value < 0) {
        value = 0;
    }
    return (u16)value;
}

#pragma force_active on

void MIC_SetIrqFunction(u32 mask, OSIrqFunction handler) {
    int i;
    for (i = 0; i < OS_IRQ_TABLE_MAX; i++) {
        if (mask & 1) {
            OS_IRQTable[i] = handler;
        }
        mask >>= 1;
    }
}

// While sampling, the microphone timer interrupt may interrupt other interrupt handlers:
// MIC_IrqHandler temporarily replaces the BIOS interrupt vector.
void MIC_EnableMultipleInterrupt(void) {
    u32 previous = OS_IRQ_HANDLER_VECTOR;
    if (previous != (u32)MIC_IrqHandler) {
        OSIntrMode irq;
        micIntrInfo[0] = 0;
        micIntrInfo[1] = 0x0380fe80;
        micIntrInfo[2] = OS_IE_TIMER3;
        micIntrInfo[3] = previous;
        irq = OS_DisableInterrupts();
        OS_IRQ_HANDLER_VECTOR = (u32)MIC_IrqHandler;
        OS_RestoreInterrupts(irq);
    }
}

void MIC_DisableMultipleInterrupt(void) {
    if (OS_IRQ_HANDLER_VECTOR == (u32)MIC_IrqHandler) {
        OSIntrMode irq = OS_DisableInterrupts();
        OS_IRQ_HANDLER_VECTOR = micIntrInfo[3];
        OS_RestoreInterrupts(irq);
    }
}

#pragma dont_inline reset

// clang-format off
asm void MIC_IrqHandler(void)
{
    mov r12, #0x4000000
    add r1, r12, #0x208
    ldrh r0, [r1]
    tst r0, r0
    bxeq lr
    ldr r3, [r12, #0x210]
    ldr r1, [r12, #0x214]
    ands r2, r1, r3
    bxeq lr
    ldr r0, =0x01DF3FFF
    tst r2, r0
    streq r2, [r12, #0x214]
    bxeq lr
    stmdb sp!, {lr}
    mrs r0, spsr
    stmdb sp!, {r0}
    stmdb sp, {sp, lr}^
    sub sp, sp, #8
    mov r0, #0x9f
    msr cpsr_c, r0
    ldr r1, =OSi_ThreadInfo
    ldrh r0, [r1, #2]
    add r0, r0, #1
    strh r0, [r1, #2]
    ldr r1, =micIntrInfo
    cmp r0, #1
    moveq r0, sp
    ldreq sp, [r1, #4]
    streq r0, [r1, #4]
    stmdb sp!, {r3}
    ldr r1, =micIntrPrio
    ldr r0, [r1]
    tst r0, r2
    strne r0, [r12, #0x214]
    ldrne r0, [r1, #4]
    ldrne r3, =OS_IRQTable
    ldrne r0, [r3, r0, lsl #2]
    bne @L03802E90
    mov r3, #1
@L03802E50:
    ldr r0, [r1, r3, lsl #3]
    tst r0, r2
    addeq r3, r3, #1
    beq @L03802E50
    str r0, [r12, #0x214]
    add r0, r1, r3, lsl #3
    ldr r2, [r0, #4]
    ldr r3, =OS_IRQTable
    ldr r0, [r3, r2, lsl #2]
    ldr r2, =OSi_ThreadInfo
    ldrh r3, [r2, #2]
    cmp r3, #1
    ldreq r2, [r1]
    streq r2, [r12, #0x210]
    moveq r2, #0x1f
    msreq cpsr_c, r2
@L03802E90:
    ldr r1, [r12, #0x210]
    stmdb sp!, {r1}
    add lr, pc, #0
    bx r0
    mov r0, #0x9f
    msr cpsr_c, r0
    mov r12, #0x4000000
    ldmia sp!, {r0}
    ldr r1, [r12, #0x210]
    eor r2, r0, r1
    and r1, r2, r1
    and r0, r2, r0
    ldmia sp!, {r3}
    orr r3, r3, r1
    bic r3, r3, r0
    str r3, [r12, #0x210]
    ldr r2, =OSi_ThreadInfo
    ldr r3, =micIntrInfo
    ldrh r0, [r2, #2]
    subs r1, r0, #1
    strh r1, [r2, #2]
    moveq r0, sp
    ldreq sp, [r3, #4]
    streq r0, [r3, #4]
    mov r0, #0x92
    msr cpsr_c, r0
    ldmia sp, {sp, lr}^
    mov r0, r0
    add sp, sp, #8
    ldmia sp!, {r0}
    msr spsr_fc, r0
    tst r1, r1
    ldreq r0, =OS_IrqHandler_ThreadSwitch
    addeq lr, pc, #0
    bxeq r0
    ldmia sp!, {pc}
}
// clang-format on
