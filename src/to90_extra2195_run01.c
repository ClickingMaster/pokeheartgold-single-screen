#pragma force_active on
#include "global.h"
#pragma force_active on
u32 ov18_021F6214(void*,int,const u8*);int ov18_021F6230(void*,int,const u8*,int,u32);
int ov18_021F6230(void*p,int idx,const u8*table,int x,u32 width){u32 text=ov18_021F6214(p,idx,table);return x-(width>>1)+(text>>1);}
