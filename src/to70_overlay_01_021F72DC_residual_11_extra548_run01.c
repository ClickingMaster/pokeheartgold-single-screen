#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{s8 previous;volatile s8 ticks;s8 mode;u8 bit:1;u8 phase:7;}State;
extern int ov01_022089F8[];
extern void sub_02023EE0(void*,int),sub_02023F40(void*,int),sub_02023F04(void*,int);
extern int sub_02023F70(void*);

void ov01_021F85F4(void *unused,void *sprite,State*s,int mode);

void ov01_021F85F4(void *unused,void *sprite,State*s,int mode){int value;if(mode!=s->previous||s->mode!=2){sub_02023EE0(sprite,ov01_022089F8[mode]);sub_02023F40(sprite,15<<12);s->phase=0;}value=sub_02023F70(sprite);switch(s->phase){case 0:value-=1<<12;if(value<=10<<12)s->phase=(u8)(s->phase+1);break;case 1:value+=1<<12;if(value>=15<<12){s->phase=(u8)(s->phase+1);s->ticks=0;}break;case 2:s->ticks++;if(s->ticks>=30){s->ticks=0;s->phase=0;}break;}sub_02023F40(sprite,value);sub_02023F04(sprite,0);}
