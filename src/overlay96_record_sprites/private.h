#ifndef TO52_R6_W08_PRIVATE_H
#define TO52_R6_W08_PRIVATE_H

#include "global.h"

#define U32AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PAT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct Ov96SpriteTemplate {
    u32 raw[13];
} Ov96SpriteTemplate;

void *ov96_02219E00(void *sys, void *list, u16 a, u16 b, u16 c, u16 d);
void ov96_02219E60(void *raw);
void ov96_02219EE0(void *p, s32 value);

#endif
