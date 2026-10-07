#pragma force_active on
#include "global.h"
#pragma force_active on
void PlaySE(u16);
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define SH(p,n) (*(s16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
#define SB(p,n) (*(s8*)((u8*)(p)+(n)))
typedef struct {void*mon;u8 gap[12];u16 hp;u8 gap2[5];u8 low:3,status:4,high:1;u8 tail[56];} Slot;
#define SLOT(s,n) (((Slot*)((u8*)(s)+4))[(n)])
void *BattleSystem_GetPartyMon(void*,int,int);void ov08_02220224(void*);int Pokemon_GetStatusIconId(void*);void ManagedSprite_SetDrawFlag(void*,int);void ov08_0221F5B0(void*,int);u32 GetMonData(void*,int,void*);void ov08_0221F550(void*,int);void ov08_02220064(void*,u16,int);void ov08_0221DBCC(void*,u16,int,void*);void ov08_022201C0(void*);void ov08_022225A4(void*);int ov08_0221CA90(void*);
typedef struct {u8 low:3,status:4,high:1;u8 pad[79];} FlagSlot;
typedef struct {u8 pad[8];void*battle,*other;u8 p10,index,special,p13,partyA,partyB;u8 gap[12];u16 battler,flag;u8 p26[2];u32 id;u8 party[6];u8 p32,ready,move;} Context;
typedef struct {Context*ctx;u8 gap[0x1fb8-4];void*sprites[19];u8 gap2[0x2076-0x1fb8-19*4];u8 selected,p77,p78,tag,mode,p7b,state,p7d;u16 hpGoal,ppGoal;}Work;
int ov08_0221CA90(void*s){Work*w=s;Context*ctx=w->ctx;switch(w->state){case 0:{SLOT(s,ctx->index).mon=BattleSystem_GetPartyMon(ctx->battle,ctx->id,ctx->party[ctx->index]);ov08_02220224(s);if(w->mode==5){w->ppGoal=GetMonData(SLOT(s,ctx->index).mon,ctx->move+58,0);w->state=2;}else{int status=Pokemon_GetStatusIconId(SLOT(s,ctx->index).mon);((FlagSlot*)((u8*)s+0x1b))[ctx->index].status=(u8)status;if(((FlagSlot*)((u8*)s+0x1b))[ctx->index].status==7){ManagedSprite_SetDrawFlag(w->sprites[ctx->index+13],0);ov08_0221F5B0(s,ctx->index);}w->hpGoal=GetMonData(SLOT(s,ctx->index).mon,163,0);w->state=4;}PlaySE(1516);}break;case 1:{u32 offset=80*ctx->index;u8*hp=(u8*)s+0x14;if(w->hpGoal!=H(hp,offset)){H(hp,offset)++;ov08_0221F550(s,ctx->index);}else w->state=3;}break;case 2:{u8*pp=(u8*)s+0x36+80*ctx->index;u32 offset=ctx->move*8;if(w->ppGoal!=pp[offset]){pp[offset]++;ov08_02220064(s,(u16)(ctx->move+1),ctx->move);}else w->state=3;}break;case 3:ov08_0221DBCC(ctx->battle,ctx->battler,ctx->ready,ctx->other);ov08_022201C0(s);w->tag=25;return 17;case 4:{u32 offset=80*ctx->index;u8*hp=(u8*)s+0x14;if(w->hpGoal!=H(hp,offset)){H(hp,offset)++;ov08_0221F550(s,ctx->index);ov08_022225A4(s);}w->state=1;}break;}return 23;}
