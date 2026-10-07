#pragma force_active on
#include "global.h"
#include "filesystem.h"
#include "overlay_28_helpers_internal.h"
#include "sprite.h"
#pragma force_active on
typedef struct {volatile u8 a,b,c,d;} Flags;
extern void ov28_0225D92C(SpriteResource **,GF_2DGfxResMan **,NARC *,int,int,int,int,int,int,int,int,int);
extern void ov28_0225D9BC(SpriteResource **,GF_2DGfxResMan **,SpriteResourcesHeader *,int);
extern void ov28_0225DA1C(DowsingMchn *,int,SpriteResourcesHeader *,const u8 *);
void ov28_0225DB8C(DowsingMchn *,NARC *);
void ov28_0225DB8C(DowsingMchn *w,NARC *narc){SpriteResourcesHeader h;Flags f={0};u32 i;ov28_0225D92C((SpriteResource **)w->resObjB,w->resMans,narc,4,7,5,6,9,499,499,499,499);ov28_0225D9BC((SpriteResource **)w->resObjB,w->resMans,&h,2);for(i=0;i<4;i++)ov28_0225DA1C(w,i+4,&h,(const u8 *)(const void *)&f);}
