#pragma force_active on
#include "global.h"
extern void ov01_021F0454(void*,int,int,int,int,int,void*,int);
extern u16 *GetBgTilemapBuffer(void*,u8);
extern void ScheduleBgTilemapBufferTransfer(void*,u8);

void ov01_021F0500(void*a,int b,int c,int d,int e,int f,void*g,int h,int j);
// @fn ov01_021F0500
void ov01_021F0500(void*a,int b,int c,int d,int e,int f,void*g,int h,int j){u16 *map;int row,i,right,left;u16 tmp;ov01_021F0454(a,b,c,d,e,f,g,h);ov01_021F0454(a,b,c,d,e,f,g,j);map=GetBgTilemapBuffer(g,j); for(row=0,left=0,right=15;row<24;left+=16,right+=16,row++){for(i=0;i<8;i++){u16 p=i+left;u16 q=right-i;tmp=map[p];map[p]=map[q];map[q]=tmp;map[p]=(map[p]&0xfbff)|(((map[p]>>10)^1)<<10);map[q]=(map[q]&0xfbff)|(((map[q]>>10)^1)<<10);}}ScheduleBgTilemapBufferTransfer(g,j);}
