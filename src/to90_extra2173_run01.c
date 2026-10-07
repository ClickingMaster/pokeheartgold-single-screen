#pragma force_active on
#include "global.h"
#pragma force_active on
extern void*Heap_Alloc(int,u32);void*ov01_021EFFEC(void);
void*ov01_021EFFEC(void){void*w=Heap_Alloc(4,48);memset(w,0,48);G2_SetWnd0InsidePlane(31,1);G2_SetWnd1InsidePlane(0,0);return w;}
