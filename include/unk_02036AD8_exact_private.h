#ifndef TO50_UNK_02036AD8_PRIVATE_H
#define TO50_UNK_02036AD8_PRIVATE_H

#include "global.h"

typedef struct CommManagerR1E {
    u8 initialized;
    u8 updateLock;
    u8 pad02[2];
    void *allocation;
    u8 *work;
} CommManagerR1E;

typedef struct CommRandomR1E {
    s64 state;
    s64 multiplier;
    s64 increment;
} CommRandomR1E;

extern CommManagerR1E _021D4140;
extern volatile u8 _0210F900[2];

extern int sub_0203993C(void);
extern int sub_02034044(int);
extern int sub_0203772C(u16);
extern int sub_02033FC4(u16);
extern int sub_02033298(void);
extern int Comm_GetLocalNetId(void);
extern int Comm_IsNetIdConnected(int);
extern int sub_02033800(void *, u16, int, void (*)(void *));
extern void sub_02036AA0(void *);
extern int sub_02033250(void);
extern void sub_0203690C(int, u8 *, u16);

void sub_02036AD8(void);
void sub_02036BE4(void);

#endif
