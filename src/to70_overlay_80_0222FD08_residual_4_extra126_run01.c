#pragma force_active on
#include "global.h"
#pragma force_active on
int ov80_02236DD4(int);
int ov80_02236DF8(int,int);
void SaveArray_Party_Init(void *);
void *AllocMonZeroed(int);
int ov80_02237120(void *);
void ov80_0222A140(void *,void *,int);
void ov80_0222A3BC(void *,void *,void *);
void Heap_Free(void *);
void ov80_022307F0(u8 *);
#define P(p,o) (*(void **)((p)+(o)))
#define H(p,o) (*(u16 *)((p)+(o)))
void ov80_022307F0(u8 *w){
 u8 *base;
 int n=ov80_02236DD4(w[4]);
 int m=ov80_02236DF8(w[4],1);
 void *mon;
 int i;
 u8 *p;
 SaveArray_Party_Init(P(w,0x4d4));
 mon=AllocMonZeroed(11);
 i=0;
 if(n>0){
 p=w;base=w+0x280;
 do{
  ov80_0222A140(base+H(p,0x4dc)*56,mon,ov80_02237120(w));
  ov80_0222A3BC(P(w,0x4f8),P(w,0x4d4),mon);
  H(p,0x4e8)=H(w+H(p,0x4dc)*2,0x254);
 i++;p+=2;
 }while(i<n);
 }
 i=0;
 if(m>0){p=w+0x3f0;
 do{
  ov80_0222A140(p,mon,ov80_02237120(w));
  ov80_0222A3BC(P(w,0x4f8),P(w,0x4d8),mon);
 i++;p+=56;
 }while(i<m);
 }
 Heap_Free(mon);
}
