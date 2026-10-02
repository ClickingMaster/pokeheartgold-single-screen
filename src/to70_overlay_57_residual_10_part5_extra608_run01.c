#pragma force_active on
#include "global.h"
#pragma force_active on
#include "touchscreen.h"
#include "bg_window.h"
#include "system.h"
typedef struct {u8 p[0xe4];BgConfig*bg;} Work;
extern TouchscreenHitbox ov57_0223BEB0[];
int ov57_0223B578(Work*);
int ov57_0223B578(Work*w){struct{u16 pixel;TouchscreenHitbox hitbox;} local;int row,col,y,x,found;if(!System_GetTouchNew())return -1;local.hitbox.rect.top=254;local.hitbox.rect.right=24;y=24;for(row=0;row<3;row++){local.hitbox.rect.left=y;for(col=0,x=40;col<4;col++,x+=56){local.hitbox.rect.bottom=x;if(TouchscreenHitbox_TouchNewIsIn(&local.hitbox))return row*4+col;}y+=56;}found=TouchscreenHitbox_FindRectAtTouchNew(ov57_0223BEB0);if(found==-1)return -1;{local.pixel=1023;if(!DoesPixelAtScreenXYMatchPtrVal(w->bg,3,gSystem.touchX,gSystem.touchY,&local.pixel))found=-1;else found+=12;}return found;}
