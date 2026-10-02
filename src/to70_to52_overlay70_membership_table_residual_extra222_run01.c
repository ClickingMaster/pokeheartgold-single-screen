#pragma force_active on
#include "global.h"
#pragma force_active on
void *GfGfxLoader_LoadFromNarc_GetSizeOut(int,int,int,int,int,u32 *);
extern const int ov70_02245AAC[];
void *ov70_0223F634(int,int,u32 *),*ov70_0223F658(int,int,u32 *);
void *ov70_0223F634(int heap,int unused,u32 *out){u32 size;void *p=GfGfxLoader_LoadFromNarc_GetSizeOut(74,13,0,heap,0,&size);*out=size>>1;return p;}
void *ov70_0223F658(int heap,int i,u32 *out){u32 size;void *p=GfGfxLoader_LoadFromNarc_GetSizeOut(74,ov70_02245AAC[i],0,heap,1,&size);*out=size>>1;return p;}
