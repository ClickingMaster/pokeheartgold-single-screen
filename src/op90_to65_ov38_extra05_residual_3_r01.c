#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
int ov38_02224DC8(const char *hay, int hlen, const char *needle, int nlen);
// @fn ov38_02224DC8
int ov38_02224DC8(const char *hay, int hlen, const char *needle, int nlen) {
    int i;
    int j;
    if (hlen < nlen) {
        return -1;
    }
    for (i = 0; i < hlen - nlen + 1; i++) {
        if (needle[0] == hay[i]) {
            const char *p = hay + i;
            for (j = 1; j < nlen; j++) {
                if (p[j] != needle[j]) {
                    break;
                }
            }
            if (j == nlen) {
                return 0;
            }
        }
    }
    return -1;
}
