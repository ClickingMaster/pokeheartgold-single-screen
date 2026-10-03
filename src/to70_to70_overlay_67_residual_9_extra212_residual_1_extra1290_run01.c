#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x484];void *sprite[4];u8 mode:1,index:7;u8 low:4,high:4;u8 state,count,x,y,width,height;} Part;
typedef struct{u8 p[16];void *bg;u8 rest[0x470];void *sprite[4];u8 mode:1,index:7;u8 low:4,high:4;u8 state,count,x,y,width,height;} Work;
extern void ManagedSprite_SetPaletteOverride(void *,int),BgTilemapRectChangePalette(void *,int,int,int,int,int,int),ScheduleBgTilemapBufferTransfer(void *,int);
int ov67_021E6A40(Work *);
int ov67_021E6A40(Work *w){switch(w->state){case 0:if(w->mode==0)ManagedSprite_SetPaletteOverride(w->sprite[w->index],w->low);else{BgTilemapRectChangePalette(w->bg,w->index,w->x,w->y,w->width,w->height,w->low);ScheduleBgTilemapBufferTransfer(w->bg,w->index);}w->state++;break;case 1:w->count++;if(w->count==4){if(w->mode==0)ManagedSprite_SetPaletteOverride(w->sprite[w->index],w->high);else{BgTilemapRectChangePalette(w->bg,w->index,w->x,w->y,w->width,w->height,w->high);ScheduleBgTilemapBufferTransfer(w->bg,w->index);}w->count=0;w->state++;}break;case 2:w->count++;if(w->count==2)return 0;break;}return 1;}
