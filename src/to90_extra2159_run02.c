#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {s16 x,y;u8 kind,anim;u16 frame:13;u16 other:1;u16 draw:1;u16 active:1;} Record;
typedef struct {u16 frame:13;u16 other:1;u16 draw:1;u16 active:1;} Flags;
typedef struct {u16 ids[8];Record sprites[8];} Map;
typedef struct {u32 pad;int heap;void *data;u8 pad2[0x28];void *system,*manager;} Work;
extern Map *sub_02096878(void *);extern void *NARC_New(int,int),*ov80_0223968C(Work *,u16,u8);
extern void ov80_02239AF8(void *,void *,void *,int,u16),ov80_0223962C(Work *,u16),ManagedSprite_SetPositionXY(void *,s16,s16),ManagedSprite_SetDrawFlag(void *,int),ov80_02239708(Work *,u16,int),ManagedSprite_SetAnim(void *,int),ManagedSprite_SetAnimationFrame(void *,int),NARC_Delete(void *),sub_02096884(void *);
void ov80_02239828(Work *);
void ov80_02239828(Work *w){
Map *map=sub_02096878(w->data);void *narc=NARC_New(184,101);int i;void *sprite;Flags *r;Map *cursor;
for(i=0;i<8;i++){if(map->ids[i]!=65535){ov80_02239AF8(w->system,w->manager,narc,w->heap,map->ids[i]);ov80_0223962C(w,map->ids[i]);}}
r=(Flags *)map;cursor=map;i=0;r=(Flags *)((u8 *)r+22);for(;i<8;i++,r=(Flags *)((u8 *)r+8),cursor=(Map *)((u8 *)cursor+8)){if(r->active==1){sprite=ov80_0223968C(w,(u16)i,cursor->sprites[0].kind);ManagedSprite_SetPositionXY(sprite,cursor->sprites[0].x,cursor->sprites[0].y);ManagedSprite_SetDrawFlag(sprite,r->draw);ov80_02239708(w,(u16)i,r->other);ManagedSprite_SetAnim(sprite,cursor->sprites[0].anim);ManagedSprite_SetAnimationFrame(sprite,r->frame);}}
NARC_Delete(narc);sub_02096884(w->data);
}
