#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{void *sprite;u8 pad[12];s16 y;u8 kind,timer,pad2,active;s16 remaining;} Entry;
typedef struct{u8*inner;}Work;
typedef struct{s16 x,y;}XY;
extern u16 ov93_02262D2C[][4],ov93_02262C7A[];
extern u8 ov93_02262CD8[][4];
extern const s16 ov93_02262CB4[][2],ov93_02262CB6[][2];
extern int Comm_GetLocalNetId(void),ov93_0225E3C4(Work*,int);
extern void ManagedSprite_SetPaletteOverride(void*,int),ManagedSprite_SetAnim(void*,int),ManagedSprite_SetPositionXYWithSubscreenOffset(void*,s16,s16,int),Sprite_TickFrame(void*);

void ov93_02261C58(Work*w,Entry*entries,int kind,int id,int duration);

void ov93_02261C58(Work*w,Entry*entries,int kind,int id,int duration){int kindCopy=kind;Entry*p=0;int i,index,coord;if(kindCopy==0)return;if(id==Comm_GetLocalNetId())return;for(i=0;i<3;i++)if(entries[i].active==0){p=&entries[i];break;}if(p==0)return;index=ov93_0225E3C4(w,id);ManagedSprite_SetPaletteOverride(p->sprite,ov93_02262D2C[w->inner[0x30]][index]);ManagedSprite_SetAnim(p->sprite,ov93_02262C7A[kindCopy]);coord=ov93_02262CD8[w->inner[0x30]][index];{s16 x=ov93_02262CB4[coord][0];p->y=ov93_02262CB6[coord][0]-24;ManagedSprite_SetPositionXYWithSubscreenOffset(p->sprite,x,p->y-96,22<<16);}Sprite_TickFrame(*(void**)p->sprite);p->kind=kindCopy;p->remaining=duration-12;p->timer=0;p->active=1;}
