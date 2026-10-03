#pragma force_active on
#include "global.h"
typedef struct {u8 p[12];void *sprite;u32 q;s16 dx,dy;u8 frames,timer,pad[2];} Entry;
typedef struct {Entry entries[12];u8 raw[0x208];} Work;
#define I(o,n) (*(int*)((u8*)(o)+(n)))
extern void ManagedSprite_GetPositionXY(void*,s16*,s16*),ManagedSprite_SetPositionXY(void*,s16,s16);void ov40_022440A0(Work*,int);
void ov40_022440A0(Work*w,int animate){int i=0,extra=0;s16 y=((s16*)((u8*)w+0x1dc))[I(w,0x208)];s16 position[2];if(I(w,0x204)>0){Entry *entry=w->entries;u8 *base=(u8*)w;do{if(i>=I(w,0x210)&&i<I(w,0x214)){if(i==I(w,0x210))y+=20;else y+=32;}else{if(i==0)y+=20;else y+=8;}ManagedSprite_GetPositionXY(entry->sprite,&position[1],&position[0]);if(!animate)ManagedSprite_SetPositionXY(entry->sprite,y,position[0]);else{entry->dx=(y-position[1])/2;entry->dy=0;entry->frames=2;entry->timer=0;}if(i==I(base,0x150)&&extra!=2){ManagedSprite_GetPositionXY(*(void**)(base+0x15c),&position[1],&position[0]);if(I(w,0x210)==I(w,0x214))y+=8;else if(i>I(w,0x210)&&i<I(w,0x214))y+=20;else y+=8;if(!animate)ManagedSprite_SetPositionXY(*(void**)(base+0x15c),y,position[0]);else{*(s16*)(base+0x164)=(y-position[1])/2;*(s16*)(base+0x166)=0;base[0x168]=2;}base+=28;extra++;}entry++;i++;}while(i<I(w,0x204));}}
