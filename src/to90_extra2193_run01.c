#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov18_021F12C8(void*,int,s16*,s16*,int),ov18_021F1294(void*,int,s16,s16,int);void ov18_021F4FC8(void*,int,int);
void ov18_021F4FC8(void*w,int id,int from){s16 x,y;ov18_021F12C8(w,from,&x,&y,1);y-=16;ov18_021F1294(w,id,x,y,1);}
