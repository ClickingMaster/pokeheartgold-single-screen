#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/mi/memory.h>
#include <nitro/os/message.h>
#include <nitro/os/thread.h>
#include <nitro/pxi.h>
#include <nitro/pxi/fifo.h>
#include <stdarg.h>

#include "spi_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct SpiWork {
    u32 busy;
    u32 owner;
    OSThread thread;
    u8 stack[0x200];
    OSMessageQueue queue;
    OSMessage messages[16];
    SpiCommand commands[16];
    u32 nextCommand;
    OSThreadQueue waitQueue;
    u32 lockId;
} SpiWork;

extern SpiWork spiWork;
extern u16 spiInitialized;

void TP_Init(void);
void NVRAM_Init(void);
void MIC_Init(void);
void PM_Init(void);
void TP_AnalyzeCommand(u32 word);
void NVRAM_AnalyzeCommand(u32 word);
void MIC_AnalyzeCommand(u32 word);
void PM_AnalyzeCommand(u32 word);
void TP_ExecuteProcess(SpiCommand *command);
void NVRAM_ExecuteProcess(SpiCommand *command);
void MIC_ExecuteProcess(SpiCommand *command);
void PM_ExecuteProcess(SpiCommand *command);

void SPI_Init(u32 priority);
void SPI_Lock(u32 id);
void SPI_Unlock(u32 id);
void SPIi_ReturnResult(u32 command, u32 result);
BOOL SPIi_CheckEntry(void);
void SpiCommonThread(void *arg);
void SpiPxiCallback(PXIFifoTag tag, u32 word, BOOL err);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void SPI_Init(u32 priority) {
    int i;
    if (spiInitialized) {
        return;
    }
    spiWork.busy = 0;
    spiInitialized = 1;
    spiWork.owner = SPI_OWNER_NONE;
    TP_Init();
    NVRAM_Init();
    MIC_Init();
    PM_Init();
    PXI_Init();
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_TOUCHPANEL, SpiPxiCallback);
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_MIC, SpiPxiCallback);
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_PM, SpiPxiCallback);
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_NVRAM, SpiPxiCallback);
    OS_InitMessageQueue(&spiWork.queue, spiWork.messages, 16);
    for (i = 0; i < 16; i++) {
        MI_CpuFill8(&spiWork.commands[i], 0, sizeof(SpiCommand));
    }
    spiWork.nextCommand = 0;
    spiWork.waitQueue.tail = NULL;
    spiWork.waitQueue.head = NULL;
    OS_CreateThread(&spiWork.thread, SpiCommonThread, 0, spiWork.stack + sizeof(spiWork.stack), sizeof(spiWork.stack), priority);
    OS_WakeupThreadDirect(&spiWork.thread);
}

void SPI_Lock(u32 id) {
    OSIntrMode state;
    for (;;) {
        state = OS_DisableInterrupts();
        if (!spiWork.busy) {
            break;
        }
        OS_RestoreInterrupts(state);
        OS_SleepThread(&spiWork.waitQueue);
    }
    spiWork.busy = 1;
    spiWork.owner = SPI_OWNER_LOCK;
    spiWork.lockId = id;
    OS_RestoreInterrupts(state);
}

void SPI_Unlock(u32 id) {
    OSIntrMode state;
    if (spiWork.busy && spiWork.owner == SPI_OWNER_LOCK && spiWork.lockId == id) {
        state = OS_DisableInterrupts();
        spiWork.owner = SPI_OWNER_NONE;
        spiWork.busy = 0;
        spiWork.lockId = 0;
        OS_RestoreInterrupts(state);
        OS_WakeupThread(&spiWork.waitQueue);
    }
}

void SPIi_ReturnResult(u32 command, u32 result) {
    int tag;
    u32 word;
    switch ((s32)(command & 0x70)) {
    case 0x00:
    case 0x10:
        tag = PXI_FIFO_TAG_TOUCHPANEL;
        break;
    case 0x40:
    case 0x50:
        tag = PXI_FIFO_TAG_MIC;
        break;
    case 0x60:
    case 0x70:
        tag = PXI_FIFO_TAG_PM;
        break;
    case 0x20:
    case 0x30:
        tag = PXI_FIFO_TAG_NVRAM;
        break;
    }
    word = SPI_PXI_START_BIT | SPI_PXI_END_BIT | (((command & 0xff) | 0x80) << 8) | (result & 0xff);
    while (PXI_SendWordByFifo(tag, word, FALSE) < 0) {}
}

BOOL SPIi_CheckException(u32 owner) {
    return spiWork.busy == 0;
}

void SPIi_GetException(u32 owner) {
    spiWork.busy = 1;
    spiWork.owner = owner;
}

void SPIi_ReleaseException(u32 owner) {
    if (spiWork.owner == owner) {
        spiWork.owner = SPI_OWNER_NONE;
        spiWork.busy = 0;
        OS_WakeupThread(&spiWork.waitQueue);
    }
}

// Reads the variadic words with a plain pointer step (this is what the original code compiles to).
#undef va_arg
#define va_arg(ap, type) (*(type *)((ap += sizeof(type)) - sizeof(type)))

BOOL SPIi_SetEntry(u32 module, u32 command, u16 count, ...) {
    va_list args;
    OSIntrMode irq;
    u32 index;
    int i;
    if (count > 4) {
        return FALSE;
    }
    irq = OS_DisableInterrupts();
    spiWork.commands[spiWork.nextCommand].module = module;
    spiWork.commands[spiWork.nextCommand].command = command;
    va_start(args, count);
    for (i = 0; i < count; i++) {
        spiWork.commands[spiWork.nextCommand].args[i] = va_arg(args, u32);
    }
    index = spiWork.nextCommand;
    spiWork.nextCommand = (index + 1) & 15;
    OS_RestoreInterrupts(irq);
    va_end(args);
    return OS_SendMessage(&spiWork.queue, &spiWork.commands[index], OS_MESSAGE_NOBLOCK);
}

BOOL SPIi_CheckEntry(void) {
    OSMessage message;
    return OS_ReadMessage(&spiWork.queue, &message, OS_MESSAGE_NOBLOCK);
}

void SpiCommonThread(void *arg) {
    OSMessage message;
    for (;;) {
        OS_ReceiveMessage(&spiWork.queue, &message, OS_MESSAGE_BLOCK);
        switch (((SpiCommand *)message)->module) {
        case SPI_MODULE_TP:
            TP_ExecuteProcess(message);
            break;
        case SPI_MODULE_MIC:
            MIC_ExecuteProcess(message);
            break;
        case SPI_MODULE_PM:
            PM_ExecuteProcess(message);
            break;
        case SPI_MODULE_NVRAM:
            NVRAM_ExecuteProcess(message);
            break;
        }
    }
}

void SpiPxiCallback(PXIFifoTag tag, u32 word, BOOL err) {
    if (!err) {
        switch (tag) {
        case PXI_FIFO_TAG_TOUCHPANEL:
            TP_AnalyzeCommand(word);
            break;
        case PXI_FIFO_TAG_MIC:
            MIC_AnalyzeCommand(word);
            break;
        case PXI_FIFO_TAG_PM:
            PM_AnalyzeCommand(word);
            break;
        case PXI_FIFO_TAG_NVRAM:
            NVRAM_AnalyzeCommand(word);
            break;
        }
    }
}

#pragma dont_inline reset
