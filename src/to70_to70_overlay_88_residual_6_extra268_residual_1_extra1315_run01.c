#pragma force_active on
#include "global.h"
typedef struct{u8 h,m,s,p[2],reset;u16 ticks;u32 divisor;} Work;
extern u32 ov88_02259924[];
extern void FillBgTilemapRect(void *,int,int,int,int,int,int,int),ScheduleBgTilemapBufferTransfer(void *,int),ov88_02258D90(void *,void *,int,u8,u8);
void ov88_02258F88(Work *,void *,void **);
void ov88_02258F88(Work *w,void *a,void **b){int i;int slot=(w->m*60+w->s+w->h*3600)/w->divisor;if(w->reset){int j;w->ticks=0;w->reset=0;FillBgTilemapRect(b[0],3,0,5,4,10,8,0);ScheduleBgTilemapBufferTransfer(b[0],3);for(i=0;i<4;i++){int row=3-i;for(j=0;j<10;j++){int col=9-j;u32 index=col+row*10;if(index>=slot){int tile;if(index==slot)tile=1;else if(row==3)tile=2;else tile=0;ov88_02258D90(a,b,tile,col,row);}}}}else if((u32)slot<40){int ticks,row,col,phase;w->ticks++;ticks=w->ticks;phase=ticks%32;col=(u32)slot%10;row=(u32)slot/10;if(phase==0){u32 step=(30*w->divisor)/4;int tile=6;for(int k=0;k<4;k++){if(step*(k+1)>(u32)ticks){tile=ov88_02259924[k];break;}}ov88_02258D90(a,b,tile,col,row);}else if(phase==16)ov88_02258D90(a,b,3,col,row);}}
