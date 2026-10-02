#pragma force_active on
#include "global.h"
#pragma force_active on
extern u8 gSystem[];int ov41_02249768(void *),ov41_02249774(void *),ov41_022464BC(void *,int,int,int),ov41_022497A8(void *,int *,int *,void *),ov41_02249820(void *,int,int,void *);void ov41_02249780(void *,int *,int *),ov41_022497A0(void *,int *,int *);
static inline void adjust(int *x,int *y,int w,int h){*x-=w/2;*y-=h/2;}
int ov41_022497A8(void *s,int *x,int *y,void *rect){int w,h,px,py;if(ov41_02249768(s)==0)return 0;ov41_02249780(s,&px,&py);ov41_022497A0(s,&w,&h);px-=w/2;py-=h/2;*x=*(u16 *)(gSystem+96)-px;*y=*(u16 *)(gSystem+98)-py;if(ov41_022464BC(rect,*x,*y,0)==0)return 1;return 0;}
