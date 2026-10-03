#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct { u32 a,x,b; ManagedSprite *sprite; u32 y; s16 dx,dy; u8 ticks,scale,pad[2]; } Rec;
typedef struct { Rec rec[12]; Rec extra[2]; u8 pad188[0x74]; int state,timer,count,flag; u32 x20c; int first,last,first2,last2; u8 pad220[0x80]; int arg,mode; } Work;
extern f32 ov40_02245E74[],ov40_02245E58[];
extern void ov40_02243E80(Work *,int,int);
extern int ov40_02244054(u32,u32);
extern void ov40_022441F8(Work *);
extern int ov40_022439CC(Work *,int),ov40_022439F4(Work *,int);
extern void ov40_02243EB0(Work *,int),ov40_022439B8(Work *),ov40_0224320C(Work *,int);
int ov40_022432AC(Work *);
int ov40_022432AC(Work *w) {
 int i; Rec *r;
 switch(w->state) { case 0: {
  ov40_02243E80(w,0,0);
  for(i=0;i<w->count;i++) {
   r=w->rec+i;
   if(r->ticks) {
    ManagedSprite_OffsetPositionXY(r->sprite,r->dx,r->dy); r->ticks--;
    if(i>=w->first && i<w->last) { ManagedSprite_SetAffineScale(r->sprite,ov40_02245E74[r->scale],ov40_02245E74[r->scale]); r->scale++; ManagedSprite_SetAffineOverwriteMode(r->sprite,2); }
    if(i>=w->first2 && i<w->last2) { ManagedSprite_SetAffineScale(r->sprite,ov40_02245E58[r->scale],ov40_02245E58[r->scale]); r->scale++; ManagedSprite_SetAffineOverwriteMode(r->sprite,2); }
   }
  }
  for(i=0;i<2;i++) {
   r=w->rec+i;
   if(((Work *)r)->extra[0].ticks) { ManagedSprite_OffsetPositionXY(((Work *)r)->extra[0].sprite,((Work *)r)->extra[0].dx,((Work *)r)->extra[0].dy); ((Work *)r)->extra[0].ticks--; }
  }
  if(!w->rec[0].ticks) {
   for(i=w->first;i<w->last;i++) { r=w->rec+i; ManagedSprite_SetAnim(r->sprite,ov40_02244054(r->a,r->b)); ManagedSprite_TickFrame(r->sprite); }
   for(i=w->first2;i<w->last2;i++) { r=w->rec+i; ManagedSprite_SetAnim(r->sprite,ov40_02244054(r->a,r->b)); ManagedSprite_TickFrame(r->sprite); }
   w->state++;
  }
  w->timer++;
 } break; case 1: {
  for(i=w->first;i<w->last;i++) { r=w->rec+i; if(r->scale==6) ManagedSprite_SetAffineOverwriteMode(r->sprite,1); else { ManagedSprite_SetAffineScale(r->sprite,ov40_02245E74[r->scale],ov40_02245E74[r->scale]); r->scale++; } }
  for(i=w->first2;i<w->last2;i++) { r=w->rec+i; if(r->scale==6) ManagedSprite_SetAffineOverwriteMode(r->sprite,1); else { ManagedSprite_SetAffineScale(r->sprite,ov40_02245E58[r->scale],ov40_02245E58[r->scale]); r->scale++; } }
  w->timer++; if(w->timer==6)w->state++;
 } break; default: {
  ov40_022441F8(w);
  if(w->mode==0)ov40_02243EB0(w,ov40_022439CC(w,w->arg)); else ov40_02243EB0(w,ov40_022439F4(w,w->arg));
  if(w->flag)ov40_02243E80(w,0,1);
  ov40_022439B8(w);ov40_0224320C(w,1);
 } break; }
 return 0;
}
