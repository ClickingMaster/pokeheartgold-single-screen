#pragma force_active on
#include "global.h"
typedef struct{int a,b;} Entry;
typedef struct{u8 pad[0xc];void *s[3];u8 unused[4];void *window;u8 p2[0x14];Entry *entry;u8 p3[0xd];s8 selected;u8 p4[6];int mode,next;u8 p5[6];s16 index;} Work;
extern int ov70_022428C0(Work *);
extern void sub_020198FC(void *,int,int,int,int),Sprite_SetDrawFlag(void *,int);
int ov70_02243DDC(Work *);
int ov70_02243DDC(Work *w){int result=ov70_022428C0(w);switch(result){case -2:sub_020198FC(w->window,0,4,0,4);Sprite_SetDrawFlag(w->s[0],0);Sprite_SetDrawFlag(w->s[1],0);Sprite_SetDrawFlag(w->s[2],0);w->mode=3;w->next=33;w->selected=-2;break;case -1:break;default:sub_020198FC(w->window,0,4,0,4);Sprite_SetDrawFlag(w->s[0],0);Sprite_SetDrawFlag(w->s[1],0);Sprite_SetDrawFlag(w->s[2],0);w->mode=3;w->next=33;w->selected=w->entry[w->index*4+result].b;break;}return -1;}
