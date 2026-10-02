#pragma force_active on
#include "global.h"
#pragma force_active on
struct Inner{u8 pad[32];int state;};struct S{struct Inner*inner;u8 pad[0xbd4-4];int value;};void ov72_0223A350(struct S*,int,int,int,int);void ov72_02238680(struct S*,int,int);void ov72_0223A444(struct S*);int ov72_02239E58(struct S*);
int ov72_02239E58(struct S*s){ov72_0223A350(s,s->value,3,1,3855);ov72_02238680(s,47,45);ov72_0223A444(s);s->inner->state=0;return 3;}
