#pragma force_active on
#include "global.h"
#pragma force_active on
int GF2DGfxResObj_GetResID(void *);
void CreateSpriteResourcesHeader(void *,int,int,int,int,int,int,int,int,void *,void *,void *,void *,int,int);
void ov41_0224B0B8(void **,void **,void *,int);
void ov41_0224B0B8(void **r,void **m,void *h,int n){int a=GF2DGfxResObj_GetResID(r[0]),b=GF2DGfxResObj_GetResID(r[1]),c=GF2DGfxResObj_GetResID(r[2]);CreateSpriteResourcesHeader(h,a,b,c,GF2DGfxResObj_GetResID(r[3]),-1,-1,0,n,m[0],m[1],m[2],m[3],0,0);}
