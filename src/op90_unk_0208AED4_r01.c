#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x7c8];u8 a,b,c,d;u8 low:4,high:4;u8 e,f,g;}Work;
int sub_0208AFE8(Work *w,u8 e,u8 a,u8 b,u8 c,u8 d,u8 low,u8 high,u8 g);
// @fn sub_0208AFE8
int sub_0208AFE8(Work *w,u8 e,u8 a,u8 b,u8 c,u8 d,u8 low,u8 high,u8 g){w->a=a;w->b=b;w->c=c;w->d=d;w->low=low;w->high=high;w->e=e;w->f=0;w->g=g; return 15;}
