#pragma force_active on
#include "global.h"
typedef struct {u8 a,b,c,d;} Rect;
typedef struct {void*a,*sprite;} Work;
extern Rect *ov96_0221DC68[];extern u8 ov96_0221CDA0[];extern void ManagedSprite_GetPositionXY(void*,s16*,s16*);extern int TouchscreenHitbox_FindRectAtTouchNew(Rect*);
int ov96_0220B0A4(Work*,int);
int ov96_0220B0A4(Work*w,int index){Rect rects[3]={{0,0,0,0},{0,0,0,0},{0,0,0,0}};struct {s16 x,y;} pos;u32 count;Rect*p;int i;if(index>=3)GF_AssertFail();p=ov96_0221DC68[index];count=(u8)(ov96_0221CDA0[index]-1);ManagedSprite_GetPositionXY(w->sprite,&pos.y,&pos.x);i=0;if((s32)count>0){do{rects[i].a=pos.x+p->a;rects[i].b=pos.x+p->b;rects[i].c=pos.y+p->c;rects[i].d=pos.y+p->d;i++;p++;}while(i<(s32)count);}rects[count].a=255;if(TouchscreenHitbox_FindRectAtTouchNew(rects)!=-1)return 1;return 0;}
