#pragma force_active on
#include "global.h"
typedef struct {s16 x,y,w,h,ox,oy;fx32 scaleX,scaleY;u8 rest[44];} GraphicsObject;
typedef struct {u32 p[4];int W,H;} Image;
typedef struct {void *manager; Image *volatile character;void *palette;s16 x,y;u16 paletteIndex,pad;u32 depth,rotation;u16 resourceId,pad2;} GraphicsResourceTemplate;
extern void sub_02015FC4(GraphicsObject *,s16,s16);
extern void sub_02015FD8(GraphicsObject *,s16,s16);
extern void sub_02015FE0(GraphicsObject *,int,int);
extern void sub_02015FF4(GraphicsObject *,u32);
extern void sub_02015FFC(GraphicsObject *,u8);
extern void sub_02016004(GraphicsObject *,void *);
extern void sub_02016008(GraphicsObject *,u32);
extern void sub_0201600C(GraphicsObject *,u32);
extern void sub_02016010(GraphicsObject *,u32);
extern void sub_02016014(GraphicsObject *,int,BOOL);
extern void sub_02016020(GraphicsObject *,u32);
void sub_0201630C(GraphicsObject *,GraphicsResourceTemplate *);
#pragma force_active on
#pragma opt_treetransformation off
void sub_0201630C(GraphicsObject *obj, GraphicsResourceTemplate *t) {
 u32 w,h;int i;
 Image *image=t->character;
 int bound=image->W;w=8;for(i=0;i<bound;i++){bound=t->character->W;w<<=1;}bound=image->H;h=8;for(i=0;i<bound;i++){bound=t->character->H;h<<=1;}
 sub_02015FC4(obj,t->x,t->y);
 sub_02015FD8(obj,(s32)w/2,(s32)h/2);
 obj->scaleX=FX32_ONE;obj->scaleY=FX32_ONE;
 sub_02015FE0(obj,w,h);
 sub_02015FF4(obj,t->rotation);
 sub_02015FFC(obj,t->depth);
 sub_02016004(obj,(u8*)t->character+0x10);
 sub_02016008(obj,NNS_G2dGetImageLocation((NNSG2dImageProxy *)((u8 *)t->character+4),NNS_G2D_VRAM_TYPE_3DMAIN));
 sub_0201600C(obj,NNS_G2dGetImagePaletteLocation((NNSG2dImagePaletteProxy *)((u8 *)t->palette+4),NNS_G2D_VRAM_TYPE_3DMAIN));
 sub_02016010(obj,t->resourceId);
 sub_02016014(obj,0,FALSE);sub_02016014(obj,1,FALSE);
 sub_02016020(obj,t->paletteIndex);
}
