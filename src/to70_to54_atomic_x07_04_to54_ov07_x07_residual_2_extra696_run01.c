#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int a;void*source;u8 pad[8];void*pic,*sprite;s16 x,y;u8 p1[12];int scale;u8 p2[0x68-0x2c];s16 attr;}Work;
extern Work *ov07_022324D8(void*,int);
extern void *ov07_0221C514(void*),*ov07_0221FA48(void*,int),*ov07_0221C4E8(void*,int);
extern int ov07_0221C468(void*),ov07_02222004(void*,int),Pokepic_GetAttr(void*,int);
extern void ManagedSprite_SetOamMode(void*,int),Pokepic_SetAttr(void*,int,int),ov07_02222A44(void*,int,int),ov07_02231E08(void*,int,int),ov07_02222AC4(void*,int,int,int,int,int),ov07_0221C410(void*,void(*)(void),void*),ov07_0222DA60(void),SpriteSystem_DrawSprites(void*);

void ov07_0222DB14(void*arg);

void ov07_0222DB14(void*arg){Work*w=ov07_022324D8(arg,108);int factor;w->a=(int)arg;w->source=ov07_0221C514(arg);factor=ov07_02222004((void*)w->a,ov07_0221C468((void*)w->a));w->pic=ov07_0221FA48((void*)w->a,ov07_0221C468(arg));w->x=Pokepic_GetAttr(w->pic,0);w->y=Pokepic_GetAttr(w->pic,1);w->attr=Pokepic_GetAttr(w->pic,41);w->y+=8;w->sprite=ov07_0221C4E8((void*)w->a,0);ManagedSprite_SetOamMode(w->sprite,1);Pokepic_SetAttr(w->pic,6,1);ov07_02222A44((u8*)w+28,2,16);ov07_02231E08((void*)w->a,16,0);ov07_02222AC4((u8*)w+64,16,0,0,16,32);w->scale*=factor;ov07_0221C410((void*)w->a,ov07_0222DA60,w);SpriteSystem_DrawSprites(w->source);}
