#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct State{u8 prefix[0x314];u16 values[4];u8 bytes[4];u32 words[4];u8 rest[0xa7c-0x330];u8 counter;}State;
typedef struct{u16 values[4],bytes[4],low[4],high[4];}Packet;

int sub_02037030(int,void *,int),sub_0203769C(void),ov80_0222BB54(State *);void ov80_0222BBD0(int,int,Packet *,State *);
void ov80_0222BBD0(int sender,int unused,Packet *packet,State *s){int i;s->counter++;if(sender==sub_0203769C())return;if(sub_0203769C()==0)return;{for(i=0;i<4;i++)s->values[i]=packet->values[i];for(i=0;i<4;i++)s->bytes[i]=packet->bytes[i];{u8*cursor;i=0;cursor=(u8*)packet+16;for(;i<4;i++,cursor+=2){s->words[i]=*(u16*)cursor;s->words[i]|=(u32)*(u16*)(cursor+8)<<16;}}}}
