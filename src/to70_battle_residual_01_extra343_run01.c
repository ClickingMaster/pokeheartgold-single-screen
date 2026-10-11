#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 species;u8 slot:4;u8 changed:4;u16 hp;u32 maxhp;u32 mask;u16 item;u16 moves[4];u8 pp[4];u16 pad;u32 flags;u32 experience;u32 personality;u16 levelFlag;u16 experienceFlag;} SaveBlock;
typedef struct {u8 pad[0x94];SaveBlock block;u8 tail[0xd4];u8 battler;} Context;
extern void *BattleSystem_GetPartyMon(void *,int,int);
extern u32 MaskOfFlagNo(int);
extern void SetMonData(void *,int,const void *);
extern void CalcMonLevelAndStats(void *);
extern void BattleController_EmitCommandComplete(void *,int,int);
extern void ov12_02259928(Context *);
void ov12_022591F4(void *,Context *);
void ov12_022591F4(void *bs,Context *ctx){
 SaveBlock *b=&ctx->block;void *mon=BattleSystem_GetPartyMon(bs,ctx->battler,b->slot);int i;
 if(!(b->flags & (2<<20))){for(i=0;i<4;i++){if(!(MaskOfFlagNo(i)&b->changed)){SetMonData(mon,i+0x36,&b->moves[i]);SetMonData(mon,i+0x3a,&b->pp[i]);}}}
 if(!(MaskOfFlagNo(b->slot)&b->mask))SetMonData(mon,6,&b->item);
 SetMonData(mon,0xa3,&b->hp);SetMonData(mon,0xa0,&b->maxhp);
 if(b->experienceFlag)SetMonData(mon,0x70,&b->experience);
 if(b->levelFlag){SetMonData(mon,0xa,&b->personality);CalcMonLevelAndStats(mon);}
 BattleController_EmitCommandComplete(bs,ctx->battler,b->species);ov12_02259928(ctx);
}
