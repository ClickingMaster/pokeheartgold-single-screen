#pragma force_active on
#pragma force_active on
#include "global.h"
#include "constants/heap.h"
typedef struct {int heap;u8 p[0x29fc-4];void*a;u8 q[0x2bb4-0x2a00];void*b;u8 r[0x3d0c-0x2bb8];} Work;
extern void ov74_0222FCC4(void*),LoadFontPal0(int,int,int),ov74_0222F024(Work*,int,int),Heap_Free(void*);extern void*Heap_AllocAtEnd(int,u32),*memset(void*,int,u32);
void ov74_0222FC50(void*,void*,enum HeapID);
void ov74_0222FC50(void*a,void*b,enum HeapID heap){int h=heap;Work*w;ov74_0222FCC4(a);w=Heap_AllocAtEnd(h,0x3d0c);memset(w,0,0x3d0c);w->a=a;LoadFontPal0(0,480,w->heap=h);w->b=b;ov74_0222F024(w,1,0);Heap_Free(w);}
