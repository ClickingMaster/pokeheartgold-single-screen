#pragma force_active on
#include "global.h"
typedef struct{int alpha,scale;volatile u8 state;u8 id;u16 pad;int data[4];} Motion;
typedef struct{u8 pad[0x14];void*obj;u8 pad2[0x7c];Motion m;}Work;
extern u8 ov89_0225AD00(void*);extern void ov89_0225AD64(void*,u8),ModelInstance_SetVisible(void*,int),ModelInstance_SetScale(void*,int,int,int),ov89_0225ADA4(void*,void*,void*,int),ModelInstance_Draw(void*);
int ov89_0225B07C(void*,Work*);void ov89_0225B100(void*,Work*);
int ov89_0225B07C(void*p,Work*w){Motion*m=&w->m;switch(m->state){case 0:m->alpha=31<<8;m->id=ov89_0225AD00(p);m->scale=4096;m->state++;case 1:m->scale+=192;if(m->alpha-192<256){ModelInstance_SetVisible((u8*)w+0x1c,0);m->state++;}else{m->alpha-=192;ModelInstance_SetScale((u8*)w+0x1c,m->scale,m->scale,4096);}break;default:ov89_0225AD64(p,m->id);return 1;}ov89_0225ADA4((u8*)w+0x1c,w,m->data,1);return 0;}
void ov89_0225B100(void*p,Work*w){Motion*m=&w->m;if(m->id!=255){NNS_G3dGlbPolygonAttr(0,GX_POLYGONMODE_MODULATE,GX_CULL_ALL,m->id,m->alpha>>8,0);NNSi_G3dModifyPolygonAttrMask(w->obj,0,0x3f000000);NNSi_G3dModifyPolygonAttrMask(w->obj,0,0x1f0000);}ModelInstance_Draw((u8*)w+0x1c);}
