#pragma force_active on
#include <nitro/types.h>

#include "spi_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Matched with the 2.0/sp1 compiler (sub/Makefile override), so it is not merged with its neighbours.

// NVRAM commands accept data buffers in main memory only.
#define IS_MAIN_MEMORY(addr) ((addr) >= 0x2000000 && (addr) < 0x2800000)

NvramWork nvramw;

void SPIi_ReturnResult(u16 command, u32 result);

void NVRAM_AnalyzeCommand(u32 packet);

void NVRAM_AnalyzeCommand(u32 packet) {
    u16 command;
    u32 arg0;
    u32 arg1;
    u32 arg2;
    if (packet & SPI_PXI_START_BIT) {
        int i;
        for (i = 0; i < 16; i++) {
            nvramw.command[i] = 0;
        }
    }
    nvramw.command[(packet & SPI_PXI_INDEX_MASK) >> SPI_PXI_INDEX_SHIFT] = (u16)packet;
    if (!(packet & SPI_PXI_END_BIT)) {
        return;
    }
    command = (nvramw.command[0] & 0xff00) >> 8;
    switch (command) {
    case SPI_PXI_COMMAND_NVRAM_RDSR:
    case SPI_PXI_COMMAND_NVRAM_RSI:
        arg2 = ((u32)nvramw.command[1] << 8) | ((u32)nvramw.command[0] << 24) | ((u32)(nvramw.command[2] & 0xff00) >> 8);
        if (!IS_MAIN_MEMORY(arg2)) {
            SPIi_ReturnResult(command, SPI_PXI_RESULT_INVALID_PARAMETER);
            return;
        }
        break;
    case SPI_PXI_COMMAND_NVRAM_READ:
    case SPI_PXI_COMMAND_NVRAM_FAST_READ:
        arg2 = ((u32)nvramw.command[4] << 16) | nvramw.command[5];
        if (!IS_MAIN_MEMORY(arg2)) {
            SPIi_ReturnResult(command, SPI_PXI_RESULT_INVALID_PARAMETER);
            return;
        }
        arg0 = (((u32)nvramw.command[0] << 24) >> 8) | nvramw.command[1];
        arg1 = ((u32)nvramw.command[2] << 16) | nvramw.command[3];
        break;
    case SPI_PXI_COMMAND_NVRAM_PW:
    case SPI_PXI_COMMAND_NVRAM_PP:
        arg2 = ((u32)nvramw.command[3] << 16) | nvramw.command[4];
        if (!IS_MAIN_MEMORY(arg2)) {
            SPIi_ReturnResult(command, SPI_PXI_RESULT_INVALID_PARAMETER);
            return;
        }
        arg0 = (((u32)nvramw.command[0] << 24) >> 8) | nvramw.command[1];
        arg1 = nvramw.command[2];
        break;
    case SPI_PXI_COMMAND_NVRAM_PE:
    case SPI_PXI_COMMAND_NVRAM_SE:
        arg0 = (((u32)nvramw.command[0] << 24) >> 8) | nvramw.command[1];
        break;
    }
    if (!SPIi_SetEntry(SPI_MODULE_NVRAM, command, 3, arg0, arg1, arg2)) {
        SPIi_ReturnResult(command, SPI_PXI_RESULT_EXCLUSIVE);
    }
}
