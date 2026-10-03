#pragma force_active on
#include "global.h"
extern int ov51_021E6AAC(void *,int *),ov51_021E6A54(void *);int ov51_021E6B44(void *);
int ov51_021E6B44(void *w){int flag=0;*(int*)((u8*)w+0x3108)=-1;{int result=ov51_021E6AAC(w,&flag);if(flag){*(int*)((u8*)w+0x310c)=1;return result;}result=ov51_021E6A54(w);if(result)*(int*)((u8*)w+0x310c)=0;return result;}}
