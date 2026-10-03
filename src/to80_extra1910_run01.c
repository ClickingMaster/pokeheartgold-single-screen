#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern int ov38_02223224(void*,int,void*,int,int);
int ov38_022234D8(void*w,int a);
#pragma force_active on
int ov38_022234D8(void*w,int a){char buf[2];int n;int i;int total=0;i=0;buf[0]=0;buf[1]=0;for(;buf[i&1]!=13||buf[(i-1)&1]!=10;i++){n=ov38_02223224(w,a,buf+(i&1),1,0);if(n<=0)return n;total+=n;}return total;}
