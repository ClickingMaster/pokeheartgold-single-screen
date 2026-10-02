#pragma force_active on
#include "global.h"
extern int abs(int);
#pragma force_active on
typedef struct Item{s16 x,y;u8 flag,pad;}Item;typedef struct W{u8 p[0x204];Item*items;s16 x,y;u32 found;}W;extern void DowsingMchn_GetHiddenItemLocs(W*),DowsingMchn_FreeHiddenItemLocs(W*);void ov28_0225DD58(W*);
void ov28_0225DD58(W*w){int offset;u16 best;DowsingMchn_GetHiddenItemLocs(w);w->found=0;for(offset=0;;offset+=6){Item*item=(Item*)((u8*)w->items+offset);if(item->flag==255)break;if(w->found==0){s16 x=item->x;best=abs(x)+abs(item->y);w->x=x;w->y=((Item*)((u8*)w->items+offset))->y;w->found=1;}else{ s16 x=item->x;u16 dist=abs(x)+abs(item->y);if(best>dist){best=dist;w->x=x;w->y=((Item*)((u8*)w->items+offset))->y;}}}DowsingMchn_FreeHiddenItemLocs(w);}
