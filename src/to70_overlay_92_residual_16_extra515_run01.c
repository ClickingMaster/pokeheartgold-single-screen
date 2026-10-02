#pragma force_active on
#include "global.h"
typedef struct{int current,start,speed,accel,t,total;} Work;
void ov92_02260798(Work*,int,int,int,int);
int ov92_022607F8(Work*);
void ov92_02260798(Work*w,int start,int end,int speed,int n){int a=FX_Div((int)(((s64)(end-start-(int)(((s64)speed*(n<<12)+2048)>>12))*8192+2048)>>12),(n*n)<<12);w->current=start;w->start=start;w->speed=speed;w->accel=a;w->t=0;w->total=n;}
int ov92_022607F8(Work*w){int t=w->t;int a=(int)(((s64)w->speed*(t<<12)+2048)>>12);int b=(int)(((s64)w->accel*((t*t)<<12)+2048)>>12);w->current=w->start+(a+FX_Div(b,8192));t=w->t;b=w->total;t++;if(t<=b){w->t=t;return 0;}w->t=b;return 1;}
