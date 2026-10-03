#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u32 low,high;} Tick;
typedef struct{u32 a,b,c;} Friend;
typedef struct{
 void*ctx;void**peer;void*callback;u8 flag,players;u8 p0[7];u8 mode,limit;u8 p1[3];u16 port;u32 addr;u8 p2[4];u32 addresses[32];u16 ports[40];int profiles[32];Tick pending;u8 response;u8 p3[3];u32 attempts;Tick retry,wait;u8 p4[12];int state;u8 initial,current;u8 p5[14];u16 remotePort;u16 unused;u32 remoteAddr;int active;u8 p6[8];int delay;Tick time;u8 p7[0x2c];u32 identity;int target;u8 p8[8];u32 addr2[32];u16 port2[32];u8 slots[44];Friend*friends;u32 unused2;u8 friendIds[64];int friendCount;
} Work;
Work*ov00_021F955C(void);
void ov00_021F956C(int);
typedef struct{u8 pad[24];int busy;} Global;
extern Global ov00_0221A928;
extern char ov00_02217AAC[];
int ov00_022139C8(void*);
void*ov00_022139B8(void*,int);
int ov00_021F7C10(void*);
void ov00_0221390C(void*,void*);
u32 ov00_02212E78(void*);
u16 ov00_02212E80(void*);
int ov00_02212DB4(void*,char*,int);
void ov00_021F89CC(void*);
int ov00_021F89D0(int);
void ov00_021F8BA8(void);
int ov00_021F5B5C(int,int);
int ov00_021F5B30(void);
void ov00_021F3CF8(int);
void ov00_021F82EC(void);
void ov00_021F63DC(void);
void ov00_021F60AC(int);
int ov00_021F5D0C(int,int,int);
void ov00_021F3F5C(int,int,void*);
void ov00_021F8468(void);
void ov00_021F85B8(void*,int,void*);
void ov00_021F85B8(void*list,int event,void*arg){int i,profile;void*item;Work*w;
 ov00_0221A928.busy++;
 if(*(u64*)&ov00_021F955C()->pending){if((u32)(event-4)<=1){w=ov00_021F955C();*(u64*)&w->pending=0;}else{w=ov00_021F955C();*(u64*)&w->pending=OS_GetTick()+0xefb5f7;}}
 switch(event){case 0:ov00_021F89CC(arg);break;
 case 4:
 for(i=0;i<ov00_022139C8(list);i++){item=ov00_022139B8(list,i);if(!ov00_021F7C10(item)){ov00_0221390C(list,item);i--;}}
 switch(ov00_021F955C()->state){
 case 2:
 for(i=0;i<ov00_022139C8(list);i++){item=ov00_022139B8(list,i);if(ov00_021F955C()->addr&&ov00_021F955C()->addr==ov00_02212E78(item)&&ov00_021F955C()->port&&ov00_021F955C()->port==ov00_02212E80(item))break;}
 if(i<ov00_022139C8(list)){ov00_021F956C(3);ov00_021F955C()->target=0;ov00_021F3CF8(ov00_021F955C()->target);ov00_021F82EC();}
 else{*(int*)((u8*)ov00_021F955C()+0xe8)=2;w=ov00_021F955C();*(u64*)((u8*)w+0xec)=OS_GetTick();}break;
 case 3:
 ov00_021F89D0(1);ov00_021F8BA8();
 if(ov00_022139C8(list)){ov00_021F5B5C(0,0);if(ov00_021F5B30())goto end;ov00_021F956C(4);*(int*)((u8*)ov00_021F955C()+0xe8)=0;}
 else{*(int*)((u8*)ov00_021F955C()+0xe8)=2;w=ov00_021F955C();*(u64*)((u8*)w+0xec)=OS_GetTick();}break;
 case 5:
 if(ov00_022139C8(list)){i=0;do{item=ov00_022139B8(list,i);if(ov00_02212E78(item)==ov00_021F955C()->remoteAddr&&ov00_02212E80(item)==ov00_021F955C()->remotePort)break;ov00_0221390C(list,item);}while(ov00_022139C8(list));}
 if(ov00_022139C8(list)){profile=ov00_02212DB4(ov00_022139B8(list,0),ov00_02217AAC,0);
 if(ov00_021F955C()->mode==1&&profile==ov00_021F955C()->profiles[0]){
 if(ov00_021F89D0(0)){if(ov00_021F955C()->players){ov00_021F63DC();if(ov00_021F5B30())goto end;}}
 else{ov00_021F60AC(ov00_021F955C()->profiles[0]);if(ov00_021F5B30())goto end;ov00_021F956C(4);ov00_021F5D0C(0,0,ov00_021F955C()->profiles[0]);ov00_021F5B30();goto end;}}
 ov00_021F956C(6);ov00_021F3F5C(0,0,ov00_022139B8(list,0));ov00_021F8468();}
 else{*(int*)((u8*)ov00_021F955C()+0xe8)=2;w=ov00_021F955C();*(u64*)((u8*)w+0xec)=OS_GetTick();}break;
 }break;
 case 5:break;
 }
 end:ov00_0221A928.busy--;
}
