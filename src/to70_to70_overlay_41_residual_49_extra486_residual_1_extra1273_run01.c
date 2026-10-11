#pragma force_active on
#include "global.h"
extern int ov41_02249774(void*),DressUpGfx_CharPixelEquals(int,int,int,int);extern void ov41_02249780(void*,int*,int*),ov41_022497A0(void*,int*,int*);
int ov41_02249820(void*w,int x,int y,int a);
int ov41_02249820(void*w,int x,int y,int a){int ww,h,px,py;if(!ov41_02249774(w))return 0;ov41_02249780(w,&px,&py);ov41_022497A0(w,&ww,&h);px-=ww/2;py-=h/2;y-=py;x-=px;if(!DressUpGfx_CharPixelEquals(a,x,y,0))return 1;return 0;}
