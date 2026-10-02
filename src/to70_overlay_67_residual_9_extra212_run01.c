#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct State {u8 reserved[0x494];u8 low:1;u8 value:7;u8 a:4;u8 b:4;u8 c,d;} State;
void ov67_021E6B6C(State *,int);
void ov67_021E6B6C(State *w,int value){w->low=0;w->value=(u8)value;w->a=1;w->b=0;w->c=0;w->d=0;}
