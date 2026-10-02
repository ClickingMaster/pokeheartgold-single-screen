#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *background;u8 pad[0x58];u8 *tiles;} Work;
extern void CopyRectToBgTilemapRect(void *,int,u8,u8,int,int,const void *,u8,u8,int,int);extern void ScheduleBgTilemapBufferTransfer(void *,int);
void ov96_021F6E68(Work *,int);
void ov96_021F6E68(Work *w,int number){u8 xs[3],ys[3];u8 digit,i;digit=number/100;xs[0]=(digit%5)*4;ys[0]=(digit/5)*8;digit=(number-digit*100)/10;xs[1]=(digit%5)*4;ys[1]=(digit/5)*8;digit=number%10;xs[2]=(digit%5)*4;ys[2]=(digit/5)*8;for(i=0;i<3;i++)CopyRectToBgTilemapRect(w->background,4,(u8)(1+i*4),0,4,8,w->tiles+12,xs[i],ys[i],20,16);ScheduleBgTilemapBufferTransfer(w->background,4);}
