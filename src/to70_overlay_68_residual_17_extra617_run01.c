#pragma force_active on
#include "global.h"
extern void BgTilemapRectChangePalette(void *,int,int,int,int,int,int);
extern void ScheduleBgTilemapBufferTransfer(void *,int);
typedef struct {u8 x,y,width,height;volatile u16 ticks,state;int result;} Effect;
typedef struct {u32 p;void *bg;u8 pad[0x1c8];Effect effect;} Work;
int ov68_021E7AD8(Work *);
int ov68_021E7AD8(Work *w){Effect *e=&w->effect;switch(e->state){case 0:BgTilemapRectChangePalette(w->bg,7,e->x,e->y,e->width,e->height,7);ScheduleBgTilemapBufferTransfer(w->bg,7);e->state++;break;case 1:e->ticks++;if(e->ticks==4){BgTilemapRectChangePalette(w->bg,7,e->x,e->y,e->width,e->height,0);ScheduleBgTilemapBufferTransfer(w->bg,7);e->ticks=0;e->state++;}break;case 2:e->ticks++;if(e->ticks==2)return e->result;break;}return 13;}
