#pragma force_active on
// clang-format off
#include "global.h"
#pragma force_active on
#include <nitro/code32.h>
// clang-format on
typedef struct{u32 a,b,c;u32**list;} S;extern S*ov00_021FF6E4(void);extern int ov00_021FF770(u32*);u32 ov00_0220FBE4(void);
#pragma opt_propagation off
u32 ov00_0220FBE4(void){int i;S*s;u32 result=0,value;s=ov00_021FF6E4();if(!s)return result;for(i=0;;i++){u32*p=s->list[i];if(!p)break;value=*p;if(value!=0x0100007f){result=value;if(ov00_021FF770(p)!=0)return result;}}return result;}
