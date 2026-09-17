#ifndef TO51_OVERLAY119_TRANSITION_F020_PRIVATE_H
#define TO51_OVERLAY119_TRANSITION_F020_PRIVATE_H

#include "global.h"

#include "sprite.h"

typedef struct Camera Camera;

typedef struct Ov119InterpR2 {
    s32 value;
    s32 start;
    s32 delta;
    s32 frame;
    s32 duration;
} Ov119InterpR2;

typedef struct Ov119WorkR2 {
    Ov119InterpR2 scale;
    u8 pad014[4];
    Ov119InterpR2 rotation;
    Ov119InterpR2 movement;
    u8 pad040[4];
    void *effect;
    u8 spriteResources[0x13C];
    void *resource184;
    u8 pad188[0x30];
    Sprite *sprites[2];
    Camera *camera;
    Ov119InterpR2 cameraInterp;
    u8 pad1D8[8];
} Ov119WorkR2;

typedef struct Ov119TaskR2 {
    u32 state;
    u32 done;
    u32 unused08;
    Ov119WorkR2 *work;
    void *fieldSystem;
    u32 *completionFlag;
    u8 pad018[8];
    void *spriteManager;
} Ov119TaskR2;

void *Heap_Alloc(enum HeapID heapId, u32 size);
void *memset(void *dest, int value, u32 size);
void GfGfx_EngineATogglePlanes(u32 plane, BOOL enable);
void ov01_021EFCF8(s32, s32, s32, u32 *, s32);
void ov01_021EFEC8(Ov119InterpR2 *, s32, s32, s32, s32);
void ov01_021EFE34(Ov119InterpR2 *, s32, s32, s32);
BOOL ov01_021EFE44(Ov119InterpR2 *);
BOOL ov01_021EFF28(Ov119InterpR2 *);
BOOL ov01_021EFE30(Ov119TaskR2 *);
void *ov01_021EFFEC(void);
void ov01_021F05C4(void *, s32, s32);
void ov01_021F05F4(void *);
void ov01_021F0614(void *, void *, void *, u32, s32, u32, u32, u32, u32);
void ov01_021F06EC(void *, void *);
Sprite *ov01_021F0718(void *, void *, s32, s32, s32, s32);
void ov01_021F074C(VecFx32 *, s32, s32, s32);
void ov01_021F0050(Ov119TaskR2 *, void *, s32, s32, s32, s32, s32);
void ov01_021F0028(void *);
void ov01_021EFCDC(Ov119TaskR2 *, void *);
void Sprite_SetDrawPriority(Sprite *, u32);
void Sprite_SetAnimCtrlSeq(Sprite *, int);
void Camera_SetDistance(s32, Camera *);
s32 Camera_GetDistance(Camera *);
void sub_0200FC20(int);

void ov119_0225F020(void *, Ov119TaskR2 *);
void ov119_0225F37C(void *, Ov119TaskR2 *);
void ov119_0225F670(void *, Ov119TaskR2 *);
void ov119_0225FA2C(void *, Ov119TaskR2 *);

typedef struct Ov119WorkF37C {
    Ov119InterpR2 position;
    Ov119InterpR2 rotation;
    void *effect;
    u8 spriteResources[0x13c];
    void *resource168;
    u8 pad16c[0x30];
    Sprite *sprites[2];
    Camera *camera;
    Ov119InterpR2 cameraInterp;
    u8 pad1bc[8];
} Ov119WorkF37C;
void *ov01_021F0250(void);
BOOL ov01_021EFE70(Ov119InterpR2 *, s32, s32, s32);
BOOL ov01_021EFE80(Ov119InterpR2 *);
void ov01_021F02C4(Ov119TaskR2 *, void *, s32, s32, s32);
void ov01_021F029C(void *);

#endif

typedef struct Ov119WorkF670 {
    Ov119InterpR2 scale;
    u8 pad014[4];
    Ov119InterpR2 blend;
    Ov119InterpR2 rotation;
    u8 effect[0x10];
    u8 spriteResources[0x13c];
    void *resource18c;
    u8 pad190[0x30];
    Sprite *sprites[2];
    Camera *camera;
    Ov119InterpR2 cameraInterp;
    u8 pad1e0[4];
    s32 timer;
} Ov119WorkF670;
void ov01_021F12B4(void *, int);
void ov01_021F12E8(void *, int, int, int, int, int, int, int, int);
void ov01_021F12D0(void *);
void ov01_021F1310(void *, int);
void sub_0200FBF4(int, int);
BOOL IsPaletteFadeFinished(void);
void BeginNormalPaletteFade(int, int, int, int, int, int, int);
void G2x_SetBlendAlpha_(u32, int, int, int, int);
