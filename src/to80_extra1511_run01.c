#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct{u8 cmd,slot;u16 value;} Packet;extern void ov12_02262240(void*,int,int,void*,int);void BattleControl_EmitPartyStatusHeal(void*,u8*,int,u16);
void BattleControl_EmitPartyStatusHeal(void*p,u8*q,int battler,u16 value){Packet pkt;pkt.cmd=42;pkt.value=value;pkt.slot=B(q+battler*192,0x2d67);ov12_02262240(p,1,battler,&pkt,4);}
