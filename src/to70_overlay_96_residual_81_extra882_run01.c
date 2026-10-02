#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x40];int active,x,y,type,size;} W;
void ov96_02200E80(W*,s32*);
void ov96_02200E80(W*w,s32*data) {int i;for(i=0;i<30;i++,data++,w=(W*)((u8*)w+20)){int type,y,x;type=(u8)*data;w->type=type;y=(u8)(*data>>8);x=(u16)(*data>>16);if(type!=0){w->active=1;if(w->type==2)w->size=32;else if(w->type==1)w->size=48;else w->size=64;w->x=x-w->size/2;w->y=y+32-w->size/2;}else w->active=0;}}
