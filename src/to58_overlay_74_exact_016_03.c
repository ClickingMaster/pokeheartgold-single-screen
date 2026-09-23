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

int ov74_02233C08(u8 *buffer, u32 *selected, u32 *counter) {
    u32 count0, count1;
    int status0 = ov74_02233B58(0, buffer, &count0);
    int status1 = ov74_02233B58(1, buffer, &count1);
    if (status0 == 1 && status1 == 1) {
        if ((count0 == 0xffffffff && count1 == 0) || (count0 == 0 && count1 == 0xffffffff)) {
            if (count0 + 1 < count1 + 1) {
                *counter = count1;
                *selected = 1;
            } else {
                *counter = count0;
                *selected = 0;
            }
        } else if (count0 < count1) {
            *counter = count1;
            *selected = 1;
        } else {
            *counter = count0;
            *selected = 0;
        }
        return 1;
    }
    if (status0 == 1 && status1 != 1) {
        *counter = count0;
        if (status1 == 0xff) {
            *selected = 0;
            return 0xff;
        }
        *selected = 0;
        return 1;
    }
    if (status1 == 1 && status0 != 1) {
        *counter = count1;
        if (status0 == 0xff) {
            *selected = 1;
            return 0xff;
        }
        *selected = 1;
        return 1;
    }
    if (status0 == 0 && status1 == 0) {
        *counter = 0;
        *selected = 0;
        return 0;
    }
    *counter = 0;
    *selected = 0;
    return 2;
}
