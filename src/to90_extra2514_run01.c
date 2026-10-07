#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 bytes[16];void*hitbox;u8 tail[8];} Item;
typedef struct{Item items[16];u8 gap[0x140];void*controller;u8 hitboxes[28][4];} Context;
typedef struct{s16 x,y,x2,y2;} Rectangle;
extern const Rectangle _02102768[12];
void sub_02086180(u32,u32,void*);void*TouchHitboxController_Create(void*,u32,void(*)(u32,u32,void*),void*,int);void sub_020860B8(Context*);
static inline void CopyHalf(u16*dst,const u16*src){const u16*source=src;u16*target=dst;u32 count=48;do{*target=*source;source++;target++;}while(--count);}
void sub_020860B8(Context*c){int i;Rectangle rectangles[12];for(i=0;i<16;i++)c->items[i].hitbox=c->hitboxes[i];CopyHalf((u16*)rectangles,(const u16*)_02102768);for(;i<28;i++){c->hitboxes[i][0]=rectangles[i-16].y-rectangles[i-16].y2;c->hitboxes[i][2]=rectangles[i-16].x-rectangles[i-16].x2;c->hitboxes[i][1]=rectangles[i-16].y+rectangles[i-16].y2;c->hitboxes[i][3]=rectangles[i-16].x+rectangles[i-16].x2;}c->controller=TouchHitboxController_Create(c->hitboxes,28,sub_02086180,c,108);}
