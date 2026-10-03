#pragma force_active on
#include "global.h"
typedef struct {u8 isBg:1,index:7;} Target;
typedef struct {u8 low:4,high:4;} Pal;
typedef struct {u8 pad[12];void *bg;u8 pad2[0x224];void *sprites[6];Target target;Pal palette;u8 state;volatile u8 timer;u8 x,y,w,h;} Work;
extern void ManagedSprite_SetPaletteOverride(void *,int),BgTilemapRectChangePalette(void *,int,int,int,int,int,int),ScheduleBgTilemapBufferTransfer(void *,int);
int ov86_021E7094(Work *);
int ov86_021E7094(Work *w) {
 switch(w->state) {
 case 0:
  if(!w->target.isBg)ManagedSprite_SetPaletteOverride(w->sprites[w->target.index],w->palette.low);
  else {BgTilemapRectChangePalette(w->bg,w->target.index,w->x,w->y,w->w,w->h,w->palette.low);ScheduleBgTilemapBufferTransfer(w->bg,w->target.index);}
  w->state++;break;
 case 1:
  w->timer++;if(w->timer!=4)break;
  if(!w->target.isBg)ManagedSprite_SetPaletteOverride(w->sprites[w->target.index],w->palette.high);
  else {BgTilemapRectChangePalette(w->bg,w->target.index,w->x,w->y,w->w,w->h,w->palette.high);ScheduleBgTilemapBufferTransfer(w->bg,w->target.index);}
  w->timer=0;w->state++;break;
 case 2: w->timer++;if(w->timer==2)return 0;break;
 }
 return 1;
}
