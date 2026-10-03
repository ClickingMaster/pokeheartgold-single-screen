#pragma force_active on
#include "global.h"
extern int ov15_021FB3F0(void *),ov15_021FD7D0(void *,u8,int,int,int);
int ov15_021FB5AC(void *);
int ov15_021FB5AC(void *w){int result=ov15_021FB3F0(w);switch(result){case -2:return ov15_021FD7D0(w,19,9,8,28);default:*(int*)((u8*)w+0x948)=result;return ov15_021FD7D0(w,(u8)(result+28),9,8,29);case -1:return 4;}}
