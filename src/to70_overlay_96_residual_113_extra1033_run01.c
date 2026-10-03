#pragma force_active on
#include "global.h"

int ov96_022158EC(VecFx32*a,VecFx32*b,int distance);
void ov96_02215914(int*p,int amount);

int ov96_022158EC(VecFx32*a,VecFx32*b,int distance){VecFx32 v;VEC_Subtract(b,a,&v);if(VEC_Mag(&v)<=distance)return 1;return 0;}
void ov96_02215914(int*p,int amount){if(*p!=0){if((*p<0?-*p:*p)<amount){*p=0;return;}*p-=amount*(*p>0?1:-1);}}
