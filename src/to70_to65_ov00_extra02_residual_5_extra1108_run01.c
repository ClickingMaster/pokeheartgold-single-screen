#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u8 p[8];char*data;}Global;
typedef struct{u8 p[0x10];int state;u8 p2[0xe4-0x14];void*object;u8 p3[0x200-0xe8];void*handle;u8 p4[0x2f4-0x204];int a,b;u8 p5[0x474-0x2fc];void*callback,*arg;}Work;
extern Global ov00_0221A928;
extern char ov00_02217A58[],ov00_02217A60[];
extern void ov00_021EC2EC(int,void*,int),ov00_021F2EE4(int,int),ov00_021F3BB4(int,int,void*,void*),ov00_021F956C(int),ov00_021F85B8(void),ov00_021F04A0(int,char*,int),ov00_021F179C(void*),ov00_021F7DDC(int);
extern int ov00_021F3EF4(char*,int,int,int),ov00_021F82EC(int),ov00_021F3CF8(void*);
extern void *ov00_021EC2A8(int,int),*ov00_02213610(int,int,int,int,int,int,int,void(*)(void),int);
extern Work *ov00_021F955C(void);

void ov00_021F1938(int id,char*text,void*config,void*task,void*callback,void*arg);

void ov00_021F1938(int id,char*text,void*config,void*task,void*callback,void*arg){int size;char buf[256];Global *global=&ov00_0221A928;Work *second,*first,*dest,*third;char *old=global->data;if(old){ov00_021EC2EC(4,old,0);ov00_0221A928.data=0;}if(text){size=ov00_021F3EF4(buf,-1,32,3);size=256-size-strlen(ov00_02217A58);ov00_0221A928.data=ov00_021EC2A8(4,size);if(ov00_0221A928.data==0){ov00_021F2EE4(9,(int)0xfffec77f);return;}MI_CpuCopy8(text,ov00_0221A928.data,size);*(ov00_0221A928.data+size-1)=0;}ov00_021F3BB4(0,id,config,task);ov00_021F955C()->callback=callback;ov00_021F955C()->arg=arg;ov00_021F956C(2);if(ov00_021F955C()->object==0){dest=ov00_021F955C();first=ov00_021F955C();second=ov00_021F955C();third=ov00_021F955C();dest->object=ov00_02213610(first->a,second->a,third->b,0,20,1,0,ov00_021F85B8,0);}if(ov00_021F955C()->object==0){if(ov00_021F82EC(5)!=0)return;}ov00_021F04A0(3,ov00_02217A60,0);if(ov00_021F82EC(ov00_021F3CF8(ov00_021F955C()->handle))!=0)return;if(ov00_021F955C()->state==0)ov00_021F179C(ov00_021F955C()->handle);ov00_021F7DDC(0);}
