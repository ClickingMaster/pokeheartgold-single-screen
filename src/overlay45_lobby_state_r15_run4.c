#include "overlay45_lobby_state_r15_private.h"

void WiFiPlazaProfile_UpdateChecksum(u8 *p, void *ctx) {
    *(u32 *)&p[0xb4] = SaveArray_CalcCRC16(ctx, &p[0x20], 0x94);
}

BOOL WiFiPlazaProfile_IsChecksumValid(u8 *p, void *ctx) {
    u16 crc = SaveArray_CalcCRC16(ctx, &p[0x20], 0x94);
    if (crc != *(u32 *)&p[0xb4]) {
        GF_AssertFail();
        return FALSE;
    }
    return TRUE;
}
