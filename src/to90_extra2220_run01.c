#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[788];u16 values[4];u8 gap[264];u16 packet[20];}Half;typedef struct{u8 pad[800];u32 words[4];}Words;typedef struct State {u8 pad0[788];u16 values[4];u8 bytes[4];u8 pad320[0];u32 words[4];u8 pad330[244];u16 packet[20];u8 pad44c[1584];u8 counter;} State;
int sub_02037030(int,void *,int),sub_0203769C(void),ov80_0222BB54(State *);void ov80_0222BBD0(int,int,u16 *,State *);
int ov80_0222BB54(State *s){int i;Half*cursor;i=0;cursor=(Half*)s;for(;i<4;i++,cursor=(Half*)((u8*)cursor+2))cursor->packet[0]=cursor->values[0];{Half*second;int j=0;second=(Half*)s;for(;j<4;j++,second=(Half*)((u8*)second+2))second->packet[4]=s->bytes[j];}{Words*word;Half*packet;i=0;word=(Words*)s;packet=(Half*)((u8*)s+16);for(;i<4;i++,word=(Words*)((u8*)word+4),packet=(Half*)((u8*)packet+2)){packet->packet[0]=word->words[0];packet->packet[4]=word->words[0]>>16;}}if(sub_02037030(67,s->packet,40)==1)return 1;return 0;}
