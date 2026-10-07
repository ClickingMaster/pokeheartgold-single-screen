#pragma force_active on
#include "global.h"
#include "battle/battle.h"
#include "battle/battle_system.h"
#include "battle/battle_controller.h"
#include "pokemon.h"
#pragma force_active on
typedef struct Packet {u8 command,mode,effect,status;u8 ids[4],party[4][6],embargo[4];} Packet;
extern void BattleBuffer_Clear(BattleContext*,int);
extern void ov12_02262240(BattleSystem*,int,int,const void*,u8);
typedef char PacketSize[(sizeof(Packet)==36)?1:-1];
static inline void SetMonFlags(Packet *packet,const BattleMon *mon) {
 packet->mode=0;
 if(mon->moveEffectFlags&0x200400c0){packet->effect=1;packet->status=0;}
 else if(mon->status2&0x1000000){packet->effect=0;packet->status=1;}else{packet->effect=0;packet->status=0;}
}
void ov12_02263138(BattleSystem *battle,BattleContext *context,int battler) {
 Packet packet;int i,j;BattleBuffer_Clear(context,battler);packet.command=17;
 for(i=0;i<4;i++){packet.ids[i]=context->selectedMonIndex[i];for(j=0;j<6;j++)packet.party[i][j]=context->unk_312C[i][j];packet.embargo[i]=context->battleMons[i].unk88.embargoFlag;}
 if(BattleSystem_GetBattleType(battle)==74){
  if(!(MaskOfFlagNo(1)&context->switchInFlag)&&!(MaskOfFlagNo(3)&context->switchInFlag)){packet.mode=1;packet.effect=0;packet.status=0;}
  else if(!(MaskOfFlagNo(1)&context->switchInFlag))SetMonFlags(&packet,&context->battleMons[1]);
  else SetMonFlags(&packet,&context->battleMons[3]);
 }else if(BattleSystem_GetBattleType(battle)==0)SetMonFlags(&packet,&context->battleMons[1]);
 else{packet.mode=0;packet.effect=0;packet.status=0;}
 ov12_02262240(battle,1,battler,&packet,36);
}
