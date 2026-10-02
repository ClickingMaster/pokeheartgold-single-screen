#pragma force_active on
#include "global.h"
extern int ov96_0220E888(int);
void ov96_0220E890(int *p,int step);
int ov96_0220E8C0(u8*p,int target,float*out);
void ov96_0220E890(int *p,int step){if(*p!=0){int a=*p<0?-*p:*p;if(a<step){*p=0;return;}*p-=(*p>0?1:-1)*step;}}
int ov96_0220E8C0(u8*p,int target,float*out){int value=0;float result=1.0f;int old,diff;float f;int speed;if(p==NULL)GF_AssertFail();old=p[0];diff=ov96_0220E888(target-old)-ov96_0220E888(old);if(diff!=0){if(p[1]!=0)f=0.5f+(float)(p[1]<<12);else f=(float)(p[1]<<12)-0.5f;speed=(int)f;if(speed!=0){value=(speed/10)*diff;if(value<=-98304)value=-98304;result=result-(float)((value-16384)/100)/4096.0f;}}*out=result;return value>>12;}
