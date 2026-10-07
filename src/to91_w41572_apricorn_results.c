#pragma force_active on
#include "global.h"
#include "sprite.h"
#include "pokemon_types_def.h"
extern void ov59_0223BE44(u8 *, int, u8, int);
typedef struct {Pokemon *mon;u8 prefix04[12];s8 value[PERFORMANCE_MAX];u8 padding15;struct PokeathlonPerformanceStars stars;struct PokeathlonTodayPerformance performance;} Data;
typedef struct {u8 prefix[0x260];Sprite *sprites[34];} Work;
typedef char EntrySize[(sizeof(Data)==0x34)?1:-1];
typedef char StarsOffset[(offsetof(Data,stars)==0x16)?1:-1];
typedef char PerformanceOffset[(offsetof(Data,performance)==0x1e)?1:-1];
typedef char SpriteOffset[(offsetof(Work,sprites)==0x260)?1:-1];
void ov59_0223BD4C(void *,void *,u8);
// @fn ov59_0223BD4C
void ov59_0223BD4C(void *owner,void *entry,u8 n){Work*w=owner;Data*d=entry;int i,index;u8 level;struct {const u8 *bytes;int offset;} bar;if(d->value[n]==0)Sprite_SetDrawFlag(w->sprites[n+3],0);else{Sprite_SetDrawFlag(w->sprites[n+3],1);if(d->value[n]>0)Sprite_SetAnimCtrlSeq(w->sprites[n+3],8);else Sprite_SetAnimCtrlSeq(w->sprites[n+3],9);}index=n*5;bar.bytes=(const u8*)d+offsetof(Data,performance);bar.offset=n*sizeof(d->performance.stats[0]);level=(u8)((d->stars.stars>>(n*3))&7);for(i=0;i<5;i++,index++){if(i>(s32)(((const struct TodayPerformanceStat *)(bar.bytes+bar.offset))->hi))ov59_0223BE44((u8*)w,index,255,0);else if(i>level)ov59_0223BE44((u8*)w,index,7,1);else ov59_0223BE44((u8*)w,index,(u8)(d->stars.color[n]+4),1);}}
