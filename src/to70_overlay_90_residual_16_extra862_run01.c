#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x1c];s32 motion[5];u16 timer,argument;int effect;} State;
extern void ov90_02258CF0(void *,int,int,int,int),ov90_02259FC8(State *,int);
void ov90_02259EE0(State *,int,int);
void ov90_02259EE0(State *s,int value,int argument){float scaled;if(value>0)scaled=0.5f+(float)(value<<12);else scaled=(float)(value<<12)-0.5f;ov90_02258CF0(s->motion,-1044480,(int)scaled,114688,8);s->timer=0;s->argument=argument;s->effect=1406;ov90_02259FC8(s,-(s->motion[0]>>12));}
