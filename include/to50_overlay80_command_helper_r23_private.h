#ifndef TO50_OV80_R23_PRIVATE_H
#define TO50_OV80_R23_PRIVATE_H
#include "global.h"

#include "system.h"
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
extern int System_GetTouchNew(void);
BOOL ov80_0222D2EC(void *);
#endif
