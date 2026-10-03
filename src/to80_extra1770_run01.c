#pragma force_active on
#include "global.h"
typedef struct {u16 moves[4];u8 p[24];u8 pp[4],ups[4];u8 q[0x98];} Mon;
typedef struct {u8 p[0x219c];u8 ids[4];u8 q[0xbac];Mon mon[4];} Context;
typedef struct {u8 command,id;u16 struggle,moves[4];u8 pp[4],max[4];} Packet;
extern void *BattleSystem_GetBattleContext(void*);extern void BattleBuffer_Clear(void*,int);extern int GetMoveMaxPP(int,int);extern int StruggleCheck(void*,Context*,int,int,int);extern void ov12_02262240(void*,int,int,void*,int);
void ov12_02262F40(void*,Context*,int);
#pragma force_active on
#pragma opt_lifetimes off
void ov12_02262F40(void*sys,Context*ctx,int id) {Packet packet;int i;BattleBuffer_Clear(BattleSystem_GetBattleContext(sys),id);packet.command=15;packet.id=ctx->ids[id];for(i=0;i<4;i++){packet.moves[i]=ctx->mon[id].moves[i];packet.pp[i]=ctx->mon[id].pp[i];packet.max[i]=GetMoveMaxPP(ctx->mon[id].moves[i],ctx->mon[id].ups[i]);}packet.struggle=StruggleCheck(sys,ctx,id,0,-1);ov12_02262240(sys,1,id,&packet,20);}
