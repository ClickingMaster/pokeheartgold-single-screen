#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u8 pad[28];void*list;u8 rest[0x76c-32];};struct R{int a;void*sprite;};struct Template{void*list;void*resources;int x,y,z,sx,sy,sz,angle,priority,screen,heap;};void*Sprite_CreateAffine(const void*);void Sprite_SetDrawFlag(void*,int);void Sprite_SetPaletteOverride(void*,int);void ov91_02260C48(struct S*,struct R*,int,int);
void ov91_02260C48(struct S*s,struct R*r,int palette,int heap){struct Template t={0};t.list=s->list;t.resources=s->rest+sizeof(s->rest);t.heap=heap;t.sx=4096;t.sy=4096;t.sz=4096;t.priority=32;t.screen=2;r->sprite=Sprite_CreateAffine(&t);Sprite_SetDrawFlag(r->sprite,0);Sprite_SetPaletteOverride(r->sprite,palette);r->a=0;}
