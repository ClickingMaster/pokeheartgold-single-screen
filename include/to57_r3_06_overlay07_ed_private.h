#ifndef TO57_R3_06_PRIVATE_H
#define TO57_R3_06_PRIVATE_H

#include "global.h"

typedef struct Ov07EdWork {
    u8 pad00[0x0D];
    u8 kind;
    s8 x;
    s8 y;
    u32 value;
    u32 callback;
    u32 param;
} Ov07EdWork;

typedef struct Ov07EdOwner {
    u8 pad00[0x18];
    u32 *script;
    u8 pad1C[0x164];
    u8 transform[0x0C];
} Ov07EdOwner;

typedef BOOL (*Ov07EdCallback)(void *, Ov07EdWork *);

extern Ov07EdCallback ov07_02234BC0[];
extern const char ov07_0223789C[];

Ov07EdWork *ov07_0221DF1C(void *);

extern u32 ov07_0221C4A8(void *p, int i);
void *SysTask_CreateOnMainQueue(void (*)(void *, void *), void *, u32);
void SysTask_Destroy(void *);
void Heap_Free(void *);
void GF_AssertFail(void);
void *ov07_0221DD8C(void *, int);
void ov07_0221ED14(void *, Ov07EdWork *);
void ov07_0221ED44(Ov07EdOwner *);
void ov07_0221ED94(void *);
void ov07_0221EE74(Ov07EdOwner *);
void ov07_0221EEC0(void *);

#endif
