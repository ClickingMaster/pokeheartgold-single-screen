#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{int x[4];void*sprites[20];int count;};void Sprite_SetAnimCtrlSeq(void*,int);void ov41_0224AF8C(struct S*,int);

#pragma force_active on
#pragma opt_lifetimes off
void ov41_0224AF8C(struct S*s,int count){int i=s->count;if(i<count){for(;i<count;i++)Sprite_SetAnimCtrlSeq(s->sprites[i],0);}else if(i>count){for(int j=i-1;j>=count;j--)Sprite_SetAnimCtrlSeq(s->sprites[j],1);}s->count=count;}
