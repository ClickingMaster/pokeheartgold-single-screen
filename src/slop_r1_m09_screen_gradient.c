#include "slop_r1_m09_screen_gradient.h"

#include <nitro/fx/fx_trig.h>

void sub_02013004(int unused, int color, int centerY, int sampleY, int angle1, int angle2, int *out1, int *out2) {
    int *dest1 = out1;
    int *dest2 = out2;
    int sample = sampleY;
    int originalColor = color;
    struct {
        int y2;
        int x2;
        int x1;
    } values;
    int angle1Saved = angle1;
    int y1;
    int low;
    int high;
#pragma unused(unused)
    values.x1 = color + (0xFFFF * FX_SinCosTable_[((angle1 >> 4) << 1) + 1] >> 0xc);
    y1 = centerY + (0xFFFF * FX_SinCosTable_[(angle1 >> 4) << 1] >> 0xc);
    values.x2 = color + (0xFFFF * FX_SinCosTable_[((angle2 >> 4) << 1) + 1] >> 0xc);
    values.y2 = centerY + (0xFFFF * FX_SinCosTable_[(angle2 >> 4) << 1] >> 0xc);
    if (angle2 - angle1Saved == (0x0000FFFF >> 1)) {
        if (sample >= 0 && sample < centerY) {
            *dest1 = 0;
            *dest2 = 0xff;
            return;
        }
        return;
    }
    if (angle2 >= 0 && angle2 < (0x0000FFFF >> 1)) {
        low = y1 < centerY ? y1 : centerY;
        high = y1 > centerY ? y1 : centerY;
        if (low <= sample && sample <= high) {
            *dest1 = values.x1 + (values.x1 - originalColor) * (sample - y1) / (y1 - centerY);
        } else {
            *dest1 = values.x2 + (values.x2 - originalColor) * (sample - values.y2) / (values.y2 - centerY);
        }
        {
            int value = *dest1;
            if (value > 0xff) {
                value = 0xff;
            } else if (value < 0) {
                value = 0;
            }
            *dest1 = value;
        }
        *dest2 = 0xff;
        return;
    }
    low = y1 < centerY ? y1 : centerY;
    high = y1 > centerY ? y1 : centerY;
    if (low <= sample && sample <= high) {
        *dest1 = values.x1 + (values.x1 - originalColor) * (sample - y1) / (y1 - centerY);
        {
            int value = *dest1;
            if (value > 0xff) {
                value = 0xff;
            } else if (value < 0) {
                value = 0;
            }
            *dest1 = value;
        }
    } else {
        *dest1 = 0;
    }
    low = values.y2 < centerY ? values.y2 : centerY;
    high = values.y2 > centerY ? values.y2 : centerY;
    if (low <= sample && sample <= high) {
        *dest2 = values.x2 + (values.x2 - originalColor) * (sample - values.y2) / (values.y2 - centerY);
        {
            int value = *dest2;
            if (value > 0xff) {
                value = 0xff;
            } else if (value < 0) {
                value = 0;
            }
            *dest2 = value;
        }
    } else {
        *dest2 = *dest1;
    }
    if (*dest1 > *dest2) {
        int temp = *dest1;
        *dest1 = *dest2;
        *dest2 = temp;
    }
}
