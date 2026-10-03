#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x2bc8];void *menu,*items;u8 p2[16];u8 window[16];}Work;
extern void ListMenuItems_Delete(void*),DestroyListMenu(void*,void*,void*),ov74_0222D0EC(void*),ClearWindowTilemapAndCopyToVram(void*),RemoveWindow(void*);

void ov74_0222D448(Work*w);

void ov74_0222D448(Work*w){ListMenuItems_Delete(w->items);w->items=0;DestroyListMenu(w->menu,0,0);w->menu=0;ov74_0222D0EC(w->window);ClearWindowTilemapAndCopyToVram(w->window);RemoveWindow(w->window);}
