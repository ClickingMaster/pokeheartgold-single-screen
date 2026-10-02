#pragma force_active on
#include "global.h"
#include "heap.h"
typedef struct {u32 a;void *parent;u32 b;void *system;u8 pad[12];void *sprite;s16 delta;u16 pad2;int first,second,target;} Work;
extern void ov07_0221C448(void *,void *);extern void ManagedSprite_TickFrame(void *);extern void SpriteSystem_DrawSprites(void *);
void ov07_02225F18(void *,Work *);
void ov07_02225F18(void *task,Work *w){if(w->first==w->target||w->second==w->target){ov07_0221C448(w->parent,task);Heap_Free(w);return;}if(w->delta<0){if(w->first+w->delta>0){w->first+=w->delta;w->second+=w->delta;}else{w->first=w->target;w->second=w->target;}}else{if(w->first+w->delta<15){w->first+=w->delta;w->second+=w->delta;}else{w->first=w->target;w->second=w->target;}}ManagedSprite_TickFrame(w->sprite);*(vu8 *)0x0400004d=w->first|(w->second<<4);SpriteSystem_DrawSprites(w->system);}
