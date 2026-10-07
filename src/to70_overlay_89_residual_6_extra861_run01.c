#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct {u8 p[0x1c];SpriteSystem *sys;SpriteManager *manager;u8 q[0x96c];ManagedSprite *sprite[3];} W;extern ManagedSpriteTemplate ov89_0225CBA4;extern void ov89_0225A1D8(W*,int);extern void PlaySE(int);
void ov89_0225A16C(void*,int);
void ov89_0225A16C(void*opaque,int index) {W*w=opaque;ManagedSpriteTemplate t=ov89_0225CBA4;int i;W*p;i=0;p=w;for(;i<3;i++,p=(W*)((u8*)p+4)){if(p->sprite[0]==0){t.x=(index<<5)+16;t.y=176;w->sprite[i]=SpriteSystem_NewSprite(w->sys,w->manager,&t);break;}}ov89_0225A1D8(w,index);PlaySE(1509);}
