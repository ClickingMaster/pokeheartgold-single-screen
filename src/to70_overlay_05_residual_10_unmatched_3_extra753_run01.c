#pragma force_active on
#include "global.h"
#include "bg_window.h"
#pragma force_active on
typedef struct Parent{u8 p[0x24];int heap;}Parent;typedef struct W{Parent*parent;u8 p4[8];BgConfig*bg;Window win;u8 p20[0x174];void*mgr;u8 p198[0x9ac];void*textman;void*objs[1];}W;typedef struct Params{void*man;Window*window;void*list,*palette;u32 priority,x,y,z,screen,which,flag,heap;}Params;
extern u8 AddTextPrinterParameterizedWithColor(Window*,int,void*,u32,u32,u32,u32,void*);extern void*SpriteManager_GetSpriteList(void*),*SpriteManager_FindPlttResourceProxy(void*,u32),*FontOAM_Create(Params*);extern void TextOBJ_SetPaletteNum(void*,int);void ov05_0221D530(W*,int,void*);
void ov05_0221D530(W*w,int i,void*text){Params p;Window*win=&w->win;AddTextWindowTopLeftCorner(w->bg,win,8,2,0,0);AddTextPrinterParameterizedWithColor(win,0,text,0,0,255,0x30400,0);p.man=w->textman;p.window=win;p.list=SpriteManager_GetSpriteList(w->mgr);p.palette=SpriteManager_FindPlttResourceProxy(w->mgr,0xb807);p.priority=0;p.x=(0x3e0-i*16)*32;p.y=0;p.z=0;p.screen=2;p.which=0;p.flag=1;p.heap=w->parent->heap;w->objs[i]=FontOAM_Create(&p);TextOBJ_SetPaletteNum(w->objs[i],0);RemoveWindow(win);}
