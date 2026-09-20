#ifndef TO56_R2_W04_OVERLAY07_CALLBACKS_PRIVATE_H
#define TO56_R2_W04_OVERLAY07_CALLBACKS_PRIVATE_H

#include "global.h"

typedef struct Ov07CallbackContext {
    u8 pad00[0x18];
    u8 *script;
} Ov07CallbackContext;

typedef struct Ov07SoundEffect {
    u8 kind;
    u8 pad01[2];
    u8 byte03;
    u8 byte04;
    u8 pad05[3];
    s32 x;
    s32 y;
    s32 z;
    s32 value14;
    u8 byte18;
    u8 pad19;
    u16 sequence;
    u8 pad1C[0x20];
} Ov07SoundEffect;

void PlaySE(u16);
void StopSE(u16, int);
void sub_020061B4(u16, u32, int);
void sub_020061EC(int);
void *ov07_0221C53C(Ov07CallbackContext *);
void ov07_0221C56C(Ov07CallbackContext *, Ov07SoundEffect *);
int ov07_0221F8C8(Ov07CallbackContext *, s8);
int ov07_0221F980(s8, s8, s8);

void ov07_0221F098(Ov07CallbackContext *);
void ov07_0221F0B0(Ov07CallbackContext *);
void ov07_0221F0CC(Ov07CallbackContext *);
void ov07_0221F104(Ov07CallbackContext *);
void ov07_0221F120(Ov07CallbackContext *);
void ov07_0221F1C0(Ov07CallbackContext *);
void ov07_0221F238(Ov07CallbackContext *);
void ov07_0221F2DC(Ov07CallbackContext *);
void ov07_0221F340(Ov07CallbackContext *);

#endif
