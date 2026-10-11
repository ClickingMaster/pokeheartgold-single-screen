#pragma force_active on
#include "global.h"
#include "to91_ov71_bounce_private.h"
#pragma force_active on
typedef struct Work {void *ctx;int state,count;u8 padc[8];void *pic;u8 pad18[0x2c];void*sprite;u8 pad48[0x30];void *resource;u32 other;SysTask *finished;void*narc;} Work;
extern void ov71_0224B9CC(void*,SysTask**),Sprite_SetAnimCtrlSeq(void*,int),Sprite_SetDrawFlag(void*,int),ov71_02247704(void*,int),Pokepic_SetAttr(void*,int,int),ov71_0224B910(Work*,int,int,int),sub_020729A4(void*,u8*,void*,int),PlayCryDelayed(void*,u8,u8),Pokepic_StartAnim(void*,int);
extern int ov71_0224B960(Work*),GetBoxMonData(void*,int,void*),TradeSequence_GetReceivedForm(void*);
extern void *TradeSequence_GetReceivedBoxMon(void*),*TradeSequence_GetReceivedSpecies(void*);
int ov71_0224B47C(Work*,u32*);
int ov71_0224B47C(Work*w,u32*state){u8 data[4];switch(*state){case 0:ov71_0224B9CC(w,&w->finished);(*state)++;case 1:if(w->finished==0){Sprite_SetAnimCtrlSeq(w->sprite,2);Sprite_SetDrawFlag(w->sprite,1);ov71_02247704(w->resource,0);w->count=0;(*state)++;}break;case 2:w->count++;if(w->count>10){Pokepic_SetAttr(w->pic,6,0);ov71_0224B910(w,16,0,12);w->state++;}break;case 3:if(ov71_0224B960(w)){if(GetBoxMonData(TradeSequence_GetReceivedBoxMon(w->ctx),76,0)==0){sub_020729A4(w->narc,data,TradeSequence_GetReceivedSpecies(w->ctx),1);void *mon=TradeSequence_GetReceivedSpecies(w->ctx);int form=TradeSequence_GetReceivedForm(w->ctx);PlayCryDelayed(mon,data[0],(u8)form);Pokepic_StartAnim(w->pic,1);}w->count=0;(*state)++;}break;case 4:w->count++;if(w->count>30)return 1;break;}return 0;}
