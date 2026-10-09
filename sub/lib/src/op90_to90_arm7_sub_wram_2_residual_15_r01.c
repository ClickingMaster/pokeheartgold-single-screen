#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern void SPIi_ReturnResult(u16, u32);
extern int SPIi_SetEntry(u32, u32, u32, ...);
typedef struct {
    u16 words[16];
} NvramWork;
NvramWork nvramw;
void NVRAM_AnalyzeCommand(u32 packet);
// @fn NVRAM_AnalyzeCommand
void NVRAM_AnalyzeCommand(u32 packet) {
    u16 cmd;
    u32 arg0, arg1, arg2;
    if (packet & 0x2000000) {
        int i;
        for (i = 0; i < 16; i++) {
            nvramw.words[i] = 0;
        }
    }
    nvramw.words[(packet & 0xf0000) >> 16] = (u16)packet;
    if (!(packet & 0x1000000)) {
        return;
    }
    cmd = (nvramw.words[0] & 0xff00) >> 8;
    switch (cmd) {
    case 0x22:
    case 0x2c:
        arg2 = ((u32)nvramw.words[1] << 8) | ((u32)nvramw.words[0] << 24) | ((u32)(nvramw.words[2] & 0xff00) >> 8);
        if (arg2 < 0x2000000 || arg2 >= 0x2800000) {
            SPIi_ReturnResult(cmd, 2);
            return;
        }
        break;
    case 0x23:
    case 0x24:
        arg2 = ((u32)nvramw.words[4] << 16) | nvramw.words[5];
        if (arg2 < 0x2000000 || arg2 >= 0x2800000) {
            SPIi_ReturnResult(cmd, 2);
            return;
        }
        arg0 = (((u32)nvramw.words[0] << 24) >> 8) | nvramw.words[1];
        arg1 = ((u32)nvramw.words[2] << 16) | nvramw.words[3];
        break;
    case 0x25:
    case 0x26:
        arg2 = ((u32)nvramw.words[3] << 16) | nvramw.words[4];
        if (arg2 < 0x2000000 || arg2 >= 0x2800000) {
            SPIi_ReturnResult(cmd, 2);
            return;
        }
        arg0 = (((u32)nvramw.words[0] << 24) >> 8) | nvramw.words[1];
        arg1 = nvramw.words[2];
        break;
    case 0x27:
    case 0x28:
        arg0 = (((u32)nvramw.words[0] << 24) >> 8) | nvramw.words[1];
        break;
    }
    if (!SPIi_SetEntry(1, cmd, 3, arg0, arg1, arg2)) {
        SPIi_ReturnResult(cmd, 4);
    }
}
