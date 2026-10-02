#pragma force_active on
#include "global.h"
#pragma force_active on
int ov99_021E7198(void *,int);void BgCommitTilemapBufferToVram(void *,int);
void LoadRectToBgTilemapRect(void *,int,u16 *,u8,u8,int,int);
void ov99_021E738C(void **,s16,s16);void ov99_021E73E0(void **,int,int);
void ov99_021E738C(void **w,s16 x,s16 y){int yy=y;int row,col,base=0;u16 tile;for(row=0;row<2;row++){for(col=0;col<2;col++){tile=col+base+1;LoadRectToBgTilemapRect(w[0],3,&tile,x+col,yy,1,1);}base+=2;yy++;}}
void ov99_021E73E0(void **w,int index,int arg){if(ov99_021E7198(w,arg)){int col=index%6,row=index/6;ov99_021E738C(w,(s16)(col*4+6),(s16)(row*3+4));BgCommitTilemapBufferToVram(w[0],3);}}
