#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
extern const char ov38_02224E78[];
int ov38_02220100(u8 *dst, const u8 *src, int len);
int ov38_022201C8(char *out, const u8 *in, int len);
// @fn ov38_022201C8
int ov38_022201C8(char *out, const u8 *in, int len) {
    u8 buf[3];
    int n;
    u8 i0, i1, i2, i3;
    int total = 0;
    n = ov38_02220100(buf, in, len > 3 ? 3 : len);
    while (n > 0) {
        if (len < 3) {
            buf[2] = 0;
        }
        if (len < 2) {
            buf[1] = 0;
        }
        i0 = buf[0] >> 2;
        i1 = ((buf[0] << 4) | (buf[1] >> 4)) & 0x3F;
        i2 = ((buf[1] << 2) | (buf[2] >> 6)) & 0x3F;
        i3 = buf[2] & 0x3F;
        out[0] = ov38_02224E78[i0];
        out[1] = ov38_02224E78[i1];
        out[2] = n > 1 ? ov38_02224E78[i2] : '*';
        out[3] = n > 2 ? ov38_02224E78[i3] : '*';
        in += n;
        out += 4;
        total += 4;
        len -= n;
        if (len == 0) {
            break;
        }
        n = ov38_02220100(buf, in, len > 3 ? 3 : len);
    }
    *out = '\0';
    return total;
}
