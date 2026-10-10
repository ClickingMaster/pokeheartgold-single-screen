#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/os/system.h>

#include "spi_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Matched with the 2.0/sp1 compiler (sub/Makefile override), so it is not merged into mic_driver.c.

MicWork micw;

void SPIi_ReturnResult(u16 command, u32 result);
BOOL MicSetTimerValue(u32 value);

void MIC_Init(void);
void MIC_AnalyzeCommand(u32 word);

void MIC_Init(void) {
    int i;
    micw.state = 0;
    for (i = 0; i < 16; i++) {
        micw.command[i] = 0;
    }
    REG_TM3CNT_H &= ~0x80;
}

void MIC_AnalyzeCommand(u32 word) {
    u16 command;
    if (word & 0x2000000) {
        int i;
        for (i = 0; i < 16; i++) {
            micw.command[i] = 0;
        }
    }
    micw.command[(word & 0xf0000) >> 16] = (u16)word;
    if (!(word & 0x1000000)) {
        return;
    }
    command = (micw.command[0] & 0xff00) >> 8;
    switch (command) {
    case 0x40:
        if (!SPIi_SetEntry(SPI_MODULE_MIC, command, 1, micw.command[0] & 0xff)) {
            SPIi_ReturnResult(command, 4);
        }
        *(volatile u16 *)MIC_SAMPLE_ADDR = 0;
        *(volatile u32 *)MIC_SAMPLE_POINTER_ADDR = 0;
        break;
    case 0x41: {
        u32 address, length;
        if (micw.state) {
            SPIi_ReturnResult(command, 3);
            break;
        }
        micw.flags = micw.command[0] & 0xff;
        address = ((u32)micw.command[1] << 16) | micw.command[2];
        if (address < 0x02000000 || address >= 0x02400000) {
            SPIi_ReturnResult(command, 2);
            break;
        }
        micw.buffer = (u8 *)address;
        length = ((u32)micw.command[3] << 16) | micw.command[4];
        if (address + length > 0x02400000) {
            SPIi_ReturnResult(command, 2);
            break;
        }
        micw.size = length;
        if (!MicSetTimerValue(((u32)micw.command[5] << 16) | micw.command[6])) {
            SPIi_ReturnResult(command, 2);
            break;
        }
        micw.offset = 0;
        micw.mode = micw.flags & 7;
        if (!SPIi_SetEntry(SPI_MODULE_MIC, command, 0)) {
            SPIi_ReturnResult(command, 4);
            break;
        }
        *(volatile u16 *)MIC_SAMPLE_ADDR = 0;
        *(volatile u32 *)MIC_SAMPLE_POINTER_ADDR = 0;
        micw.state = 1;
        break;
    }
    case 0x42:
        if (micw.state != 2) {
            SPIi_ReturnResult(command, 3);
            break;
        }
        if (!SPIi_SetEntry(SPI_MODULE_MIC, command, 0)) {
            SPIi_ReturnResult(command, 4);
            break;
        }
        micw.state = 3;
        REG_TM3CNT_H &= ~0x80;
        break;
    case 0x43: {
        OSIntrMode irq;
        if (micw.state != 2) {
            SPIi_ReturnResult(command, 3);
            break;
        }
        if (!MicSetTimerValue(((u32)micw.command[1] << 16) | micw.command[2])) {
            SPIi_ReturnResult(command, 2);
            break;
        }
        irq = OS_DisableInterrupts();
        REG_TM3CNT_H &= ~0x80;
        REG_TM3CNT_L = micw.timer;
        REG_TM3CNT_H = micw.clock | 0xc0;
        OS_RestoreInterrupts(irq);
        SPIi_ReturnResult(command, 0);
        break;
    }
    default:
        SPIi_ReturnResult(command, 1);
        break;
    }
}
