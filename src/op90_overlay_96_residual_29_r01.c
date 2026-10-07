#pragma force_active on
#include "global.h"
typedef struct {
    u8 pad[0x10];
    s32 origin;
    u8 pad14[4];
    s32 middle;
    u8 pad1c[0xc];
    s32 span;
} Host;
void ov96_021EB0A4(Host *host, s32 x, s32 y, s32 *outX, s32 *outY);
// @fn ov96_021EB0A4
void ov96_021EB0A4(Host *host, s32 x, s32 y, s32 *outX, s32 *outY) {
    y = y + host->origin - host->middle * 2;
    *outX = x;
    *outY = host->span + (y + (host->middle * 2 - host->span) / 2);
}
