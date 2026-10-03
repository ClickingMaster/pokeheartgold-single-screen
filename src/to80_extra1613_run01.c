#pragma force_active on
#include "global.h"
#include <nnsys/g2d/g2d_Softsprite.h>
typedef struct {GXTexSizeS s;GXTexSizeT t;GXTexFmt fmt;u32 xc;GXTexPlttColor0 transparent;}Texture;
typedef struct {s16 x,y,cx,cy,width,height;fx32 sx,sy;int z;int rotation;u32 x1c,x20;Texture *texture;u32 address,palette,paletteOffset;int flipX,flipY;u16 color;u8 alpha;}Work;
void sub_020161CC(Work *);
static inline void Scale(int x,int y,int z){*(vu32 *)0x0400046c=x;*(vu32 *)0x0400046c=y;*(vu32 *)0x0400046c=z;}
void sub_020161CC(Work *w){int u0,u1,v0,v1;G3_PushMtx();*(vu32 *)0x040004c0=0x7fff8000|w->color;*(vu32 *)0x040004c4=0x4210;G3_TexImageParam(w->texture->fmt,(GXTexGen)1,w->texture->s,w->texture->t,(GXTexRepeat)0,(GXTexFlip)0,w->texture->transparent,w->address);G3_TexPlttBase(w->palette+(w->paletteOffset<<5),w->texture->fmt);G3_PolygonAttr(0,(GXPolygonMode)0,(GXCull)3,0,w->alpha,0);if(w->flipX){u0=w->width;u1=0;}else{u1=w->width;u0=0;}if(w->flipY){v0=w->height;v1=0;}else{v1=w->height;v0=0;}G3_Translate((w->x+w->cx)<<12,(w->y+w->cy)<<12,w->z<<12);G3_RotZ(FX_SinIdx(w->rotation),FX_CosIdx(w->rotation));Scale(w->sx,w->sy,FX32_ONE);G3_Translate(-w->cx<<12,-w->cy<<12,0);NNS_G2dDrawSpriteFast(0,0,0,w->width,w->height,u0,v0,u1,v1);G3_PopMtx(1);}
