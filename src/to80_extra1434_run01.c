#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u32 words[4];struct{u8 bytes[32];}a;struct{u8 bytes[20];}b;}Packet;
typedef struct{int state;u8 p[0x68-4];int active;u8 q[8];Packet packet;u8 r[0x134-0xb8];}Work;
extern Work *volatile _022577C4;
static inline int release(Work*w){if(w->active){w->active=0;return 1;}return 0;}
extern void ov45_0223325C(int,int,int,int,int,void*,int),ov45_02232DDC(void*,void*,void*),ov45_02236C40(void*,void*,void*);
int ov45_02232B9C(int*);int ov45_02232BB0(Packet*);int ov45_02232CA4(void);int ov45_02232D8C(void*,void*);
int ov45_02232B9C(int*p){if(p[1]==0)return 0;return p[0];}
int ov45_02232BB0(Packet*p){Work*w=_022577C4;int result;if(!w)return 4;if(w&&w->state==10)return 7;if(w&&_022577C4->state!=8)return 4;w=_022577C4;if(!w->active)result=0;else{w->packet.words[0]=p->words[0];w->packet.words[1]=p->words[1];w->packet.words[2]=p->words[2];w->packet.words[3]=p->words[3];w->packet.a=p->a;w->packet.b=p->b;result=1;}if(!result)return 4;return 0;}
int ov45_02232CA4(void){int value;Work*w=_022577C4;int result;if(!w)return 4;if(w&&w->state==10)return 7;if(w&&_022577C4->state!=8)return 4;w=_022577C4;result=release(w);if(!result)return 4;w=_022577C4;ov45_0223325C(*(int*)((u8*)w+0x4c),0,1,0,2,&w->packet,0x44);value=3;ov45_02232D8C((u8*)_022577C4+0x134,&value);return 0;}
int ov45_02232D8C(void*w,void*value){struct{void*argument,*out,*found;}local;ov45_02232DDC(&local.found,w,value);if(local.found==(u8*)w+4)return 0;local.argument=local.found;ov45_02236C40(&local.out,w,local.argument);return 1;}
