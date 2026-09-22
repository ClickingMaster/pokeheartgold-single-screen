#ifndef TO57_NEW22_PRIVATE_H
#define TO57_NEW22_PRIVATE_H
#include "global.h"

#include "palette.h"
typedef struct To57New22Entry50 {
    u8 pad_00[8];
    u16 active;
    u8 pad_0A[0x11];
    u8 flag : 7;
    u8 disabled : 1;
    u8 pad_1C[0x34];
} To57New22Entry50;
typedef struct To57New22Work {
    void *data;
    u8 pad_004[0x1e8 - 4];
    PaletteData *palette;
    u8 pad_1ec[0x1f60 - 0x1ec];
    u16 palette0[0x10];
    u16 palette1[0x10];
} To57New22Work;
extern int ov08_0221D5DC(u8 *work, int value);
extern void ov08_02221E6C(void *work, int value, int zero, int flag);
void ov08_02221B1C(u8 *work, const u16 *src);
void ov08_022220AC(u8 *work, unsigned int value);
void ov08_02222524(To57New22Work *work, int state);
u8 ov08_02222564(To57New22Entry50 *entries);
void ov08_022225A4(u8 *work);
void ov08_02221BD0(u16 *dst, const u16 *src, int x, int y, u8 width, u8 height);
#endif
