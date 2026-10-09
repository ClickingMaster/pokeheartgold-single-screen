#include "touch_sampling_internal.h"

TouchState _021D2198;

int sub_020213F8(int mode, int latest, u32 threshold)
{
    u32 i;
    for (i = 0; i < STATE.count; i++) {
        s16 index = latest - STATE.count + i + 1;
        if (index < 0) {
            index += 9;
        }
        if (STATE.samples[index].touch == 1 && STATE.samples[index].validity == 0) {
            int dx, dy, prev;
            dx = STATE.samples[index].x;
            prev = STATE.buffer[STATE.write - 1].x;
            dx = prev >= dx ? prev - dx : dx - prev;
            dy = STATE.samples[index].y;
            prev = STATE.buffer[STATE.write - 1].y;
            dy = prev >= dy ? prev - dy : dy - prev;
            if ((u32)dx >= threshold || (u32)dy >= threshold) {
                STATE.buffer[STATE.write] = STATE.samples[index];
                STATE.write++;
                if (STATE.write >= STATE.capacity) {
                    if (mode == 1) {
                        STATE.write %= STATE.capacity;
                    } else {
                        return -1;
                    }
                }
            }
        }
    }
    return STATE.write;
}

int sub_020214B0(int mode, int latest, u32 threshold)
{
    u32 i;
    for (i = 0; i < STATE.count; i++) {
        s16 index = latest - STATE.count + i + 1;
        if (index < 0) {
            index += 9;
        }
        STATE.buffer[STATE.write] = STATE.samples[index];
        STATE.write++;
        if (STATE.write >= STATE.capacity) {
            if (mode == 4) {
                STATE.write %= STATE.capacity;
            } else {
                return -1;
            }
        }
    }
    return STATE.write;
}

void sub_02021528(TouchOutput *out)
{
    int i;
    out->count = 0;
    for (i = 0; i < 8; i++) {
        out->samples[i].validity = 0;
        out->samples[i].touch = 0;
        out->samples[i].x = 0;
        out->samples[i].y = 0;
    }
}

void sub_02021540(TouchOutput *out, int latest)
{
    u32 i;
    sub_02021528(out);
    for (i = 0; i < STATE.count; i++) {
        s16 index = latest - STATE.count + i + 1;
        if (index < 0) {
            index += 9;
        }
        if (STATE.samples[index].validity == 0) {
            out->samples[out->count] = STATE.samples[index];
            out->count++;
        }
    }
}
