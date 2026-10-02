#pragma force_active on
#include "global.h"
#pragma force_active on
void ov96_0220404C(int,int,float *,float *);
void ov96_02204134(float,float,int *,int *);
void ov96_0220404C(int first,int second,float *out1,float *out2) { float a=(float)first,b=(float)second; float x=(float)(22.53846153846154+b*5.0/52.0); float y=(float)((a*0.19375+b/8.0-15.5-10.0)/(0.39402173913043476+b/512.0)); *out2=x; *out1=y; }
void ov96_02204134(float a,float b,int *out1,int *out2) { float x=(float)(4.0/(1.0+(b-57.0)*(b-57.0))+(11.0+(b*b*b)/7600.0-(24.0-b)*(24.0-b)*(24.0-b)/4400.0-1.0)); x*=8.0; *out2=(int)x; float y=(float)((10.0+0.34782608695652173*a)-(0.6451612903225806*(x/8.0-10.0)*(64.0-a)/64.0)); y*=8.0; *out1=(int)y; }
