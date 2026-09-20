#ifndef TO55_SOL_R2_T10_PRIVATE_H
#define TO55_SOL_R2_T10_PRIVATE_H

#include "global.h"

typedef struct Ov07T10Slot {
    u32 unk00;
    s16 xy[2];
    void *pokepic;
    void *sprite;
    u32 index;
} Ov07T10Slot;

typedef struct Ov07T10Work {
    u8 state[4];
    void *owner;
    void *field08;
    void *field0C;
    void *field10;
    void *field14;
    void *field18;
} Ov07T10Work;

void *ov07_0221C530(void *);
void *ov07_0221C514(void *);
void *ov07_0221C528(void *);
void *ov07_0221C4A0(void *);
void *ov07_0221FA78(void *);
u32 ov07_0221C468(void *);
u32 ov07_0221C470(void *);
void *ov07_0221C4E8(void *, int);
void *ov07_0221FA48(void *, u32);
void *ov07_0221FA04(void *, u32);
BOOL ov07_0221FAB0(void *);
u32 ov07_0223197C(void *, u32);
u32 ov07_022319E0(void *);
u32 ov07_02231958(void *, u32);
void ov07_02231FA0(void *, s16 *);
void ov07_02231FD8(void *, s16 *);
void ov07_02231FE4(void *, Ov07T10Work *);
void ov07_02232020(void *, u32, Ov07T10Slot *, int *);
void ov07_022323FC(void *, u32, Ov07T10Slot *, int *);

#endif
