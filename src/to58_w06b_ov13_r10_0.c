#include "global.h"
extern int SOC_RecvFrom(int, void *, int, int, u8 *);
extern int SOC_SendTo(int, const void *, int, int, u8 *);
int ov13_02222984(int a, void *b, int c, int d, u8 *e, u32 *f);
int ov13_022229EC(int a, const void *b, int c, int d, u8 *e, u32 f);
int ov13_02222984(int a, void *b, int c, int d, u8 *e, u32 *f) { *e = *f; return SOC_RecvFrom(a,b,c,d,e); }
