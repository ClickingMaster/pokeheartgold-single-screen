#pragma force_active on
#include "global.h"
extern void ov96_021F8A98(void*,void*,void*,int);
extern void*ov96_021F8ACC(void*,int,int);
extern NNS_G2D_VRAM_TYPE Sprite_GetVramType(void*);
extern void*Sprite_GetImageProxy(void*),*Sprite_GetPaletteProxy(void*);
extern void Sprite_SetImageProxy(void*,void*),Sprite_SetPaletteProxy(void*,void*),Sprite_SetPalOffset(void*,int);
extern void NNS_G2dSetImageLocation(NNSG2dImageProxy*,NNS_G2D_VRAM_TYPE,u32),NNS_G2dSetImagePaletteLocation(NNSG2dImagePaletteProxy*,NNS_G2D_VRAM_TYPE,u32);
void ov96_021F8AFC(void*,int,void**);
void ov96_021F8AFC(void*w,int n,void**out){u8 desc[48];void*s,*image,*pal;NNS_G2D_VRAM_TYPE type;struct{volatile u32 loc,offset,ploc,poffset;}slots;ov96_021F8A98(desc,w,(u8*)w+0x2d8,1);out[0]=ov96_021F8ACC(desc,17,4);out[1]=ov96_021F8ACC(desc,5,0);out[2]=ov96_021F8ACC(desc,18,1);out[3]=ov96_021F8ACC(desc,0,2);s=out[3];type=Sprite_GetVramType(s);image=Sprite_GetImageProxy(s);pal=Sprite_GetPaletteProxy(s);slots.loc=NNS_G2dGetImageLocation(image,type);slots.offset=n<<9;NNS_G2dInitImageProxy(image);NNS_G2dSetImageLocation(image,type,slots.loc+slots.offset);slots.ploc=NNS_G2dGetImagePaletteLocation(pal,type);slots.poffset=(n+6)<<5;NNS_G2dInitImagePaletteProxy(pal);NNS_G2dSetImagePaletteLocation(pal,type,slots.ploc+slots.poffset);Sprite_SetImageProxy(s,image);Sprite_SetPaletteProxy(s,pal);Sprite_SetPalOffset(s,n);}
