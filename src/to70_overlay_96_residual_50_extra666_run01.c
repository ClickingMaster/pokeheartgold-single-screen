#pragma force_active on
#include "global.h"
typedef struct {u8 pad[16];void *a,*b;u8 pad2[8];u8 x,y;u8 pad3[22];void *sprites[4];} Work;
typedef struct {s16 x,y;u8 pad[48];} Template;
extern Template ov96_0221DA80[];extern s16 ov96_0221BA80[][2];extern int ov96_0221BAE8[][2];extern void *SpriteSystem_NewSpriteWithYOffset(void*,void*,void*,int);extern void ManagedSprite_SetAnimateFlag(void*,int);extern void ManagedSprite_SetAnim(void*,int);
void ov96_021EFA04(Work*);

void ov96_021EF99C(Work*w);

void ov96_021EF99C(Work*w){struct Coord {s16 x,y;};struct Coord*pos=(struct Coord*)ov96_0221BA80;int i=0;s16*tmpl=(s16*)ov96_0221DA80;Work*cursor=w;u8*api=(u8*)ov96_0221DA80;for(;i<4;i++){void*a=w->a,*b=w->b;tmpl[0]=pos->x;tmpl[1]=pos->y;cursor->sprites[0]=SpriteSystem_NewSpriteWithYOffset(a,b,api,2<<20);ManagedSprite_SetAnimateFlag(cursor->sprites[0],1);pos++;tmpl+=26;api+=52;cursor=(Work*)((u8*)cursor+4);}ManagedSprite_SetAnimateFlag(w->sprites[2],0);ov96_021EFA04(w);}
void ov96_021EFA04(Work*w){ManagedSprite_SetAnim(w->sprites[2],ov96_0221BAE8[w->x][w->y]);}
