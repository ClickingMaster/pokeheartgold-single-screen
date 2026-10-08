#pragma force_active on
#include "global.h"
#include "gf_gfx_loader.h"
void ov49_0225C368(NNSG2dPaletteData *pltt);
#pragma opt_propagation off
// @fn ov49_0225C368
void ov49_0225C368(NNSG2dPaletteData *pltt) {
    int i;
    int n = pltt->szByte / 2;
    u16 *p = pltt->pRawData;
    int r, g, b, gray;
    i = 0;
    if (n > 0) {
        do {
            r = *p & 0x1F;
            g = (*p >> 5) & 0x1F;
            b = (*p >> 10) & 0x1F;
            gray = (r * 76 + g * 151 + b * 29) >> 8;
            *p = (((gray * 128) >> 8) << 10) | (((gray * 200) >> 8) << 5) | ((gray * 256) >> 8);
            p++;
        } while (++i < n);
    }
}
#pragma opt_propagation reset
