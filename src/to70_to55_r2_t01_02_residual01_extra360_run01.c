#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *Heap_Alloc(int,u32);extern void Heap_Free(void *);
typedef struct {u8 state;u8 pad[3];void *owner,*system,*manager,*original;void **sprites[3];} Work;
extern void Sprite_TickFrame(void *);extern void Sprite_OffsetPositionXY(void *,int,int);extern void SpriteSystem_DrawSprites(void *);extern void Sprite_DeleteAndFreeResources(void *);extern void ov07_0221C448(void *,void *);
extern void ov07_0221F9E8(void *,void *);extern void *SpriteSystem_NewSprite(void *,void *,void *);extern void ov07_0221C3F4(void *,void (*)(void *,Work *),void *,int);
void ov07_0222314C(void *,Work *);void ov07_022231BC(void *,void *,void *,void *);
void ov07_0222314C(void *task,Work *w){switch(w->state){case 0:Sprite_TickFrame(*w->sprites[0]);Sprite_TickFrame(*w->sprites[1]);Sprite_TickFrame(*w->sprites[2]);Sprite_OffsetPositionXY(*w->sprites[0],1,0);Sprite_OffsetPositionXY(*w->sprites[1],-1,0);Sprite_OffsetPositionXY(*w->sprites[2],1,1);SpriteSystem_DrawSprites(w->manager);break;case 1:{void *owner=w->owner;Sprite_DeleteAndFreeResources(w->original);Heap_Free(w);ov07_0221C448(owner,task);break;}}}
void ov07_022231BC(void *owner,void *system,void *manager,void *original){Work *w=Heap_Alloc(5,0x1cc);u8 template[0x34];int i;if(!w)GF_AssertFail();w->state=0;w->system=system;w->manager=manager;w->original=original;w->owner=owner;ov07_0221F9E8(template,owner);w->sprites[0]=original;for(i=1;i<3;i++)w->sprites[i]=SpriteSystem_NewSprite(w->system,w->manager,template);ov07_0221C3F4(owner,ov07_0222314C,w,0x1000);}
