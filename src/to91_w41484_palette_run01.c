#pragma force_active on
#include "global.h"
#include "heap.h"
#include "pokepic.h"
#include "sprite.h"
#include "palette.h"
#pragma force_active on
typedef struct R2Motion{s16 x,y;s32 duration,value8,valueC,currentX,currentY,deltaX,deltaY,deltaTail;}R2Motion;
typedef struct Motion24{s16 x,y;u8 raw[0x20];}Motion24;
typedef struct W35Fade{void*owner;int state;Pokepic*pic;ManagedSprite*sprite;int special;Motion24 motion;s16 x,y;int delta;}W35Fade;
typedef char AssertSize[(sizeof(W35Fade)==64)?1:-1];
extern int ov07_0221BFD0(void*),ov07_0221C470(void*),ov07_02222004(void*,int),ov07_0221C468(void*),ov07_0221FAC8(u8*,int);
extern int ov07_02231924(void*,int);extern u8 ov07_02221FF0(void**);
extern Pokepic*ov07_0221FA48(void*,int);extern ManagedSprite*ov07_0221C4E8(void*,int);extern PaletteData*ov07_0221FA78(void*);
extern void ov07_022227A8(R2Motion*,int,int,int,int);
extern void ov07_0221C410(void*,void*,void*),ov07_0222CCBC(int,W35Fade*);
void ov07_0222CEC0(void*);
void ov07_0222CEC0(void *ctx){W35Fade *p;u32 mask,shift,first,second;p=Heap_Alloc((enum HeapID)ov07_0221BFD0(ctx),sizeof(W35Fade));memset(p,0,64);p->owner=ctx;p->pic=ov07_0221FA48(p->owner,ov07_0221C470(ctx));p->x=Pokepic_GetAttr(p->pic,0);p->y=Pokepic_GetAttr(p->pic,1);p->delta=Pokepic_GetAttr(p->pic,41);ov07_022227A8((R2Motion*)&p->motion,4,0,1,4);p->motion.x=(s16)((u32)(s32)p->motion.x+(u32)ov07_02222004(p->owner,ov07_0221C470(p->owner)));Pokepic_StartPaletteFade(p->pic,0,16,0,0);p->sprite=ov07_0221C4E8(p->owner,1);first=ov07_02231924(p->owner,ov07_0221C468(p->owner));second=ov07_02231924(p->owner,ov07_0221C470(p->owner));if((first==2&&second==4)||(first==5&&second==3)){shift=ov07_02221FF0((void**)p->sprite);mask=1;mask<<=shift;PaletteData_BeginPaletteFade(ov07_0221FA78(p->owner),4,(u16)mask,0,0,16,0);p->special=1;if(!ov07_0221FAC8((u8*)p->owner,ov07_0221C470(p->owner)))Pokepic_SetAttr(p->pic,6,1);}ov07_0221C410(p->owner,(void*)ov07_0222CCBC,p);}
