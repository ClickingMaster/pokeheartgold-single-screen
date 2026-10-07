#ifndef SOL40_OV49_R14_PRIVATE_H
#define SOL40_OV49_R14_PRIVATE_H
#include "global.h"
#include "overlay49_entry40.h"
typedef struct { u8 a,b,c,d; u16 e,f; } Ov49R14Out;
void ov45_0223089C(void *, int);
void ov45_02230908(void *, void *);
void ov45_022308E4(void *, void *);
void ov45_02230700(void *, int);
void ov49_022599F8(void *, void *);
void ov49_02259A20(Ov49R14Out *, void *, u8);
#endif
