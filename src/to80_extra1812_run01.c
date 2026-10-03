#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov96_021F8CFC(int,int,void*);
void ov96_021F895C(u8*w,int x,int y);
void ov96_021F895C(u8*w,int x,int y){*(int*)(w+56)+=x;*(int*)(w+60)+=y;ov96_021F8CFC(x,y,w);}
