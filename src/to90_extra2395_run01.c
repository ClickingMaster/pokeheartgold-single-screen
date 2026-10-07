#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[384],current[6],goal[6],unused[6],passes[6];s8 countdown[6];u8 gap,total;}State;int ov93_02262130(State*);
#pragma opt_strength_reduction off
int ov93_02262130(State*s){int reverse=5;int i;int prior;if(s->total>6)return 1;prior=0;for(i=0;i<6;i++,reverse--){int saved=prior;State*p=(State*)((u8*)s+reverse);if(p->countdown[0]>0){(p->countdown[0])--;prior=0;}else if(p->passes[0]>1&&p->current[0]==p->goal[0]){prior=1;}else{prior=0;int old=*(volatile u8*)&p->current[0];p->current[0]+=23;int goal=p->goal[0],lifted=goal+160;if((old<=goal&&(u32)p->current[0]>=(u32)goal)||(old<=lifted&&(int)p->current[0]>=lifted)){if(p->passes[0]<1){p->passes[0]++;}else if(reverse==5||saved==1){p->passes[0]++;p->current[0]=p->goal[0];s->total++;if(s->total>=6)return 1;}}p->current[0]=(int)p->current[0]%160;}}return 0;}
