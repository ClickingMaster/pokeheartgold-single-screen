#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 padding[0x670];void*managed;}SpriteSlot;
NNSG2dImageProxy*Sprite_GetImageProxy(void*);
void PokedexApp_LoadSpriteCharData(void*,int,const void*,u32,NNS_G2D_VRAM_TYPE);
void PokedexApp_LoadSpriteCharData(void*p,int index,const void*data,u32 size,NNS_G2D_VRAM_TYPE type){u32 location;void*managed=((SpriteSlot*)((u8*)p+index*4))->managed;location=NNS_G2dGetImageLocation(Sprite_GetImageProxy(*(void**)managed),type);DC_FlushRange(data,size);if(type==1)GX_LoadOBJ(data,location,size);else GXS_LoadOBJ(data,location,size);}
