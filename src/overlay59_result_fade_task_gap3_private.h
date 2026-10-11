#ifndef OV59_R13_G3_PRIVATE_H
#define OV59_R13_G3_PRIVATE_H
#include "global.h"

#include "heap.h"
#include "pokepic.h"
#include "screen_fade.h"
#include "sys_task_api.h"
#include "sound_playback.h"

typedef struct Ov59R13G3Scene {
    u8 pad00[0x20];
    Pokepic *pic;
    void *particle;
} Ov59R13G3Scene;

typedef struct Ov59R13G3Work {
    enum HeapID heap;
    u8 pad04[0x4B];
    u8 activeCount;
    u8 pad50[0x28];
    Ov59R13G3Scene *scene;
} Ov59R13G3Work;

typedef struct Ov59R13G3TaskData {
    Ov59R13G3Work *work;
    u8 timer;
    u8 state;
    u16 pad06;
} Ov59R13G3TaskData;

void ov59_0223C16C(SysTask *task, Ov59R13G3TaskData *data);
void ov59_0223C3C0(Ov59R13G3Scene *scene, int value);
BOOL ov59_0223C3DC(Ov59R13G3Scene *scene);
void sub_020154B8(void *particle);
#endif
