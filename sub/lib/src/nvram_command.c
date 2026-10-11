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
    if (packet & 0x2000000) {
        int i;
        for (i = 0; i < 16; i++) {
            nvramw.command[i] = 0;
        }
    }
    nvramw.command[(packet & 0xf0000) >> 16] = (u16)packet;
    if (!(packet & 0x1000000)) {
        return;
    }
    command = (nvramw.command[0] & 0xff00) >> 8;
    switch (command) {
    case 0x22:
    case 0x2c:
        arg2 = ((u32)nvramw.command[1] << 8) | ((u32)nvramw.command[0] << 24) | ((u32)(nvramw.command[2] & 0xff00) >> 8);
        if (!IS_MAIN_MEMORY(arg2)) {
            SPIi_ReturnResult(command, 2);
            return;
        }
        break;
    case 0x23:
    case 0x24:
        arg2 = ((u32)nvramw.command[4] << 16) | nvramw.command[5];
        if (!IS_MAIN_MEMORY(arg2)) {
            SPIi_ReturnResult(command, 2);
            return;
        }
        arg0 = (((u32)nvramw.command[0] << 24) >> 8) | nvramw.command[1];
        arg1 = ((u32)nvramw.command[2] << 16) | nvramw.command[3];
        break;
    case 0x25:
    case 0x26:
        arg2 = ((u32)nvramw.command[3] << 16) | nvramw.command[4];
        if (!IS_MAIN_MEMORY(arg2)) {
            SPIi_ReturnResult(command, 2);
            return;
        }
        arg0 = (((u32)nvramw.command[0] << 24) >> 8) | nvramw.command[1];
        arg1 = nvramw.command[2];
        break;
    case 0x27:
    case 0x28:
        arg0 = (((u32)nvramw.command[0] << 24) >> 8) | nvramw.command[1];
        break;
    }
    if (!SPIi_SetEntry(SPI_MODULE_NVRAM, command, 3, arg0, arg1, arg2)) {
        SPIi_ReturnResult(command, 4);
    }
}
