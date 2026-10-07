#pragma force_active on
#include "global.h"
#include "to52_r6_overlay88_tilemap_helpers_r4_private.h"
extern void FillBgTilemapRect(void *,int,int,int,int,int,int,int);
extern int ov45_0222DCFC(void *,int),ov45_0222DD08(void *,int),ov45_0222DD14(void *,int),ov45_0222DD20(void *,int),ov45_0222DD2C(void *,int),ov45_0222AB28(void *,int);
void ov88_0225918C(void *,void **,void *,void *,int);
#pragma force_active on
void ov88_0225918C(void*w,void**bg,void*data,void*other,int mode){struct{volatile int c,b,a,row,d;int r;}f;int col,base,nextRow;FillBgTilemapRect(*bg,3,0,21,4,10,8,0);base=0;f.row=0;do{f.r=(u8)f.row;col=0;do{if(ov45_0222DCFC(data,col+base)){int e,use;f.a=ov45_0222DD08(data,col+base);f.b=ov45_0222DD14(data,col+base);f.c=ov45_0222DD20(data,col+base);f.d=ov45_0222DD2C(data,col+base);e=ov45_0222AB28(other,col+base);if(mode==2&&f.d==1)use=0;else use=mode;ov88_02258DE8(w,bg,(u8)col,f.r,use,f.a,f.b,f.c,e);}col++;}while(col<5);nextRow=f.row+1;base+=5;f.row=nextRow;}while(nextRow<4);}
