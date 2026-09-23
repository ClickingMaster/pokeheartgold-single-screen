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

void *ov74_02233B04(int sector) {
    if (sector == 0) return (u8 *)ov74_0223D33C[2] + 0x1000;
    if (sector >= 1 && sector <= 4) return (u8 *)ov74_0223D33C[2] + 0x2000 + (sector - 1) * 0xf80;
    if (sector >= 5 && sector < 14) return (u8 *)ov74_0223D33C[2] + 0x6000 + (sector - 5) * 0xf80;
    return 0;
}
