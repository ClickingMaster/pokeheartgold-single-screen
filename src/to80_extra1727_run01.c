#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern int sprintf(char*,const char*,...);
#include <string.h>
extern const char ov00_0221A520[];extern u8 __lower_mapC[];extern int ov00_02213FD4(const char*,int);extern void ov00_02212CD0(void*,const char*,const char*);
static inline u32 swap16(s32 value){return ((value>>8)&255)|((value<<8)&0xff00);}
static inline int lower(int c){if(c<0||c>=128)return c;return __lower_mapC[c];}
void ov00_02213044(void*,const char*,int);u32 ov00_0221321C(const char*,u32);
void ov00_02213044(void*ctx,const char*p,int left){char buffer[128];union{volatile u16 word;volatile u8 bytes[2];}count;const char*names;int group;while(*p){const char*name;int length=ov00_02213FD4(p,left);if(length<0)return;name=p;left-=length;p+=length;length=ov00_02213FD4(p,left);if(length<0)return;{const char*value=p;p+=length;left-=length;ov00_02212CD0(ctx,name,value);}}p++;left--;for(group=0;group<2;group++){int fields,iteration;if(left<2)return;memcpy((void*)&count,p,2);p+=2;names=p;left-=2;count.word=swap16(count.word);fields=0;while(*p){int length=ov00_02213FD4(p,left);if(length<0||length>100)return;p+=length;fields++;left-=length;}p++;left--;for(iteration=0;iteration<count.word;iteration++){int field;const char*name;for(name=names,field=0;field<fields;field++){int length=ov00_02213FD4(p,left);if(length<0)return;sprintf(buffer,ov00_0221A520,name,iteration);ov00_02212CD0(ctx,buffer,p);p+=length;left-=length;name+=strlen(name)+1;}}}}
