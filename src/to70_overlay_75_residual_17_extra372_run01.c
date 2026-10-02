#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct State {void *base;u8 pad4[4];int next;u8 padc[0x14];void *format;u8 pad24[0x10];void *win;u8 pad38[12];void *busy;u8 pad48[0x4c];int step;} State;
int ov75_02249534(void *),ov75_02246CF8(void *),ov75_02246CE8(void *),sub_020317BC(void *,int);void *sub_02031774(void *);void ov75_02249884(void *,u32 *,int),BufferIntegerAsString(void *,u32,u32,int,int,int),ov75_022494CC(void *,void *,int,int,int);
int ov75_02249550(State *),ov75_022495B0(State *);
int ov75_02249550(State *s){if(ov75_02249534(s->busy)==1)return 0;switch(s->step){case 0:ov75_022494CC(s,s->win,ov75_02246CF8(s->base)==2?38:39,1,0xf0f);s->step++;break;case 1:s->next=5;break;}return 0;}
int ov75_022495B0(State *s){u8 *base=s->base;if(ov75_02249534(s->busy)==1)return 0;switch(s->step){case 0:{int id;int num=ov75_02246CE8(base);if(num==sub_020317BC(*(void **)(base+4),3)){u32 values[4];u32 i;ov75_02249884(sub_02031774(*(void **)(base+4)),values,0x73);for(i=0;i<4;i++)BufferIntegerAsString(s->format,i,values[i],4,2,1);id=41;}else{id=40;}ov75_022494CC(s,s->win,id,1,0xf0f);s->step++;break;}case 1:s->next=0;break;}return 0;}
