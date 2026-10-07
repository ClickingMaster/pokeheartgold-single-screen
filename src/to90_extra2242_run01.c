#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct T{s16 x,y,z;u16 anim;int unused,priority,vram;int resources[6];int transfer,tail;}T;typedef struct W{u8 pad[0x404];void*system;void*sprites[50];}W;extern const int ov99_021E9FB0[5];extern void*ov98_0221E6E0(void*,T*);extern void ManagedSprite_SetAnimateFlag(void*,int),ManagedSprite_SetDrawFlag(void*,int);void ov99_021E810C(W*),ov99_021E8198(W*);
void ov99_021E810C(W*w){T t={0};const int*coords=ov99_021E9FB0;int y=48,i;for(i=0;i<5;i++,coords++,y+=32){t.anim=i;t.resources[0]=32;t.resources[1]=3;t.resources[2]=3;t.resources[3]=3;t.vram=2;t.priority=i;t.x=104+*coords*8;t.y=y;W*slot=(W*)((u8*)w+(i+15)*4);slot->sprites[0]=ov98_0221E6E0(w->system,&t);ManagedSprite_SetAnimateFlag(slot->sprites[0],1);ManagedSprite_SetDrawFlag(slot->sprites[0],0);}}
