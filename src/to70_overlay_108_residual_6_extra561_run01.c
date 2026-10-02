#pragma force_active on
#include "global.h"
#pragma force_active on
#include "touchscreen_list_menu.h"
typedef struct {u8 p[12];TouchscreenListMenuTemplate template;} Settings;
extern const Settings _021EA720;
extern const u16 ov108_021EA748[],ov108_021EA74A[];
typedef struct {u8 p[0x10];u32 touch;u8 p2[0x32c];BgConfig*bg;TouchscreenListMenuSpawner*spawner;u8 p3[0x16c];ListMenuItem*items;u32 pad;TouchscreenListMenu*menu;u8 p4[0x1801f];u8 selected;} Work;
extern void ov108_021E79A8(Work*,int,int,int);
void ov108_021E6A58(Work*);
void ov108_021E6A58(Work*w){TouchscreenListMenuHeader header;u16 x,y;MI_CpuFill8(&header,0,sizeof(header));header.template=_021EA720.template;header.listMenuItems=w->items;header.bgConfig=w->bg;header.numWindows=3;y=ov108_021EA74A[w->selected*2];x=ov108_021EA748[w->selected*2];w->menu=TouchscreenListMenu_Create(w->spawner,&header,w->touch,x,y,0,0);ov108_021E79A8(w,1,w->selected,1);ov108_021E79A8(w,4,w->selected,1);}
