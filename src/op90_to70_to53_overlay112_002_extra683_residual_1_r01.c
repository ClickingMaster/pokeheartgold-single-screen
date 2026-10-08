#pragma force_active on
#include "global.h"
typedef struct { u8 cmd; u8 sub; u8 crcLo; u8 crcHi; u32 arg; u8 data[1]; } Pkt;
extern Pkt ov112_021FFA18;
extern u8 ov112_021FFA20[];
int ov112_021E5900(void *buf, int len);
void CARD_SpiWaitWriteRange(void *buf, int len);
void ov112_021E59B4(const u8 *src, int len, u8 cmd, u8 sub, u32 arg);

// @fn ov112_021E59B4
void ov112_021E59B4(const u8 *src, int len, u8 cmd, u8 sub, u32 arg) {
    int i;
    int crc;
    u8 *dst;
    Pkt *h;
    h = &ov112_021FFA18;
    h->cmd = cmd;
    h->sub = sub;
    h->arg = arg;
    h->crcLo = 0;
    h->crcHi = 0;
    dst = ov112_021FFA20;
    for (i = 0; i < len;) {
        u8 b = *src;
        src++;
        i++;
        *dst = b;
        dst++;
    }
    len = (u8)(len + 8);
    crc = ov112_021E5900(&ov112_021FFA18, len);
    h->crcLo = crc;
    h->crcHi = crc >> 8;
    for (i = 0; i < len; i++) {
        ((u8 *)&ov112_021FFA18)[i] ^= 0xAA;
    }
    CARD_SpiWaitWriteRange(&ov112_021FFA18, len);
}
