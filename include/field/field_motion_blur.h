#ifndef POKEHEARTGOLD_FIELD_MOTION_BLUR_H
#define POKEHEARTGOLD_FIELD_MOTION_BLUR_H

#include "global.h"

#include "heap.h"
#include "sys_task.h"

// Display capture into a VRAM bank, blended with the live picture every frame: motion blur.
typedef struct FieldCaptureConfig {
    GXDispMode dispMode; // a VRAM display mode: the bank that receives the capture
    GXBGMode bgMode;
    GXBG0As bg0As;
    GXCaptureSize size;
    GXCaptureMode mode;
    GXCaptureSrcA srcA;
    GXCaptureSrcB srcB;
    GXCaptureDest dest;
    int eva;
    int evb;
    enum HeapID heapID;
} FieldCaptureConfig;

// The environment of the capture task.
typedef struct FieldCapture {
    u32 savedLcdcBanks;
    FieldCaptureConfig config;
    BOOL started;
    SysTask *task;
} FieldCapture;

typedef struct FieldMotionBlur {
    FieldCapture *capture;
} FieldMotionBlur;

FieldCapture *FieldCapture_Start(const FieldCaptureConfig *config);
void FieldCapture_Stop(FieldCapture **capture, GXDispMode dispMode, GXBGMode bgMode, GXBG0As bg0As);
void FieldCapture_Task(SysTask *task, FieldCapture *capture);
void FieldCapture_StartTask(SysTask *task, FieldCapture *capture);
void FieldCapture_Init(FieldCaptureConfig *config);
FieldMotionBlur *FieldMotionBlur_Start(int eva, int evb);
void FieldMotionBlur_Stop(FieldMotionBlur **motionBlur);

#endif // POKEHEARTGOLD_FIELD_MOTION_BLUR_H
