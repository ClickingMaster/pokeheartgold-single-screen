#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 gap[0x9b];u8 count;u8 gap2[8];void *pointer,*other;u8 zero,one,copy;u8 low:4;u8 mid:2;u8 top:2;} State;
void ov80_0222E754(State*);
void ov80_0222E754(State*w){w->pointer=(u8*)w+0xb4;w->other=(u8*)w+8;w->zero=0;w->one=1;w->copy=w->count;w->low=0;w->mid=0;if(w->count>=4)w->top=1;else w->top=0;}
