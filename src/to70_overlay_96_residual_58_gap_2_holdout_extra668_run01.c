#pragma force_active on
#include "global.h"
typedef struct{u8 pad[8];u32 size;u8 data[1];}Screen;
typedef struct{int active;u8 pad[16];void(*callback)(void);int unused,state;u8 pad2[0x78];Screen *screen7;u32 pad3;void*tiles6;u32 pad4;Screen*screen6;u8 pad5[0x18];void*sprite;}Work;
extern void *ov96_021EB5B8(void*);extern void Sprite_SetMatrix(void*,const void*),BG_LoadScreenTilemapData(void*,int,const void*,u32),BgTilemapRectChangePalette(void*,int,int,int,int,int,int),ScheduleBgTilemapBufferTransfer(void*,int),OS_WaitVBlankIntr(void),SetBgAffine(void*,int,const void*,int,int),GfGfx_EngineBTogglePlanes(int,int),ov96_021F4BB4(void);
extern float _fmul(float,float);
void ov96_021F4990(Work*,void*);
void ov96_021F4990(Work*w,void*bg){s32 v[3]={0,0,0};s32 m[4];float scale;v[0]=14<<14;v[1]=41<<16;Sprite_SetMatrix(ov96_021EB5B8(w->sprite),v);BG_LoadScreenTilemapData(bg,6,(u8*)w->tiles6+12,w->screen6->size);BgTilemapRectChangePalette(bg,6,0,0,32,24,3);ScheduleBgTilemapBufferTransfer(bg,6);BG_LoadScreenTilemapData(bg,7,w->screen7->data,w->screen7->size);ScheduleBgTilemapBufferTransfer(bg,7);scale=0.025f;m[0]=(int)_fmul(4096.0f,scale);m[1]=0;m[2]=0;m[3]=m[0];OS_WaitVBlankIntr();SetBgAffine(bg,7,m,128,96);GfGfx_EngineBTogglePlanes(8,1);w->callback=ov96_021F4BB4;w->state=4;w->active=1;}
