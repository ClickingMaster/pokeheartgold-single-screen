#pragma force_active on
#include "global.h"
#pragma force_active on
void ov92_02263108(float *,const float *,const float *);
void ov92_02263108(float *d,const float *a,const float *b){
 d[0]=a[0]*b[0]-a[1]*b[1]-a[2]*b[2]-a[3]*b[3];
 d[1]=a[2]*b[3]+(a[0]*b[1]+a[1]*b[0])-a[3]*b[2];
 d[2]=a[3]*b[1]+(a[2]*b[0]+(a[0]*b[2]-a[1]*b[3]));
 d[3]=a[3]*b[0]+(a[0]*b[3]+a[1]*b[2]-a[2]*b[1]);
}
