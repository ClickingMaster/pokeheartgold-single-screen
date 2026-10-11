#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Particle { void *sprite, *marker; s16 health; } Particle;
typedef struct Work { void *root; u8 p4[8]; void *cursor, *miss; u8 effects[0x140]; Particle *entries[10]; struct {u32 low:16;u32 high:16;} counters; struct {u32 pad:4;u32 count:4;u32 rest:24;} flags; } Work;
extern void ManagedSprite_SetPositionXY(void *, s16, s16);
extern void ManagedSprite_SetAnim(void *, int);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern void ManagedSprite_GetPositionXY(void *, s16 *,s16 *);
extern void ov96_0220B1D8(void *,int,s16,s16);
extern int ov96_021E5F24(void *);
extern void ov96_021E8228(void *,u8,int,int,int);
extern int LCRandom(void);
extern int System_GetTouchNewCoords(int *,int *);
extern void PlaySE(int);
extern void GF_SndSetTrackPitchBySeqNo(int,int,int);
u16 ov96_0220AAEC(Work *,int,s16,int,int,int *);
u16 ov96_0220AAEC(Work *w,int event,s16 power,int boost,int extra,int *finished) {
 int count;
 int removed=0,i,anim;
 int x,y;
 struct {s16 y,x;} xy;
 GF_ASSERT(w!=NULL);
 if(!w->flags.count)return 0;
 System_GetTouchNewCoords(&x,&y);
 anim=7;
 ManagedSprite_SetPositionXY(w->cursor,(s16)x,(s16)y);
 if(boost || extra)anim=18;
 ManagedSprite_SetAnim(w->cursor,anim);
 ManagedSprite_SetDrawFlag(w->cursor,1);
 if(extra){power+=150;PlaySE(2235);}else if(boost){power+=160;PlaySE(2235);}
 count=w->flags.count;
 for(i=0;i<count;i++) {
 Particle *p=w->entries[count-(i+1)];
 if(power<=0)break;
 p->health-=power;
 if(p->health>0)break;
 if(p->health<=0) {
 w->flags.count--;
 w->counters.high++;
 if(w->counters.high>200)w->counters.high=200;
 ManagedSprite_SetAnim(p->sprite,0);
 ManagedSprite_GetPositionXY(p->sprite,&xy.x,&xy.y);
 ManagedSprite_SetDrawFlag(p->marker,0);
 power=-p->health;removed++;
 ov96_0220B1D8(w->effects,4,xy.x,xy.y);
 ov96_021E8228(w->root,(u8)ov96_021E5F24(w->root),event,3,1);
 }
 }
 if(removed)PlaySE(2233);
 else {PlaySE(2232);ManagedSprite_SetPositionXY(w->miss,(s16)x,(s16)y);ManagedSprite_SetAnim(w->miss,6);ManagedSprite_SetDrawFlag(w->miss,1);}
 if(!removed && w->flags.count) {
 Particle *p=w->entries[w->flags.count-1];
 if(p->health<=20)ManagedSprite_SetAnim(p->sprite,3);
 else if(p->health<=60){ManagedSprite_SetAnim(p->sprite,2);PlaySE(2232);GF_SndSetTrackPitchBySeqNo(2232,65535,64-((int)LCRandom()%128));}
 }
 *finished=removed>0 && w->flags.count==0;
 return (u16)removed;
}
