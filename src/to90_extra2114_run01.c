#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef void *(*Alloc)(const void *,u32);
typedef void (*Release)(const void *,void *,int);
typedef struct {u32 a,b;s32 state;u8 pad[0x200];Alloc alloc;Release release;u8 pad2[0x100];u32 request;} Tail;
typedef struct {u8 prefix[0x1000];Tail tail;} State;
typedef struct {u32 a,b;State *state;} Global;
extern Global ov00_0221A694;
extern u8 ov00_022164A4[],ov00_022164B4[],ov00_0221A6A0[];
extern int ov00_021E9F2C(u32,int),ov00_021E84D4(void),DWCi_AUTH_UpDateWiFiID(void *,void *);
extern void DWCi_AUTH_RemakeWiFiID(void *),DWCi_AUTH_MakeWiFiID(void *);
int ov00_021E82FC(void);
#pragma force_active on
int ov00_021E82FC(void){
 Alloc alloc=ov00_0221A694.state->tail.alloc;Release release=ov00_0221A694.state->tail.release;void *block;s32 state;
 if(ov00_021E9F2C(ov00_0221A694.state->tail.request,0)!=1){ov00_0221A694.state->tail.state=0x4e84;return 14;}
 if(ov00_021E84D4()!=0)return 14;
 state=ov00_0221A694.state->tail.state;
 if(state<0x4e84){
  if(state==0x4e22){
   block=alloc(ov00_022164A4,0x71f);
   if(!block){ov00_0221A694.state->tail.state=0x4e84;return 2;}
   if(DWCi_AUTH_UpDateWiFiID(ov00_0221A6A0,(void *)(((u32)block+31)&~31u))!=1){release(ov00_022164B4,block,0);ov00_0221A694.state->tail.state=0x4e84;return 15;}
   release(ov00_022164B4,block,0);
  }
  return 21;
 }
 switch(state){case 0x4e88:{DWCi_AUTH_RemakeWiFiID(ov00_0221A6A0);ov00_0221A694.state->tail.state=0x4e88;return 16;}
 case 0x4e8c:{
  block=alloc(ov00_022164A4,0x700);
  if(!block){ov00_0221A694.state->tail.state=0x4e8c;return 17;}
  DWCi_AUTH_MakeWiFiID((void *)(((u32)block+31)&~31u));release(ov00_022164B4,block,0);ov00_0221A694.state->tail.state=0x4e8c;return 17;
 }
 default:return 18;}
}
