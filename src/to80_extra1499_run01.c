#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int heap;void *objects[48];u8 started,finished,type,delay;s8 timer;u8 other,active;} State;
extern u8 ov01_02206980[];
extern void ov01_021F0868(void *,int,int,int,int,int,int,int,int,int);
extern int ov01_021F08C0(void *);
int ov01_021F0A4C(State *);
int ov01_021F0A4C(State *s){int i,result;if(!s->active)return 1;if(s->started<48){s->timer--;if(s->timer<=0){int slot,column,row;s->timer=s->delay;slot=s->started;column=ov01_02206980[slot%8]*32+16;row=((u32)slot>>3)*32;ov01_021F0868(s->objects[slot],column,column,-16-row,176-row,s->type,s->heap,32,32,s->other);s->started++;}}for(i=s->finished;i<s->started;i++){result=ov01_021F08C0(s->objects[i]);if(result==1)s->finished++;}if(s->finished>=48&&result==1){s->active=0;return 1;}return 0;}
