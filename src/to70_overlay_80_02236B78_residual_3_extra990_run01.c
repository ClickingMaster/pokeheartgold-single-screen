#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[4];u8 mode,p2,index;u8 p3[17];u16 moves[4];} Work;
extern int ov80_022372B4(Work*);
int ov80_02237264(Work*);
int ov80_02237264(Work*w){int result,value;if(w->mode==0 && (u16)(w->moves[w->index]+0xfecb)<=1)return 7;value=ov80_022372B4(w);result=7;switch(value){case -1:break;case 0:case 1:result=0;break;case 2:case 3:result=1;break;}return result;}
