#pragma force_active on
#include "global.h"
#include "system.h"
#include "touchscreen.h"
#include "sound.h"
#pragma force_active on
void PlaySE(int);
typedef struct{u8 pad[0x14];void *ctx;u8 pad18[0x6c-0x18];u8 drag,pad6d;s16 oldX;}Work;
void *ov102_021EA228(void*);void Sprite_GetPositionXY(void*,s16*,s16*);void Sprite_SetPositionXY(void*,s16,s16);int ov102_021E7F30(Work*,int,int),ov102_021E7EEC(Work*,int,int);void ov102_021E79DC(void*,void*,u8);void ov102_021E940C(void*,int);int ov102_021EA238(void*);void ov102_021EA248(void*,TouchscreenHitbox*,u8);extern const TouchscreenHitbox ov102_021EC634[];int ov102_021E7F6C(Work*);
int ov102_021E7F6C(Work*w){struct{s16 y,x;TouchscreenHitbox hit;}f;void *sprite=ov102_021EA228(w->ctx);Sprite_GetPositionXY(sprite,&f.x,&f.y);if(w->drag){int d;s16 x;if(!ov102_021E7F30(w,f.x,f.y)){w->drag=0;return -1;}x=(s16)gSystem.touchX;if(x<28)x=28;else if(x>226)x=226;d=x-w->oldX;if(d<0)d=0-d;if(d<2)return -1;Sprite_SetPositionXY(sprite,x,f.y);w->oldX=x;*(u16*)((u8*)w+0x4e)=0;ov102_021E79DC((u8*)w+0x64,(u8*)w+8,(x-28)/2);PlaySE(1500);ov102_021E940C(w->ctx,3);return -1;}else{int result,count,i;if(!System_GetTouchNew())return -1;if(ov102_021E7EEC(w,f.x,f.y)){w->oldX=gSystem.touchX;w->drag=1;PlaySE(1500);return 6;}result=TouchscreenHitbox_FindRectAtTouchNew(ov102_021EC634);if(result==-1){count=ov102_021EA238(w->ctx);for(i=0;i<count;i++){ov102_021EA248(w->ctx,&f.hit,i);if(TouchscreenHitbox_PointIsIn(&f.hit,gSystem.touchX,gSystem.touchY))return i+4;}return -1;}return result;}}
