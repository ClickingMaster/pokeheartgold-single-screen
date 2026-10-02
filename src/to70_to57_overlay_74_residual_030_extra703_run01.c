#pragma force_active on
#include "global.h"
typedef struct{u32 x[4];u8 screen,a,b,c;u32 y[2];}Template;extern Template _0223B720;extern void InitBgFromTemplate(void*,u8,Template*,int),BgClearTilemapBufferAndCommit(void*,u8);
void ov74_02235308(void*w,int id,u32 screen,u32 x,u32 y);
void ov74_02235308(void*w,int id,u32 screen,u32 x,u32 y){Template t=_0223B720;t.screen=screen;switch(screen){case 1:t.x[2]=2048;break;case 2:t.x[2]=4096;break;case 3:t.x[2]=4096;break;case 4:t.x[2]=8192;break;}t.b=x>>11;t.c=y>>14;InitBgFromTemplate(w,(u8)id,&t,0);BgClearTilemapBufferAndCommit(w,(u8)id);}
