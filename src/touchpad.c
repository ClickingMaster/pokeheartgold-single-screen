#include "touchpad.h"

#include "global.h"

#include "system.h"
#include "touchpad_internal.h"

// not declared in lib/include
void TP_RequestAutoSamplingStartAsync(u16, u16, TPData *, u16);
void TP_RequestAutoSamplingStopAsync(void);
void TP_WaitBusy(u32);
u32 TP_CheckBusy(u32);
u16 TP_GetLatestIndexInAuto(void);

TouchpadState gTouchpadState;

void GF_TouchpadInit(void) {
    memset(&gTouchpadState, 0, sizeof(gTouchpadState));
}

void GF_TouchpadResetSampling(void) {
    GF_ASSERT(gTouchpadState.mode == 0);
    gTouchpadState.mode = 0;
}

BOOL GF_TouchpadStartRecording(TPData *data, u32 capacity, u32 frequency) {
    int result;
    if (gTouchpadState.paused) {
        GF_TouchpadSetSamplingState(1, 1, data, capacity, 0, frequency * 2);
        return TRUE;
    }
    if (frequency >= 5 || frequency == 0) {
        return FALSE;
    }
    if (gTouchpadState.mode != 0) {
        return FALSE;
    }
    result = GF_TouchpadStartAutoSampling(frequency);
    if (result == 1) {
        GF_TouchpadSetSamplingState(1, 1, data, capacity, 0, frequency * 2);
        return TRUE;
    }
    return result;
}

BOOL GF_TouchpadStartSampling(int frequency) {
    int result;
    if (gTouchpadState.paused) {
        GF_TouchpadSetSamplingState(2, 1, NULL, 0, 0, frequency * 2);
        return TRUE;
    }
    if ((u32)frequency >= 5 || frequency == 0) {
        return FALSE;
    }
    if (gTouchpadState.mode != 0) {
        return FALSE;
    }
    result = GF_TouchpadStartAutoSampling(frequency);
    if (result == 1) {
        GF_TouchpadSetSamplingState(2, 1, NULL, 0, 0, frequency * 2);
        return TRUE;
    }
    return result;
}

void GF_TouchpadSetSamplingState(u16 mode, u8 autoRead, TPData *data, u32 capacity, u32 write, u32 count) {
    gTouchpadState.mode = mode;
    gSystem.touchpadReadAuto = autoRead;
    gTouchpadState.buffer = data;
    gTouchpadState.capacity = capacity;
    gTouchpadState.count = count;
    gTouchpadState.write = write;
    TouchSamples_Clear(gTouchpadState.samples, 9);
}

void TouchSamples_Clear(TPData *data, int count) {
    int i;
    for (i = 0; i < count; i++) {
        data[i].touch = 0;
    }
}

int GF_TouchpadStopAutoSampling(void) {
    u32 retries = 0;
    u32 busy;
    if (gTouchpadState.mode == 0) {
        return 1;
    }
    do {
        TP_RequestAutoSamplingStopAsync();
        TP_WaitBusy(4);
        busy = TP_CheckBusy(4);
        if (busy) {
            retries++;
        }
    } while (busy && retries <= 5);
    if (retries > 5) {
        return 2;
    }
    return 1;
}

BOOL GF_TouchpadStopSampling(void) {
    int result;
    if (gTouchpadState.paused) {
        GF_TouchpadSetSamplingState(0, 0, NULL, 0, 0, 0);
        return TRUE;
    }
    result = GF_TouchpadStopAutoSampling();
    if (result == 1) {
        GF_TouchpadSetSamplingState(0, 0, NULL, 0, 0, 0);
    }
    return result;
}

int GF_TouchpadUpdate(TouchOutput *out, int mode, u32 threshold) {
    int result = 3;
    u16 latest;
    if (gTouchpadState.paused) {
        if (out) {
            TouchOutput_Clear(out);
        }
        if (mode == 2) {
            return 1;
        }
        return 0;
    }
    if (gTouchpadState.mode) {
        latest = TP_GetLatestIndexInAuto();
        TouchSamples_Calibrate(gTouchpadState.samples, 9);
        if (out) {
            TouchOutput_CollectValid(out, latest);
        }
        if (gTouchpadState.mode == 1) {
            result = GF_TouchpadRecordSamples(mode, latest, threshold);
        } else {
            result = 1;
        }
    }
    return result;
}

void TouchSamples_Calibrate(TPData *data, u32 count) {
    u32 i;
    TPData calibrated;
    for (i = 0; i < count; i++) {
        TP_GetCalibratedPoint(&calibrated, &data[i]);
        data[i] = calibrated;
    }
}

void GF_TouchpadResumeOnLidOpen(void) {
    if (gTouchpadState.paused && gTouchpadState.mode) {
        GF_ASSERT(GF_TouchpadStartAutoSampling(gTouchpadState.count >> 1) == 1);
        gTouchpadState.paused = 0;
    }
}

void GF_TouchpadPauseOnLidClose(void) {
    if (gTouchpadState.paused != 1 && gTouchpadState.mode) {
        GF_ASSERT(GF_TouchpadStopAutoSampling() == 1);
        gTouchpadState.paused = 1;
    }
}

int GF_TouchpadStartAutoSampling(u32 frequency) {
    int retries = 0;
    u32 busy;
    do {
        TP_RequestAutoSamplingStartAsync(0, (u16)frequency, gTouchpadState.samples, 9);
        TP_WaitBusy(2);
        busy = TP_CheckBusy(2);
        if (busy) {
            retries++;
        }
    } while (busy && retries <= 5);
    if (retries > 5) {
        return 2;
    }
    return 1;
}

int GF_TouchpadRecordSamples(int mode, int latest, u32 threshold) {
    switch (mode) {
    case 1:
    case 3:
        return GF_TouchpadRecordMoves(mode, latest, threshold);
    case 4:
    case 5:
        return GF_TouchpadRecordAll(mode, latest, threshold);
    default:
        return 1;
    }
}

int GF_TouchpadRecordMoves(int mode, int latest, u32 threshold) {
    u32 i;
    for (i = 0; i < gTouchpadState.count; i++) {
        s16 index = latest - gTouchpadState.count + i + 1;
        if (index < 0) {
            index += 9;
        }
        if (gTouchpadState.samples[index].touch == 1 && gTouchpadState.samples[index].validity == 0) {
            int dx, dy, prev;
            dx = gTouchpadState.samples[index].x;
            prev = gTouchpadState.buffer[gTouchpadState.write - 1].x;
            dx = prev >= dx ? prev - dx : dx - prev;
            dy = gTouchpadState.samples[index].y;
            prev = gTouchpadState.buffer[gTouchpadState.write - 1].y;
            dy = prev >= dy ? prev - dy : dy - prev;
            if ((u32)dx >= threshold || (u32)dy >= threshold) {
                gTouchpadState.buffer[gTouchpadState.write] = gTouchpadState.samples[index];
                gTouchpadState.write++;
                if (gTouchpadState.write >= gTouchpadState.capacity) {
                    if (mode == 1) {
                        gTouchpadState.write %= gTouchpadState.capacity;
                    } else {
                        return -1;
                    }
                }
            }
        }
    }
    return gTouchpadState.write;
}

int GF_TouchpadRecordAll(int mode, int latest, u32 threshold) {
    u32 i;
    for (i = 0; i < gTouchpadState.count; i++) {
        s16 index = latest - gTouchpadState.count + i + 1;
        if (index < 0) {
            index += 9;
        }
        gTouchpadState.buffer[gTouchpadState.write] = gTouchpadState.samples[index];
        gTouchpadState.write++;
        if (gTouchpadState.write >= gTouchpadState.capacity) {
            if (mode == 4) {
                gTouchpadState.write %= gTouchpadState.capacity;
            } else {
                return -1;
            }
        }
    }
    return gTouchpadState.write;
}

void TouchOutput_Clear(TouchOutput *out) {
    int i;
    out->count = 0;
    for (i = 0; i < 8; i++) {
        out->samples[i].validity = 0;
        out->samples[i].touch = 0;
        out->samples[i].x = 0;
        out->samples[i].y = 0;
    }
}

void TouchOutput_CollectValid(TouchOutput *out, int latest) {
    u32 i;
    TouchOutput_Clear(out);
    for (i = 0; i < gTouchpadState.count; i++) {
        s16 index = latest - gTouchpadState.count + i + 1;
        if (index < 0) {
            index += 9;
        }
        if (gTouchpadState.samples[index].validity == 0) {
            out->samples[out->count] = gTouchpadState.samples[index];
            out->count++;
        }
    }
}
