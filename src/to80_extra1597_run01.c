#pragma force_active on
#include "global.h"
#define PTR(off) (*(void **)(w+(off)))
extern void sub_02032674(void *,int *,int *),BufferIntegerAsString(void *,int,int,int,int,int),ov112_021E7CA4(void *,int,int),ov112_021E98E8(void *,int),ov112_021EA010(void *,int,void *,int,int),ov112_021EA044(void *,int,int,int),Pokewalker_TryGetBoxMon(void *,void *),BufferBoxMonNickname(void *,int,void *),ov112_021E9290(void *,void *,void *,int),ov112_021EA670(void *,int),ov112_021EA688(void *,int),FillWindowPixelBuffer(void *,int),CopyWindowToVram(void *),ov112_021EA64C(void *),Heap_Free(void *);
extern int ov112_021E9610(void *,int);
extern void *AllocMonZeroed(int),*Mon_GetBoxMon(void *);
void ov112_021EADD0(u8 *,int);
void ov112_021EADD0(u8 *w,int exists){int a,b,value;void *mon,*box;sub_02032674(PTR(0x1e440),&a,&b);BufferIntegerAsString(PTR(0x1e448),7,b,7,0,1);BufferIntegerAsString(PTR(0x1e448),6,a,7,0,1);value=ov112_021E9610(w,b);BufferIntegerAsString(PTR(0x1e448),12,value,7,0,1);ov112_021E7CA4(w,5,16);ov112_021E98E8(w,1);*(u16 *)(w+0x1f2e0)=0;ov112_021EA010(w,1,PTR(0x1e50c),0,0x10200);ov112_021EA044(w,2,83,0x10200);ov112_021EA010(w,3,PTR(0x1e510),0,0x10200);ov112_021EA044(w,4,84,0x10200);if(value<0)ov112_021EA044(w,0,88,0x10200);else if(value==0)ov112_021EA044(w,0,87,0x10200);else ov112_021EA044(w,0,86,0x10200);
 if(exists){mon=AllocMonZeroed(154);box=Mon_GetBoxMon(mon);Pokewalker_TryGetBoxMon(PTR(0x1e440),box);BufferBoxMonNickname(PTR(0x1e448),13,box);ov112_021EA044(w,5,89,0x30400);ov112_021E9290(box,w+0x1d7ac,w+0x1d79c,1);ov112_021EA670(w,7);Heap_Free(mon);}else{ov112_021EA688(w,7);FillWindowPixelBuffer(w+0x1ebf8,0);CopyWindowToVram(w+0x1ebf8);}ov112_021EA64C(w);}
