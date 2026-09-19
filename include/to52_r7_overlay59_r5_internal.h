#ifndef TO52_R7_W08_PRIVATE_H
#define TO52_R7_W08_PRIVATE_H

#include "global.h"

typedef struct Ov59Work {
    u8 raw[0x254];
} Ov59Work;

#define U8AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define PTRAT(p, o) (*(void **)((u8 *)(p) + (o)))

void ov59_02238C40(Ov59Work *work, int msgNo);
void ov59_02238C7C(Ov59Work *work);
void ov59_02238CFC(Ov59Work *work, int msgNo, int speed);

#endif
