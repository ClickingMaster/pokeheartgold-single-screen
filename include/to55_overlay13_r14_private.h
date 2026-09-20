#ifndef TO55_T15_PRIVATE_H
#define TO55_T15_PRIVATE_H

#include "nitro/os/alarm.h"
#include "nitro/os/message.h"

#include "global.h"

typedef void *(*Ov13AllocCallback)(u32 size);
typedef void (*Ov13FreeCallback)(void *ptr);

typedef struct Ov13Runtime {
    Ov13AllocCallback alloc;
    void *unk04;
    void *allocation;
    Ov13FreeCallback free;
} Ov13Runtime;

extern Ov13Runtime ov13_0224DD80;
extern OSMessageQueue ov13_0224DDA0;
extern OSMessage ov13_0224DD90[4];

BOOL ov13_02222BF4(void *message);
BOOL ov13_02222C08(void *message);
BOOL ov13_022236B8(BOOL (*callback)(void *), void *allocation, u32 size);
BOOL ov13_022235DC(void);
BOOL ov13_022235A4(void);
BOOL ov13_02223478(u32, u32, u32, u32);
int ov13_02223434(void *, u32);
BOOL ov13_02223568(void);
void ov13_02222B34(const void *, void *);

int ov13_02222C1C(Ov13AllocCallback alloc, Ov13FreeCallback free);
int ov13_02222CE0(void);
int ov13_02222D78(void);
int ov13_02222DB0(void **output);

#endif
