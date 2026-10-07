#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
extern void ov80_022329B4(void*),ov80_0222A52C(void*,void*,void*,int,void*,int,int,int),ov80_0222A140(void*,void*,int),ov80_02237E18(void*,void*,void*);extern int sub_02030D84(void*,int,u8,int,int),ov80_02237D88(void*);extern void*AllocMonZeroed(int);extern void Heap_Free(void*);void ov80_02232908(void*);
void ov80_02232908(void*w){struct{u32 a[2],b[6];u16 ids[6];u32 templates[6][14];}frame;int i,j,k;u8*dst,*record;void*mon;ov80_022329B4(w);dst=w;for(i=0;i<14;i++,dst+=2)H(dst,48)=sub_02030D84(P(w,8),6,(u8)i,0,0);j=0;u16*id=frame.ids;dst=w;for(;j<4;j++,id++,dst+=2){*id=sub_02030D84(P(w,8),8,(u8)j,0,0);H(dst,0x26c)=*id;}ov80_0222A52C(frame.templates,frame.ids,frame.a,0,frame.b,4,11,205);mon=AllocMonZeroed(11);k=0;record=(u8*)frame.templates;for(;k<4;k++,record+=56){ov80_0222A140(record,mon,ov80_02237D88(w));ov80_02237E18(w,P(w,44),mon);}Heap_Free(mon);}
