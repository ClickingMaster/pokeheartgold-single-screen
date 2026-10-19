#ifndef POKEHEARTGOLD_TOUCHPAD_INTERNAL_H
#define POKEHEARTGOLD_TOUCHPAD_INTERNAL_H

#include "global.h"

#include "touchpad.h"

enum TouchpadMode {
    TOUCHPAD_MODE_OFF,
    TOUCHPAD_MODE_RECORDING,
    TOUCHPAD_MODE_SAMPLING,
};

// GF_TouchpadUpdate: how the frame's samples are added to the record buffer
enum TouchpadRecordMode {
    TOUCHPAD_RECORD_MOVES_WRAP = 1, // samples that moved by the threshold; a full buffer starts over
    TOUCHPAD_RECORD_NONE,
    TOUCHPAD_RECORD_MOVES,    // samples that moved by the threshold; stops when the buffer is full
    TOUCHPAD_RECORD_ALL_WRAP, // every sample; a full buffer starts over
    TOUCHPAD_RECORD_ALL,      // every sample; stops when the buffer is full
};

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
