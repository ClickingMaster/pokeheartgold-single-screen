#pragma force_active on
#include "global.h"
#include "system.h"
#include "touchscreen.h"
extern int System_GetTouchNew(void);
extern int TouchscreenHitbox_PointIsIn(const TouchscreenHitbox*,u32,u32);
int ov102_021E83E4(void);
int ov102_021E83E4(void){TouchscreenHitbox box;int row,col,x,y;if(!System_GetTouchNew())return -1;row=0;y=56;do{box.rect.top=y;box.rect.bottom=box.rect.top+16;col=0;x=12;do{box.rect.left=x;box.rect.right=box.rect.left+72;if(TouchscreenHitbox_PointIsIn(&box,gSystem.touchX,gSystem.touchY))return row*3+col;col++;x+=80;}while(col<3);y+=24;row++;}while(row<4);return -1;}
