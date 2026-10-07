#pragma force_active on
#include "global.h"
#pragma force_active on
void *GfGfxLoader_LoadFromNarc_GetSizeOut(int,int,int,int,int,u32*);void *ov40_0222DD68(int,int,u32*);
void *ov40_0222DD68(int heap,int unused,u32*count){u32 size;void*data=GfGfxLoader_LoadFromNarc_GetSizeOut(74,13,0,heap,0,&size);*count=size>>1;return data;}
