#include "global.h"
typedef struct Ov93Copy16 { u32 a,b,c,d; } Ov93Copy16;
typedef struct Ov93Queue { u8 pad[0x2f2c]; Ov93Copy16 entries[8]; s32 head, tail; } Ov93Queue;
u32 ov93_02260F3C(Ov93Queue *p, const Ov93Copy16 *src);
u32 ov93_02260F3C(Ov93Queue *p, const Ov93Copy16 *src) {
    if (p->tail - p->head >= 8) return 0;
    p->entries[p->tail % 8] = *src;
    p->tail++;
    return 1;
}
