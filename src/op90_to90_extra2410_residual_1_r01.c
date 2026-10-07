#pragma force_active on
#include "global.h"

typedef struct {u32 zero,id,connected;void (*notify)(int, int);u32 pad[2];u32 blocked;void (*receive)(void *,u8,u8,u8);u32 own,started,state,active;u64 tick;} W40083State;
typedef struct {u8 command;u8 value,pad[2];u32 id;} W40083Packet;
extern W40083State _021FF9E0;
extern W40083Packet ov112_021FFA18;
extern u8 _021FF500[];
extern u8 ov112_021FFA20[];
extern u32 CARD_SpiWaitReadRange(void *);
extern int ov112_021E5A84(u32),ov112_021E5AD0(void),ov112_021E5B28(u32);
extern void ov112_021E59B4(int,int,int,int,u32),ov112_021E5938(void),ov112_021E5A80(void);
int ov112_021E5B98(void);
#pragma opt_common_subs off
// @fn ov112_021E5B98
int ov112_021E5B98(void){
 W40083Packet *pkt;
 u32 length=CARD_SpiWaitReadRange(&ov112_021FFA18);
 if(ov112_021E5A84(length))length=0;
 if(length==0){if(ov112_021E5AD0())return 1;return 0;}
 if(!ov112_021E5B28(length))return 0;
 _021FF9E0.tick=OS_GetTick();
 pkt=&ov112_021FFA18;
 if(pkt->command<240){if(!_021FF9E0.active)return 0;if(pkt->id!=_021FF9E0.id)return 0;}
 switch(pkt->command){
 case 252:
  switch(_021FF9E0.state){case 1:_021FF9E0.state=2;ov112_021E59B4(0,0,250,_021FF500[0],_021FF9E0.own);break;case 0:case 2:case 3:case 4:default:break;}
  if(_021FF9E0.started)break;
  _021FF9E0.started=1;_021FF9E0.state=2;_021FF9E0.connected=0;
  if(length>1&&!_021FF9E0.active)break;
  ov112_021E59B4(0,0,250,_021FF500[0],_021FF9E0.own);
  if(_021FF9E0.active)ov112_021E5938();
  ov112_021E5A80();break;
 case 250:
  if(_021FF9E0.state!=1)break;
  _021FF9E0.state=3;_021FF9E0.connected=1;_021FF500[1]=pkt->value;
  if(_021FF500[1]!=1)break;
  _021FF9E0.id=pkt->id^_021FF9E0.own;
  ov112_021E59B4(0,0,248,_021FF500[0],_021FF9E0.own);
  _021FF9E0.active=1;_021FF9E0.blocked=0;_021FF9E0.started=0;break;
 case 248:
  if(_021FF9E0.state!=2)break;
  _021FF9E0.state=4;_021FF500[1]=pkt->value;
  _021FF9E0.id=pkt->id^_021FF9E0.own;
  _021FF9E0.active=1;_021FF9E0.blocked=0;_021FF9E0.started=0;
  if(_021FF9E0.notify)_021FF9E0.notify(pkt->value, 0);break;
 case 246:ov112_021E5938();break;
 default:
  if(_021FF9E0.blocked!=1){if(_021FF9E0.active)_021FF9E0.receive(ov112_021FFA20,(u8)(length-8),ov112_021FFA18.command,ov112_021FFA18.value);}break;
 }
 return 0;
}
#pragma opt_common_subs reset
