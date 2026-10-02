#pragma force_active on
#include "global.h"
#pragma force_active on
struct Pair{int a,b;};struct Table{int pad[6];struct Pair ids;};extern const struct Table ov96_0221CBCC;struct S{u8 pad[0x120];u32 image[2];u32 palette[2];};void**ov96_021EB5EC(void*,int,int);void*SpriteTransfer_GetCharProxy(void*);void*SpriteTransfer_GetPaletteProxy(void*,void*);u32 NNS_G2dGetImageLocation(const NNSG2dImageProxy*,NNS_G2D_VRAM_TYPE);u32 NNS_G2dGetImagePaletteLocation(const NNSG2dImagePaletteProxy*,NNS_G2D_VRAM_TYPE);void ov96_022089D0(void*,struct S*);
void ov96_022089D0(void*c,struct S*s){struct Pair p=ov96_0221CBCC.ids;u8 i;for(i=0;i<2;i++){int id=((int*)&p)[i];void*a=*ov96_021EB5EC(c,id,0);void*b=*ov96_021EB5EC(c,id,1);NNSG2dImageProxy*image=SpriteTransfer_GetCharProxy(a);NNSG2dImagePaletteProxy*palette=SpriteTransfer_GetPaletteProxy(b,image);s->image[i]=NNS_G2dGetImageLocation(image,NNS_G2D_VRAM_TYPE_2DSUB);s->palette[i]=NNS_G2dGetImagePaletteLocation(palette,NNS_G2D_VRAM_TYPE_2DSUB);}}
