#ifndef TO50_OVERLAY92_R6_PRIVATE_H
#define TO50_OVERLAY92_R6_PRIVATE_H
#include "global.h"

#include "camera.h"
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
typedef struct {
    u8 pad500[0x500];
    u8 object[0x14];
    u32 active;
    u8 pad518[0x70];
    u8 anims[1][0x14];
} Ov92Work;
extern const VecFx32 ov92_02263B5C;
void sub_020181E0(void *, void *);
void sub_020181D4(void *, void *);
void ov92_0225DD48(void *, int);
void ov92_0225DD88(void *);
#endif
