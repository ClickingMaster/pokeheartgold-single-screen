#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int pad[8];int flag,x,y;}S;
void ov96_02208AF8(S*s,const u8*p);
void ov96_02208AF8(S*s,const u8*p){int i;for(i=0;i<20;i++){u16 x=p[0]<<3,y=p[1]<<3;if(x&&y){s->flag=1;s->x=x;s->y=y;}else s->flag=0;p+=2;s=(S*)((u8*)s+12);}}
