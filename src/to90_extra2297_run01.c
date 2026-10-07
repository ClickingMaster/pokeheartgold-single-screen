#pragma force_active on
#include "global.h"
#pragma force_active on
#include "touchscreen_list_menu.h"
typedef struct {u8 pad[0x40];u32 touch;u8 mode;u8 pad45[15];BgConfig *bg;TouchscreenListMenuSpawner *spawner;u8 pad5c[0x238];ListMenuItem *items;TouchscreenListMenu *menu;} Work;
extern const TouchscreenListMenuTemplate ov59_0223C630[2];
extern const u16 ov59_0223C668[];extern const u8 ov59_0223C66A[],ov59_0223C66B[];
void ov59_02238E98(Work*);
void ov59_02238E98(Work *w){TouchscreenListMenuHeader h;MI_CpuFill8(&h,0,sizeof(h));h.template=ov59_0223C630[1];h.listMenuItems=w->items;h.bgConfig=w->bg;h.numWindows=ov59_0223C668[(w->mode-1)*4];u8 y=ov59_0223C66B[(w->mode-1)*8];u8 x=ov59_0223C66A[(w->mode-1)*8];w->menu=TouchscreenListMenu_Create(w->spawner,&h,(u8)w->touch,x,y,8,0);}
