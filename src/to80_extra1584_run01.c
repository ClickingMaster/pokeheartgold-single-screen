#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x288];void*resource;u8 a[0x128];void*ptr;u32 p3[2];int n;void*alt;u32 p4;int(**table)(void*,u8*);u8 p5,status,index;}W;
extern void PokeathlonCourse_SetStateField07(W*,int),PokeathlonCourse_SetField1F4(W*,int),ov96_021E87B4(int,void*,void*,void*);extern int ov96_021E5F24(W*);extern void*ov96_021E9A14(void);extern u8*ov96_021E8A20(void*);int ov96_021E6AE8(W*);
int ov96_021E6AE8(W*w){if(w->table==NULL){PokeathlonCourse_SetStateField07(w,10);return 0;}if(w->ptr==&w->alt){w->ptr=&w->table;w->n=0;}if(w->table[w->index](w,&w->status)){PokeathlonCourse_SetField1F4(w,0);w->ptr=&w->alt;w->n=0;PokeathlonCourse_SetStateField07(w,10);}else if(!ov96_021E5F24(w)){u8*dst,*src;u32 i;ov96_021E87B4(27,(u8*)w+0x2b4,ov96_021E9A14(),w->resource);dst=ov96_021E8A20((u8*)w+0x2dc);src=ov96_021E8A20((u8*)w+0x28c);i=40;do{*dst++=*src++;}while(--i);}else ov96_021E87B4(27,(u8*)w+0x28c,ov96_021E9A14(),w->resource);return 0;}
