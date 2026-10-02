#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 words[13];} Template;
typedef struct {u8 pad[0x14];void **sys;u8 pad2[0xe0-0x18];void *first[2],*second[2];} State;
extern void ov92_0225DDD8(Template *,int,int,int,int,int);
extern void *SpriteSystem_NewSprite(void *,void *,Template *);
extern void ManagedSprite_SetAnim(void *,int),ManagedSprite_TickFrame(void *),ManagedSprite_SetPaletteOverrideOffset(void *,int),ManagedSprite_SetOamMode(void *,int),ManagedSprite_SetDrawFlag(void *,int);
void ov92_0225E820(State *);
void ov92_0225E820(State *s){Template t;void *a=s->sys[2],*c=s->sys[3];int i;for(i=0;i<2;i++){ov92_0225DDD8(&t,0,0,1,4,0x232a);t.words[11]=2;t.words[2]=20;s->second[i]=SpriteSystem_NewSprite(a,c,&t);ManagedSprite_SetAnim(s->second[i],0);ManagedSprite_TickFrame(s->second[i]);ManagedSprite_SetPaletteOverrideOffset(s->second[i],4);ManagedSprite_SetOamMode(s->second[i],1);ManagedSprite_SetDrawFlag(s->second[i],0);ov92_0225DDD8(&t,0,0,2,0,0x2328);t.words[11]=2;t.words[2]=20;s->first[i]=SpriteSystem_NewSprite(a,c,&t);ManagedSprite_SetAnim(s->first[i],6);ManagedSprite_TickFrame(s->first[i]);ManagedSprite_SetPaletteOverrideOffset(s->first[i],0);ManagedSprite_SetOamMode(s->first[i],1);ManagedSprite_SetDrawFlag(s->first[i],0);}}
