#pragma force_active on
#include "global.h"
typedef struct {u16 value;u8 p[14];void *arg;u8 items[3][76];u8 gap[8];u8 data[16];} W;
extern void ov43_0222DB28(W*,void*,int);extern void ov43_0222DF4C(void*,void*,void*,int,void*,void*,int);
static inline void setup(W*w,void*b,int value){w->value=value;ov43_0222DB28(w,b,value);}
void ov43_0222DAE8(W*,void*,void*,int,int,int);
void ov43_0222DAE8(W*w,void*a,void*b,int index,int value,int c) {if(index==0){setup(w,b,value);}ov43_0222DF4C(w->items[index],a,b,value,w->data,w->arg,c);}
