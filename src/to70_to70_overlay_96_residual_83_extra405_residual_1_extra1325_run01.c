#pragma force_active on
#include "global.h"
typedef struct {u8 pad[8];void *bg;u8 pad2[20];u8 *data;} Work;
extern void CopyRectToBgTilemapRect(void*,u8,u8,u8,u8,u8,const void*,u8,u8,u8,u8),ScheduleBgTilemapBufferTransfer(void*,u8);
void ov96_02203BD0(Work*,int);
void ov96_02203BD0(Work *w,int value){u8 y[3],x[3];u16 digit;u8 i;
digit=value/100;y[0]=(digit%5)*4;x[0]=(digit/5)*8;digit=(u16)(value-100*digit)/10u;y[1]=(digit%5)*4;x[1]=(digit/5)*8;digit=value%10;y[2]=(digit%5)*4;x[2]=(digit/5)*8;
for(i=0;i<3;i++)CopyRectToBgTilemapRect(w->bg,5,19+i*4,16,4,8,w->data+12,y[i],x[i],20,20);ScheduleBgTilemapBufferTransfer(w->bg,5);}
