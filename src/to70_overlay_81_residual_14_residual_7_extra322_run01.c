#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad0[8];u8 state;u8 pad9[9];u8 count;u8 pad13[57];void*bg;u8 pad50[784];void*sprites[1];u8 pad364[288];u8 x,y,width,height,cycles,timer,palette,next;}W;
extern void BgTilemapRectChangePalette(void*,int,u8,u8,u8,u8,u8),ScheduleBgTilemapBufferTransfer(void*,int),ov81_02242FBC(void*,int);int ov81_022425EC(W*);void ov81_02242694(W*,int);
int ov81_022425EC(W*w){if(w->timer==0){if(w->palette==0)BgTilemapRectChangePalette(w->bg,5,w->x,w->y,w->width,w->height,10);else BgTilemapRectChangePalette(w->bg,5,w->x,w->y,w->width,w->height,9);ScheduleBgTilemapBufferTransfer(w->bg,5);w->palette^=1;}w->timer++;if(w->timer==2){w->cycles++;if(w->cycles==4){w->state=w->next;return 0;}w->timer=0;}return 1;}
void ov81_02242694(W*w,int active){u32 i;if(active==1){G2x_SetBlendAlpha_(0x04001050,0,15,6,10);for(i=0;i<w->count;i++){if(w->sprites[i])ov81_02242FBC(w->sprites[i],1);}}else{*(volatile u16*)0x04000050=0;for(i=0;i<w->count;i++){if(w->sprites[i])ov81_02242FBC(w->sprites[i],0);}}}
