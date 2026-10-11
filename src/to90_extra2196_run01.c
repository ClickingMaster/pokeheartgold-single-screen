#pragma force_active on
#include "global.h"
#pragma force_active on
extern u8 ov18_021FA310[];
void ManagedSprite_GetPositionXY(void*,s16*,s16*);void ManagedSprite_SetPositionXY(void*,s16,s16);
u32 PokedexApp_GetScrollbarMinY(void*,int,void*,int,int);u32 PokedexApp_GetScrollbarMaxY(void*,int,void*,int,int);
BOOL PokedexApp_DragFormScrollbar(u8*,int,u32);
BOOL PokedexApp_DragFormScrollbar(u8*c,int unused,u32 y){s16 x,oldY;u32 min,max,segments,width,position,index,lower,upper;
ManagedSprite_GetPositionXY(*(void**)(c+0x684),&x,&oldY);min=PokedexApp_GetScrollbarMinY(c,5,ov18_021FA310,64,86);if(y<min)y=min;max=PokedexApp_GetScrollbarMaxY(c,5,ov18_021FA310,64,86);if(y>max)y=max;ManagedSprite_SetPositionXY(*(void**)(c+0x684),x,(s16)y);min=PokedexApp_GetScrollbarMinY(c,5,ov18_021FA310,64,86);max=PokedexApp_GetScrollbarMaxY(c,5,ov18_021FA310,64,86);max-=min;segments=(*(s8*)(c+0x18c4))-1;width=(max<<8)/segments;position=(y-min)<<8;index=0;lower=index;upper=index;for(;index<=segments;index++){if(position>=lower&&position<upper+width){if(*(s8*)(c+0x18c5)!=index){*(u8*)(c+0x18c5)=index;return 1;}break;}lower+=width;upper+=width;}return 0;}
