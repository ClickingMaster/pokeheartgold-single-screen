#pragma force_active on
#include "global.h"
typedef struct {u8 pad0[0x9c];void *b,*d,*a,*c;u8 padac[0x28];}Row;
typedef struct {void *obj;u32 x4,x8;}Item;
#define PTR(off) (*(void **)(w+(off)))
extern void ov96_021EB408(void *,int,int,int,int),ov96_021EB564(void *,int),ov96_021EB52C(void *,int,int),ov96_021EB588(void *,VecFx32 *);
extern void *ov96_021EB4F4(void *,int,int),*ov96_021EB5E8(void *),*ov96_021EA424(void *,void *,int,void *);
void ov96_021FD128(u8 *,void *);
void ov96_021FD128(u8 *w,void *mgr){int i;VecFx32 pos;Row *rows=(Row *)w;u8 *base;for(i=0;i<30;i++)ov96_021EB408(mgr,2,1,104,6);for(i=0;i<4;i++){ov96_021EB408(mgr,2,1,104,7);ov96_021EB408(mgr,2,1,104,9);ov96_021EB408(mgr,2,1,104,11);ov96_021EB408(mgr,2,1,104,13);}for(i=0;i<4;i++)ov96_021EB408(mgr,2,1,104,14);ov96_021EB408(mgr,1,1,104,12);ov96_021EB408(mgr,1,1,104,15);for(i=0,base=w;i<30;i++){*(void **)(base+0x4d8)=ov96_021EB4F4(mgr,104,6);base+=12;}for(i=0;i<4;i++){rows[i].a=ov96_021EB4F4(mgr,104,7);rows[i].b=ov96_021EB4F4(mgr,104,9);rows[i].c=ov96_021EB4F4(mgr,104,11);rows[i].d=ov96_021EB4F4(mgr,104,13);ov96_021EB564(rows[i].a,3);ov96_021EB564(rows[i].d,8);}for(i=0,base=w;i<4;i++){*(void **)(base+0x3cc)=ov96_021EB4F4(mgr,104,14);base+=4;}PTR(0x1c)=ov96_021EB4F4(mgr,104,12);ov96_021EB564(PTR(0x1c),11);ov96_021EB52C(PTR(0x1c),1,1);pos.x=0x20000;pos.y=0x10000;pos.z=0;ov96_021EB588(PTR(0x1c),&pos);PTR(0x28)=ov96_021EB4F4(mgr,104,15);ov96_021EB564(PTR(0x28),10);PTR(0x20)=ov96_021EA424(PTR(0x3c4),ov96_021EB5E8(PTR(0x18)),0,PTR(0x14));PTR(0x24)=ov96_021EA424(PTR(0x3c4),ov96_021EB5E8(PTR(0x18)),0,PTR(0x14));}
