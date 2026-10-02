#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[64];void *bg;u8 q[0x368-68];u8 thing;} State;
extern void BgSetPosTextAndCommit(void *,int,int,int),ov41_0224888C(void *,int),ov41_022488D8(void *,int,int,int,int);
void ov41_02247414(State *);
void ov41_02247414(State *s){volatile u16 *reg=(volatile u16*)0x04000008;reg[0]=(reg[0]&~3)|1;reg[1]=reg[1]&~3;reg[2]=(reg[2]&~3)|2;reg[3]=(reg[3]&~3)|3;BgSetPosTextAndCommit(s->bg,3,3,-16);ov41_0224888C(&s->thing,0);ov41_022488D8(&s->thing,0,2,0,0);}
