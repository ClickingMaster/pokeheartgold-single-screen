#pragma force_active on
#include "global.h"
#pragma force_active on
int GF_CosDegNoWrap(u16);int GF_SinDegNoWrap(u16);void ov91_02260EE0(int,int,int,int,int,int*,int*);
static inline int Mul(int a,int b){return (int)(((s64)a*b+2048)>>12);}
void ov91_02260EE0(int step,int count,int radius,int start,int end,int*x,int*y){u16 angle=(s16)(start+((end-start)*step)/count);int cosine=GF_CosDegNoWrap(angle);int r=radius<<12;*x=Mul(cosine,r)>>12;*y=Mul(GF_SinDegNoWrap(angle),r)>>12;}
