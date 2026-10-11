#ifndef POKEHEARTGOLD_TOUCHPAD_INTERNAL_H
#define POKEHEARTGOLD_TOUCHPAD_INTERNAL_H

#include "global.h"

#include "touchpad.h"

typedef struct TouchpadState {
    TPData *buffer; // record buffer of GF_TouchpadStartRecording
    u32 capacity;
    u32 count;         // samples per frame (2 * frequency)
    TPData samples[9]; // TP auto sampling ring
    u32 write;         // next record position
    u16 mode;          // 0: off, 1: recording, 2: sampling only
    u16 paused;        // lid closed
} TouchpadState;

// the frame's valid samples (GF_TouchpadUpdate)
typedef struct TouchOutput {
    u16 count;
    TPData samples[8];
} TouchOutput;

extern TouchpadState gTouchpadState;

void GF_TouchpadInit(void);
void GF_TouchpadResetSampling(void);
BOOL GF_TouchpadStartRecording(TPData *data, u32 capacity, u32 frequency);
BOOL GF_TouchpadStartSampling(int frequency);
void GF_TouchpadSetSamplingState(u16 mode, u8 autoRead, TPData *data, u32 capacity, u32 write, u32 count);
void TouchSamples_Clear(TPData *data, int count);
int GF_TouchpadStopAutoSampling(void);
BOOL GF_TouchpadStopSampling(void);
int GF_TouchpadUpdate(TouchOutput *out, int mode, u32 threshold);
void TouchSamples_Calibrate(TPData *data, u32 count);
void GF_TouchpadResumeOnLidOpen(void);
void GF_TouchpadPauseOnLidClose(void);
int GF_TouchpadStartAutoSampling(u32 frequency);
int GF_TouchpadRecordSamples(int mode, int latest, u32 threshold);
int GF_TouchpadRecordMoves(int mode, int latest, u32 threshold);
int GF_TouchpadRecordAll(int mode, int latest, u32 threshold);
void TouchOutput_Clear(TouchOutput *out);
void TouchOutput_CollectValid(TouchOutput *out, int latest);

#endif // POKEHEARTGOLD_TOUCHPAD_INTERNAL_H
