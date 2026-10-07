#ifndef OVERLAY07_SIX_SPRITE_WORK_PRIVATE_H
#define OVERLAY07_SIX_SPRITE_WORK_PRIVATE_H
#include "global.h"
#include "heap.h"
#include "sprite_system.h"
#include "sys_task.h"
typedef struct H04Work H04Work;

/* The native template copy owns thirteen initialized script/template words. */
struct T { u32 words[13]; };
struct Data { u8 pad[192]; u8 first[32]; u8 second[1]; };
struct Context { u8 pad[192]; struct Data *data; u8 gap[64]; struct T templ; };
union Ov07TemplateBridge { struct T native; ManagedSpriteTemplate managed; };
#include "overlay07_linear_motion_private.h"
typedef struct Ov07MotionSlot {
    W02LinearMotion motion;
    u8 reserved[12];
} Ov07MotionSlot;
typedef struct Ov07SixSpriteWork {
    u8 state, p[7], count, b, a, pb;
    void *ctx;
    SpriteSystem *system;
    SpriteManager *manager;
    ManagedSprite *sprite[6];
    int index;
    ManagedSprite *first, *second;
    Ov07MotionSlot interp1, interp2;
} Ov07SixSpriteWork;
typedef char Ov07SixSpriteWorkSize[(sizeof(Ov07SixSpriteWork) == 132) ? 1 : -1];
typedef char Ov07TemplateBridgeSize[(sizeof(union Ov07TemplateBridge) == 52) ? 1 : -1];
void *ov07_0221BFD0(void *);
u32 ov07_0221C4A8(void *, int);
void ov07_0221F9E8(struct T *, struct Context *);
void ov07_02231E08(void *, int, int);
void ov07_0221C3F4(void *, void *, void *, u32);
void ov07_0221C448(void *, void *);
void ov07_02222268(W02LinearMotion *, s16, s16, s16, s16, u16);
BOOL ov07_022222F0(W02LinearMotion *, void *);
void ov07_02229FC4(SysTask *, void *);
void ov07_0222A1F0(H04Work *, void *, void *, void *);
#endif
