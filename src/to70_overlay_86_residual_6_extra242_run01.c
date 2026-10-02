#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct State {u8 pad[0x24c];u8 low:1,value:7;u8 a:4,b:4;u8 c,d;} State;
void ov86_021E71C0(State *);
void ov86_021E71C0(State *w){w->low=0;w->value=0;w->a=1;w->b=0;w->c=0;w->d=0;}
