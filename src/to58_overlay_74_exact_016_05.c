#include "global.h"

extern struct { u8 pad[0x68]; u32 field[10]; } sPmAgbRomHeader;
extern u32 ov74_0223D33C[];
extern u32 ov74_0223D34C;
extern u32 sPmAgbCartridgeSpec[];
extern void ov74_02233AB8(int, void *);
extern u16 ov74_02233A88(u32 *, u16);
extern void CTRDG_WriteAndVerifyAgbFlashAsync(u8, void *, u32, void *);
extern void ov74_02233E50(void *);

u16 ov74_02233ACC(int sector);
void *ov74_02233B04(int sector);
int ov74_02233B58(int bank, u8 *buffer, u32 *counter);
int ov74_02233C08(u8 *buffer, u32 *selected, u32 *counter);
void ov74_02233DBC(int sector);

void ov74_02233DBC(int sector) {
    u8 *buffer = (u8 *)ov74_0223D33C[2];
    int size;
    int index;
    ov74_0223D33C[5] = 1;
    MIi_CpuClear32(0, (u32 *)buffer, 0x1000);
    size = ov74_02233ACC(sector);
    MIi_CpuCopy32((u32 *)ov74_02233B04(sector), (u32 *)buffer, size);
    *(u32 *)(buffer + 0xffc) = ov74_0223D33C[4];
    *(u16 *)(buffer + 0xff4) = sector;
    *(u32 *)(buffer + 0xff8) = 0x08012025;
    *(u16 *)(buffer + 0xff6) = ov74_02233A88((u32 *)buffer, ov74_02233ACC(sector));
    index = (sector + (int)ov74_0223D33C[3] + 1) % 14 + 14 * sPmAgbCartridgeSpec[1];
    CTRDG_WriteAndVerifyAgbFlashAsync((u8)index, buffer, 4, (void *)ov74_02233E50);
}
