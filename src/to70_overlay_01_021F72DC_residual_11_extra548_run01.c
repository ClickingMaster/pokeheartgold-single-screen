#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{s8 previous;volatile s8 ticks;s8 mode;u8 bit:1;u8 phase:7;}State;
extern int ov01_022089F8[];
extern void BillboardObject_SetSequence(void*,int),BillboardObject_SetSequenceFrame(void*,int),BillboardObject_AdvanceFrame(void*,int);
extern int BillboardObject_GetSequenceFrame(void*);

void ov01_021F85F4(void *unused,void *sprite,State*s,int mode);

void ov01_021F85F4(void *unused,void *sprite,State*s,int mode){int value;if(mode!=s->previous||s->mode!=2){BillboardObject_SetSequence(sprite,ov01_022089F8[mode]);BillboardObject_SetSequenceFrame(sprite,15<<12);s->phase=0;}value=BillboardObject_GetSequenceFrame(sprite);switch(s->phase){case 0:value-=1<<12;if(value<=10<<12)s->phase=(u8)(s->phase+1);break;case 1:value+=1<<12;if(value>=15<<12){s->phase=(u8)(s->phase+1);s->ticks=0;}break;case 2:s->ticks++;if(s->ticks>=30){s->ticks=0;s->phase=0;}break;}BillboardObject_SetSequenceFrame(sprite,value);BillboardObject_AdvanceFrame(sprite,0);}
