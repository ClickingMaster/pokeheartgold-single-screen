#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct T{s16 x,y,z;u16 anim;int unused,priority,vram;int resources[6];int transfer,tail;}T;typedef struct W{u8 pad[0x404];void*system;void*sprites[50];}W;extern const int ov99_021E9FB0[5];extern void*PokeathlonUtil_NewSprite(void*,T*);extern void ManagedSprite_SetAnimateFlag(void*,int),ManagedSprite_SetDrawFlag(void*,int);void ov99_021E810C(W*),ov99_021E8198(W*);
#pragma force_active on
void ov99_021E8198(W*w){int yy;T t={0};u32 row=0,base=0;int y=32;for(;row<5;row++){u32 col=0;int x=48;yy=y;for(;col<6;col++,x+=32){u32 index=col+base;t.resources[0]=index+2;t.resources[1]=1;t.resources[2]=1;t.resources[3]=1;t.vram=1;t.x=x;t.y=yy;t.transfer=1;void*sprite=PokeathlonUtil_NewSprite(w->system,&t);W*slot=(W*)((u8*)w+(index+20)*4);slot->sprites[0]=sprite;}base+=6;y+=24;}}
