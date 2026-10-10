#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void NNS_G2dMapScrToChar256x16Pltt(u16 *, int, int, int, int, int);

void NNS_G2dMapScrToChar256x16Pltt(u16 *dst, int width, int height, int stride, int tile, int palette) {
    struct {
        int y;
        u16 *p;
    } f;

    f.y = 0;
    u16 pal = (u16)(palette << 12);
    for (; f.y < height; f.y++) {
        f.p = dst;
        int x;
        for (x = 0; x < width; x++) {
            *f.p++ = tile++ | pal;
        }
        dst += stride;
    }
}
