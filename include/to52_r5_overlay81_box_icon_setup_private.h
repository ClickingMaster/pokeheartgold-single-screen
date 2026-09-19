#ifndef TO52_R5_CARD17_PRIVATE_H
#define TO52_R5_CARD17_PRIVATE_H
#include "global.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
void ov81_02242DFC(void *, int);
void ov81_02242D88(void *, int);
void *ov81_02242CBC(void *, int, int, int, int, int, int);
void *ov81_02242D18(void *, int, int, int, int, int, int);
void ov81_02241FEC(u8 *work);
void ov81_02242058(u8 *work, int flag);
void ov81_022420B4(u8 *work);
#endif
