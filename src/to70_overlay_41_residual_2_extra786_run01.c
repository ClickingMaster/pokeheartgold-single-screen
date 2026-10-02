#pragma force_active on
#include "global.h"
#pragma force_active on
#include "system.h"
extern int ov41_02245F04(void *);extern void ov41_02245FA8(void *,int *,int *);extern int ov41_022464BC(void *,int,int,int);int ov41_02245F14(void *,int *,int *,void *);
int ov41_02245F14(void *w,int *x,int *y,void *map){int a,b;int i,j;if(!ov41_02245F04(w))return 0;ov41_02245FA8(w,&a,&b);*x=gSystem.touchX-a;*y=gSystem.touchY-b;for(i=*y-4;i<*y+4;i++){if(i>=0){for(j=*x-4;j<*x+4;j++){if(j>=0){if(ov41_022464BC(map,j,i,0)==0)return 1;}}}}return 0;}
