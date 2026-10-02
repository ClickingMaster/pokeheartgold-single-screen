#pragma force_active on
#include "global.h"
extern u32 GetMonData(void*,int,void*);
int ov83_022412DC(void*);
int ov83_022412DC(void*m){int different=0;u32 a;a=GetMonData(m,58,0);if(a!=GetMonData(m,66,0))different=1;a=GetMonData(m,59,0);if(a!=GetMonData(m,67,0))different=1;a=GetMonData(m,60,0);if(a!=GetMonData(m,68,0))different=1;a=GetMonData(m,61,0);if(a!=GetMonData(m,69,0))different=1;return different;}
