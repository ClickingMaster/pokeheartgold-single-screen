#include "global.h"
#pragma force_active on
typedef struct LinkState {void *start,*end;u32 unknown8;u32 *buffer;s32 count,length;u8 enabled,state,timer,flag,type;} LinkState;
typedef struct Entry {u32 address;u16 reserved,id;u8 flag,state,count,unknown;} Entry;
extern void *ov74_02231184(void),*ov74_0223115C(void);
extern u32 ov74_022311BC(void);
extern u8 ov74_02231094(void),ov74_022311AC(void);
extern void ov74_022313F0(void*);
void ov74_0223078C(void*);
static inline void ApplyIncomingPeer(Entry *entries,void *input,LinkState *w){
    int i;
    for(i=0;i<8;i++)if(*(u16*)((u8*)input+18)==entries[i].id){entries[i].flag=1;entries[i].state=3;w->timer=180;w->state=0;return;}
}
void ov74_0223078C(void *input){
    LinkState *w=ov74_02231184();u32 *packet;Entry *entries;
    if(ov74_02231094()!=2 || w->state==2)return;
    packet=w->buffer;
    DC_FlushRange(*(void**)((u8*)input+12),*(u16*)((u8*)input+16));
    MI_CpuCopy8(*(void**)((u8*)input+12),w->buffer,*(u16*)((u8*)input+16));
    if((packet[0]&255)!=ov74_022311AC())return;
    if(((packet[0]<<20)>>28)>ov74_022311BC())return;
    ov74_022313F0(w->buffer);
    if((packet[2]&255)!=253)return;
    entries=ov74_0223115C();ApplyIncomingPeer(entries,input,w);
}
