#ifndef TO55_W16_PRIVATE_H
#define TO55_W16_PRIVATE_H

#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "heap.h"
#include "sprite.h"

typedef struct Ov119Interp {
    s32 value;
    s32 start;
    s32 delta;
    s32 frame;
    s32 duration;
} Ov119Interp;

typedef struct Ov119TailWork {
    u8 spriteResources[0x13c];
    void *resource13c;
    u8 pad140[0x30];
    Sprite *sprites[3];
    Ov119Interp position[3];
    Ov119Interp rotation[3];
    u32 active[3];
    Window *window;
    void *effect;
    Camera *camera;
    Ov119Interp cameraInterp;
    u8 pad220[4];
    s16 timer;
    u8 pad226[2];
} Ov119TailWork;

typedef struct Ov119Task {
    u32 state;
    u32 done;
    u32 unused08;
    Ov119TailWork *work;
    void *fieldSystem;
    u32 *completionFlag;
    u8 pad018[8];
    void *spriteManager;
} Ov119Task;

void ov01_021EFCF8(s32, s32, s32, u32 *, s32);
void ov01_021EFEC8(Ov119Interp *, s32, s32, s32, s32);
void ov01_021EFE34(Ov119Interp *, s32, s32, s32);
BOOL ov01_021EFE44(Ov119Interp *);
BOOL ov01_021EFF28(Ov119Interp *);
void ov01_021EFCDC(Ov119Task *, void *);
void ov01_021F05C4(void *, s32, s32);
void ov01_021F05F4(void *);
void ov01_021F0614(void *, void *, void *, u32, s32, u32, u32, u32, u32);
void ov01_021F06EC(void *, void *);
Sprite *ov01_021F0718(void *, void *, s32, s32, s32, s32);
void ov01_021F074C(VecFx32 *, s32, s32, s32);
void *ov01_021F09BC(enum HeapID);
void ov01_021F09EC(void *);
void ov01_021F0A0C(void *, u8, u8, void *, u8);
BOOL ov01_021F0A4C(void *);
void sub_0200FC20(int);
void ov119_02260258(void *, Ov119Task *);

#endif
