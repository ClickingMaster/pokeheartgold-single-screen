#ifndef TO52_R6_W14_PRIVATE_H
#define TO52_R6_W14_PRIVATE_H
#include "global.h"

typedef struct Ov41Dispatch14 {
    void *object;
    s32 mode;
} Ov41Dispatch14;

BOOL ov41_02245F14(void *, s32, s32, void *);
BOOL ov41_022497A8(void *, s32, s32, void *);
void ov41_02245F9C(void *, s16, s16);
void ov41_0224971C(void *, s32, s32);
void ov41_02245FA8(void *, s32 *, s32 *);
void ov41_02249780(void *);
void ov41_022497A0(void *, s32 *, s32 *);

BOOL ov41_02249AA8(void *, s32, s32, void **);
void ov41_02249AF4(void *, s32, s32);
void ov41_02249B44(void *, s32 *, s32 *);
#endif
