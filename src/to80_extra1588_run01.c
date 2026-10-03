#pragma force_active on
#include "global.h"
typedef struct {void *a,*d,*c,*b;u8 rest[0x10];}Row;
typedef struct {u32 ctx;u8 pad4[0x44];Row rows[4];u8 padc8[0xb4];void *sprites[6];}Work;
extern const u8 ov96_0221C718[],ov96_0221C720[];
extern void ov96_021EB408(void *,int,int,int,int),ov96_021EB564(void *,int),ov96_021EB52C(void *,int,int),ov96_021EB588(void *,VecFx32 *),ov96_02200C8C(void *,Work *),Sprite_SetDrawFlag(void *,int),Sprite_SetMatrix(void *,VecFx32 *),Sprite_SetAnimCtrlSeq(void *,int);
extern void *ov96_021EB4F4(void *,int,int),*ov96_021EB3E4(void *,int,int,int,int),*ov96_021EB5E8(void *),*ov96_021EA2C4(void *,void *,int,u32);
void ov96_02200180(Work *,void *,void *);
void ov96_02200180(Work *w,void *mgr,void *owner){int i,j;void *obj;VecFx32 b,a;for(i=0;i<4;i++){ov96_021EB408(mgr,3,2,101,2);ov96_021EB408(mgr,3,2,101,3);ov96_021EB408(mgr,3,2,101,8);ov96_021EB408(mgr,3,2,101,10);}for(i=0;i<2;i++)ov96_021EB408(mgr,3,2,101,16);for(i=0;i<4;i++){w->rows[i].a=ov96_021EB4F4(mgr,101,2);w->rows[i].b=ov96_021EB4F4(mgr,101,3);w->rows[i].c=ov96_021EB4F4(mgr,101,8);w->rows[i].d=ov96_021EB4F4(mgr,101,10);ov96_021EB564(w->rows[i].a,i);ov96_021EB564(w->rows[i].b,i+4);ov96_021EB564(w->rows[i].c,10);}obj=ov96_021EB3E4(mgr,3,2,101,17);ov96_021EB52C(obj,1,1);ov96_021EB564(obj,11);a.z=0;a.x=0x80000;a.y=0x238000;ov96_021EB588(obj,&a);ov96_02200C8C(mgr,w);for(j=0;j<6;j++){w->sprites[j]=ov96_021EA2C4(owner,ov96_021EB5E8(mgr),3,w->ctx);Sprite_SetDrawFlag(w->sprites[j],1);b.x=ov96_0221C718[j]<<12;b.y=0x238000;b.z=0;Sprite_SetMatrix(w->sprites[j],&b);Sprite_SetAnimCtrlSeq(w->sprites[j],ov96_0221C720[j]);}}
