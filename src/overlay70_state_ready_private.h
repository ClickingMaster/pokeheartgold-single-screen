#ifndef W13_OVERLAY70_PRIVATE_H
#define W13_OVERLAY70_PRIVATE_H
#include "global.h"
typedef struct Ov70GlobalState {
    s32 state;
    s32 result;
} Ov70GlobalState;
extern Ov70GlobalState _02246800;
BOOL ov70_02237F38(void);
#endif
