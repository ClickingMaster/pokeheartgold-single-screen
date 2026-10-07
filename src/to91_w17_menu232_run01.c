#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "list_menu_items.h"
typedef struct Ov83SpriteWrap {s16 x,y;void *sprite;} Ov83SpriteWrap;
typedef struct Ov83SpriteHolder {void *unk0,*sprite;} Ov83SpriteHolder;
typedef struct W{u8 pad[14];u8 flags0:3,active:1,flags4:4;u8 gap[65];Window windows[64];u8 tail[140];ListMenuItem*items;u8 tail2[668];Ov83SpriteWrap*first;Ov83SpriteHolder*move;Ov83SpriteWrap*sprites[6];Ov83SpriteWrap*last;u8 tail3[188];void*object;u8 gap4[8];u32 info;}W;
extern void ov83_022477EC(int,int,u32*),ov83_02247A18(void*),ov83_0224755C(Ov83SpriteWrap*,int),ov83_0224759C(Ov83SpriteHolder*,int,int);
void ov83_0224042C(void*);
#pragma force_active on
void ov83_0224042C(void *argument){W*w=argument;if(w->active==1){u32 i;for(i=0;i<6;i++)ClearWindowTilemapAndScheduleTransfer(&w->windows[48+i]);ClearWindowTilemapAndScheduleTransfer(&w->windows[54]);ClearWindowTilemapAndScheduleTransfer(&w->windows[55]);ClearWindowTilemapAndScheduleTransfer(&w->windows[58]);ClearWindowTilemapAndScheduleTransfer(&w->windows[59]);ClearWindowTilemapAndScheduleTransfer(&w->windows[60]);ov83_022477EC(2,0,&w->info);ov83_022477EC(6,0,&w->info);ov83_022477EC(7,0,&w->info);ov83_02247A18(w->object);ListMenuItems_Delete(w->items);ov83_0224755C(w->first,0);ov83_0224755C(w->last,0);{u32 j;for(j=0;j<6;j++)ov83_0224755C(w->sprites[j],0);}ov83_0224759C(w->move,48,40);w->active=0;}}
