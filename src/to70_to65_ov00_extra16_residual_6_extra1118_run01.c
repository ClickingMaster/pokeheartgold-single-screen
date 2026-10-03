#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern int OS_SNPrintf(char*,u32,const char*,...);
typedef struct{u8 p[21];volatile u8 mode,gender;u8 q[0xcd];void*handle;u8 r[0xb8];int phase;u8 s[0x5c];volatile u32 value;u32 saved,override;}Work;
extern Work*ov00_021F955C(void);
extern int ov00_021F3EF4(char*,u32,u8,u8),ov00_02213838(void*,int,int,u8*,int,char*,int);
extern void ov00_0221396C(void*);
typedef struct{u8 type;u8 pad[11];}Entry;
extern Entry ov00_0221A948[];
extern struct{u32 a,b;u32 value;}ov00_0221A928;
extern char ov00_02217AE4[],ov00_02217AF0[],ov00_02217AAC[];
int ov00_021F3CF8(u32);
int ov00_021F3CF8(u32 value){struct{u8 types[161];char text[256];}slots;int count=7,i,n,result;u8*dst;slots.types[0]=8;slots.types[1]=10;slots.types[2]=50;slots.types[3]=51;slots.types[4]=52;slots.types[5]=53;slots.types[6]=54;if(ov00_021F955C()->mode==0||ov00_021F955C()->mode==1){Entry*entry=ov00_0221A948;dst=slots.types+7;for(i=0;i<154;i++,entry++){u8 type=entry->type;if(type){*dst++=type;count++;}}}switch(ov00_021F955C()->phase){case 0:case 1:default:break;case 3:if(ov00_021F955C()->override==0){Work*a=ov00_021F955C();Work*b=ov00_021F955C();Work*c=ov00_021F955C();u32 arg=a->value;u8 gender=b->gender;u8 mode=c->mode;n=ov00_021F3EF4(slots.text,arg,gender,mode);if(ov00_0221A928.value)OS_SNPrintf(slots.text+n,256-n,ov00_02217AE4,ov00_0221A928.value);break;}value=ov00_021F955C()->override;case 2:case 4:case 5:OS_SNPrintf(slots.text,256,ov00_02217AF0,ov00_02217AAC,value);ov00_021F955C()->saved=value;break;}ov00_0221396C(ov00_021F955C()->handle);i=0;do{result=ov00_02213838(ov00_021F955C()->handle,1,0,slots.types,count,slots.text,6);if(result==0||result!=2)break;i++;}while(i<5);if(result==0){Work*w=ov00_021F955C();*(u64*)((u8*)w+0x174)=OS_GetTick()+0xefb5f7;}return result;}
